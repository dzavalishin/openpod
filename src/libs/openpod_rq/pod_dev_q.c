#include <pod_driver.h>
#include <pod_device.h>
#include <pod_dev_q.h>
#include <pod_queues.h>

#include <errno.h>

// Sketch code

static errno_t pod_dev_q_refuse( pod_device *dev, pod_request *rq );
static void pod_dev_q_runner( void *arg );


static void pod_dev_q_log_rc( pod_device *dev, const char *descr, int rc )
{
    if( rc == 0 ) return;
    pod_kernel_log( "drv %s dev %s: %s, rc = %d", dev->drv->name, dev->name, descr, rc );
}



errno_t	pod_dev_q_construct( pod_device *dev )
{
	// Don't attempt serving requests until constructed
	dev->calls->enqueue = pod_dev_q_refuse;

	dev->calls->dequeue = pod_dev_q_dequeue;
	dev->calls->fence = pod_dev_q_fence;
	dev->calls->raise_prio = pod_dev_q_raise;

	dev->curr_rq = 0;

	// TODO use destruct to cleanup
	dev->rq_run_mutex = 0;
	dev->rq_run_cond = 0; 
        //dev->rq_run_thread.system_thread_id = 0;
        POD_THREAD_NOT_NULL( dev->rq_run_thread );
	dev->default_r_q = 0;

	errno_t rc;

	//dev->flags |= OPENPOD_DEV_INTERNAL_THREAD_RUN;
        POD_DEV_INTERNAL_SET( dev, OPENPOD_DEV_INTERNAL_THREAD_RUN );

	rc = pod_q_construct( &(dev->default_r_q) );
	if( rc ) return rc;

	rc = pod_kernel_create_mutex( &(dev->rq_run_mutex) );
	if( rc ) goto err_q;

	rc = pod_kernel_create_cond( &(dev->rq_run_cond) );
	if( rc ) goto err_mutex;

	rc = pod_kernel_thread_start( &(dev->rq_run_thread), pod_dev_q_runner, dev );
	if( rc ) goto err_cond;

	// Initialized, let them send us requests

	dev->calls->enqueue = pod_dev_q_enqueue;

	return 0;


err_thread:
	pod_dev_q_log_rc( dev, "kill thread", 
        pod_kernel_thread_kill( dev->rq_run_thread ) );

err_cond:
	pod_dev_q_log_rc( dev, "destroy cond", 
	    pod_kernel_destroy_cond( dev->rq_run_cond ) );

err_mutex:
	pod_dev_q_log_rc( dev, "destroy mutex", 
	    pod_kernel_destroy_mutex( dev->rq_run_mutex ) );

err_q:
	pod_dev_q_log_rc( dev, "destroy q", 
		pod_q_destruct( dev->default_r_q ) );

	//dev->flags &= ~OPENPOD_DEV_INTERNAL_THREAD_RUN;
	//dev->flags &= ~OPENPOD_DEV_INTERNAL_THREAD_RUNNING;
    POD_DEV_INTERNAL_CLEAR( dev, OPENPOD_DEV_INTERNAL_THREAD_RUN );
    POD_DEV_INTERNAL_CLEAR( dev, OPENPOD_DEV_INTERNAL_THREAD_RUNNING );

	return rc;

}

errno_t	pod_dev_q_destruct( pod_device *dev )
{
	// Stop serving requests
	dev->calls->enqueue = pod_dev_q_refuse;

	// Request thread to stop and check it
	//dev->flags &= ~OPENPOD_DEV_INTERNAL_THREAD_RUN;
        POD_DEV_INTERNAL_CLEAR( dev, OPENPOD_DEV_INTERNAL_THREAD_RUN );
        pod_kernel_signal_cond( dev->rq_run_cond );

	// TODO wait for thread to die - use mutex?

	// Clean up current request, if any

	// Clean up requests in a q, if any
	while( !pod_q_is_empty( dev->default_r_q ) )
	{
	    pod_request *next_rq;

	    pod_kernel_lock_mutex( dev->rq_run_mutex );

	    pod_q_get_last( dev->default_r_q, &next_rq ); 


	    // refuse request
	    if( next_rq )
	    {
		next_rq->err = pod_rq_status_stopped;
	        // TODO ERR can block frv from running,
	        // need some thread pool to run done?
	        next_rq->done( next_rq );
	    }
	    pod_kernel_unlock_mutex( dev->rq_run_mutex );
    
	}



	if( POD_THREAD_NOT_NULL( dev->rq_run_thread ) )
		pod_dev_q_log_rc( dev, "destroy thread", pod_kernel_thread_kill( dev->rq_run_thread ) );

	if( dev->rq_run_cond )
		pod_dev_q_log_rc( dev, "destroy cond", pod_kernel_destroy_cond( dev->rq_run_cond ) );

	if( dev->rq_run_mutex )
		pod_dev_q_log_rc( dev, "destroy mutex", pod_kernel_destroy_mutex( dev->rq_run_mutex ) );

	if( dev->default_r_q )
		pod_dev_q_log_rc( dev, "destroy q", pod_q_destruct( dev->default_r_q ) );

}

// Replacement for a pod_dev_q_enqueue - just refuse all incoming rq
static errno_t pod_dev_q_refuse( pod_device *dev, pod_request *rq )
{
	return EINVAL; // TODO confirm and document ret codes
}


// должна быть реализована в драйвере, запускает запрос на исполнение 
//errno_t	pod_dev_q_exec( pod_device *dev, pod_request *rq );

// реализована в фреймворке, вызывается кодом драйвера по завершении в/в, можно из прерывания
errno_t	pod_dev_q_iodone( pod_device *dev )
{

    // TODO must state that signal_cond can be called in interrupt
    pod_kernel_signal_cond( dev->rq_run_cond );

}

// Thread that executes requests from q and calls callbacks
void	pod_dev_q_runner( void *arg )
{
    pod_device *dev = arg;

    errno_t rc;

    //dev->flags |= OPENPOD_DEV_INTERNAL_THREAD_RUNNING;
    POD_DEV_INTERNAL_SET( dev, OPENPOD_DEV_INTERNAL_THREAD_RUNNING );

    while( POD_DEV_INTERNAL_CHECK( dev, OPENPOD_DEV_INTERNAL_THREAD_RUN ) )
    {
	pod_request *next_rq;
	pod_request *prev_rq;

	pod_kernel_lock_mutex( dev->rq_run_mutex );

	pod_kernel_wait_cond( dev->rq_run_cond, dev->rq_run_mutex );

	prev_rq = dev->curr_rq;
	dev->curr_rq = 0;

	//rc = 
	pod_q_get_last( dev->default_r_q, &next_rq ); 

	pod_kernel_unlock_mutex( dev->rq_run_mutex );

	// Start next request
	if( next_rq )
	{
	    dev->curr_rq = next_rq;
	    dev->default_start_rq( dev, next_rq );
	}

	if( prev_rq )
	{
	    // TODO ERR can block frv from running,
	    // need some thread pool to run done?
	    prev_rq->done( prev_rq );
	}

    }

    //dev->flags &= ~OPENPOD_DEV_INTERNAL_THREAD_RUNNING;
    POD_DEV_INTERNAL_CLEAR( dev, OPENPOD_DEV_INTERNAL_THREAD_RUNNING );

}



errno_t	pod_dev_q_enqueue( pod_device *dev, pod_request *rq )
{
    errno_t rc = 0;

    pod_kernel_lock_mutex( dev->rq_run_mutex );

    rc = pod_q_enqueue( dev->default_r_q, rq );

    if( !rc )
        rc = pod_q_sort( dev->default_r_q, rq_prio_cmp ); 

    pod_kernel_unlock_mutex( dev->rq_run_mutex );

    return rc;
}



errno_t	pod_dev_q_dequeue( pod_device *dev, pod_request *rq )
{
    errno_t rc = 0;
    pod_kernel_lock_mutex( dev->rq_run_mutex );

    if( rq->err != pod_rq_status_unprocessed )
        rc = ENOENT;

    // TODO checks to be on q internally?
    if( !rc )
        {
        rc = pod_q_dequeue( dev->default_r_q, rq );
        if( !rc ) rq->err = pod_rq_status_dequeued;
        }
    pod_kernel_unlock_mutex( dev->rq_run_mutex );
    return rc;
}



errno_t	pod_dev_q_fence( pod_device *dev )
{
    errno_t rc = 0;
    pod_kernel_lock_mutex( dev->rq_run_mutex );

    rc = pod_q_fence( dev->default_r_q );

    pod_kernel_unlock_mutex( dev->rq_run_mutex );
    return rc;
}



errno_t	pod_dev_q_raise( pod_device *dev, pod_request *rq, uint32_t io_prio )
{
    errno_t rc = 0;
    pod_kernel_lock_mutex( dev->rq_run_mutex );

    // Strictly speaking we have to check pod_q_is_on_q( pod_q *q, pod_request *rq );
    // Practically no one gives a shit if request is already done, so don't bother

    rq->io_prio = io_prio;
    // TODO skip sort if rq is done
    rc = pod_q_sort( dev->default_r_q, rq_prio_cmp ); 

    pod_kernel_unlock_mutex( dev->rq_run_mutex );
    return rc;
}




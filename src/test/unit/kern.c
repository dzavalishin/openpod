#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
// sleep
#include <unistd.h>

#include <pthread.h>


#include <openpod.h>
#include <pod_io_video.h>

// Bare bones implementation of OpenPOD kernel API


void req_done_func( struct pod_request *rq )
{
	fprintf( stderr, "rq done, status = %d\n", rq->err );

	struct pod_video_rq_mode *rq_arg = rq->op_arg;

	if( rq_arg == 0 )
	{
		fprintf( stderr, "rq arg == 0\n" );
	}
	else
	{
		if( !rq->err )
			fprintf( stderr, "x_size, y_size = %d*%d, format = %d\n", rq_arg->x_size, rq_arg->y_size, rq_arg->buf_fmt );
	}

	free(rq); // frees piggybacked arg too
}




static pthread_t req_thread = 0;

static void *req_thread_func( void *arg )
{
	struct pod_device *dev = arg;
	//struct pod_driver *drv = dev->drv;
	errno_t	rc;

	while(1)
	{
		sleep(1);

		pod_request *rq = calloc( 1, sizeof(pod_request) + sizeof(struct pod_video_rq_mode) );
		if( rq == 0 ) 
		{
			fprintf( stderr, "out of mem\n" );
			break;
		}

		struct pod_video_rq_mode *rq_arg = ((void*)rq) + sizeof(pod_request);

		rq->request_class	= POD_DEV_CLASS_VIDEO;
		rq->operation		= pod_video_getmode;
		rq->io_prio		= 0;
		rq->op_arg		= rq_arg;
		rq->done		= req_done_func;

		rc = pod_rq_enqueue( dev, rq );
		if( rc )
		{
			fprintf( stderr, "can't enqueue rq\n" );
			free( rq );
			break;
		}

	}

	return 0;
} 



errno_t		pod_dev_link( struct pod_driver *drv, struct pod_device *dev )	// Report a new available device to the OS kernel
{
	(void) drv;
	(void) dev;

	fprintf( stderr, "Device link: 0x%p\n", dev );

	if( req_thread == 0 )
		pthread_create( &req_thread, NULL, &req_thread_func, dev );


	// Empty - add dev to list? Do some requests?
	return 0;
}

errno_t		pod_dev_unlink( struct pod_driver *drv, struct pod_device *dev )
{
	(void) drv;
	(void) dev;

	fprintf( stderr, "Device unlink: 0x%p\n", dev );

	// Empty - remove dev from list? Stop requests?
	return 0;
}

errno_t		pod_dev_event( struct pod_driver *drv, struct pod_device *dev, int event_id, void *event_info )
{
	(void) drv;
	(void) dev;

	fprintf( stderr, "Device event: 0x%p, id %d\n", dev, event_id );

	switch( event_id )
	{
	case POD_EVENT_LOG:
		fprintf( stderr, "Device says: '%s'\n", (const char *)event_info );
		break;

	case POD_EVENT_STATE:
		fprintf( stderr, "Device state is: %x\n", *((int *)event_info) );
		break;

	default:
		return EINVAL;	// Unknown event type
	}

	return 0;
}
	


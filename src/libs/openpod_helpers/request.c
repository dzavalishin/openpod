#include <errno.h>

#include <openpod.h>

//-------------------------------------------------------------------
//
// Request methods
//
//-------------------------------------------------------------------



errno_t	pod_rq_enqueue( pod_device *dev, pod_request *rq )
{
	if( (dev == 0) || (dev->calls == 0) || (dev->calls->enqueue == 0 ) )
		return EFAULT;

	if( rq->request_class != dev->class_id ) 
	{
		rq->err = pod_rq_status_param;
		if( rq->done ) rq->done( rq );
		return 0;
	}

	if( !POD_DEV_STATE_CHECK( dev, POD_DEV_STATE_INIT ) )
		return ENXIO;

	if( !POD_DEV_STATE_CHECK( dev, POD_DEV_STATE_FOUND ) )
		return ENODEV;

	rq->err = pod_rq_status_unprocessed;

	return dev->calls->enqueue( dev, rq );
}


errno_t	pod_rq_dequeue( pod_device *dev, pod_request *rq )
{
	if( (dev == 0) || (dev->calls == 0) || (dev->calls->dequeue == 0 ) )
		return EFAULT;

	if( !POD_DEV_STATE_CHECK( dev, POD_DEV_STATE_INIT ) )
		return ENXIO;

	if( !POD_DEV_STATE_CHECK( dev, POD_DEV_STATE_FOUND ) )
		return ENODEV;

	return dev->calls->dequeue( dev, rq );
}

errno_t	pod_rq_fence( pod_device *dev, pod_request *rq )
{
	if( (dev == 0) || (dev->calls == 0) || (dev->calls->fence == 0 ) )
		return EFAULT;

	if( !POD_DEV_STATE_CHECK( dev, POD_DEV_STATE_INIT ) )
		return ENXIO;

	if( !POD_DEV_STATE_CHECK( dev, POD_DEV_STATE_FOUND ) )
		return ENODEV;

	return dev->calls->fence( dev, rq );
}

errno_t	pod_rq_raise( pod_device *dev, pod_request *rq, uint32_t io_prio )
{
	if( (dev == 0) || (dev->calls == 0) || (dev->calls->raise_prio == 0 ) )
		return EFAULT;

	if( !POD_DEV_STATE_CHECK( dev, POD_DEV_STATE_INIT ) )
		return ENXIO;

	if( !POD_DEV_STATE_CHECK( dev, POD_DEV_STATE_FOUND ) )
		return ENODEV;

	return dev->calls->raise_prio( dev, rq, io_prio );
}

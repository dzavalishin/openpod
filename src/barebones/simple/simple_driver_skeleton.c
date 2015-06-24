
#include "simple_driver_skeleton.h"


//-------------------------------------------------------------------
//
// Driver code - these parts have to be actually implemented in a
// real driver.
//
//-------------------------------------------------------------------


// This is where you detect hardware or, at least, check for existance

errno_t		simple_driver_sense( struct pod_driver *drv )
{
	// TODO detect hardware here

#if 0
	if( not_detected )
	{
		drv->private_data = 0;
		return ENOENT;
	}
#endif
	drv->private_data = &test_device;

	return 0;
}



// This is where you do actual IO.
// NB! No wait, request must be fullfilled right now just on call.
// If your hardware is slow, you need another driver skeleton, the
// one which implements request queue and uses interrupts.

static errno_t	simple_driver_enqueue( pod_device *dev, pod_request *rq )
{
	(void) dev;
	// Serve request - do it right now

	//printf( "rq->request_class = %d, dev->class_id = %d\n", rq->request_class, dev->class_id );

//	if( rq->request_class != dev->class_id ) goto einval;

	switch( rq->operation )
	{
	case pod_video_getmode:
		{
		struct pod_video_rq_mode *rq_arg = rq->op_arg;

		// TODO do actual non-blocking io here

		rq_arg->x_size = 1024;
		rq_arg->y_size = 768;
		rq_arg->buf_fmt = pod_pixel_rgba;
		rq_arg->vbuf = 0; // no direct access to video buffer
		}
		break;

	// TODO add and implement other class ops here

	default:
einval:
		rq->err = pod_rq_status_param;
		if( rq->done ) rq->done( rq );

		return 0;
	}

	rq->err = pod_rq_status_ok;
	if( rq->done ) rq->done( rq );

	return 0;
}
















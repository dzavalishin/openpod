
#include "simple_driver_skeleton.h"

static errno_t		simple_device_io_getmode( pod_device *dev, struct pod_video_rq_mode *m );

static void		(**class_interface)(struct pod_device *dev, void *arg) =
{
	0, // nop
	simple_device_io_getmode,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
};


//-------------------------------------------------------------------
//
// Driver code - these parts have to be actually implemented in a
// real driver.
//
//-------------------------------------------------------------------


// This is where you detect hardware or, at least, check for existance

errno_t		simple_driver_sense( struct pod_driver *drv )
{
	drv->private_data = &test_device;
	pod_device *dev = &test_device;

	dev->class_interface = class_interface;

	POD_DEV_STATE_SET( dev, POD_DEV_STATE_INIT );

#if 0
	// Your code to detect hardware here

	if( not_detected )
	{
		return ENOENT;
	}
#endif

	POD_DEV_STATE_SET( dev, POD_DEV_STATE_FOUND );

	// Device is not activated, but give kernel chance
	// to take a look so that kernel can decide whether
	// corresponding driver is needed. Especially of
	// use for video drivers.
	pod_dev_event( drv, dev, POD_EVENT_PREVIEW, 0 );

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

	errno_t rc = 0;

	switch( rq->operation )
	{
	case pod_video_getmode:
		{
		struct pod_video_rq_mode *rq_arg = rq->op_arg;
		rc = simple_device_io_getmode( dev, rq_arg );
		}
		break;

	// Implement other class ops here

	default:
		rq->err = pod_rq_status_param;
		if( rq->done ) rq->done( rq );
		return 0;
	}

	rq->err = rc ? pod_rq_status_ioerr : pod_rq_status_ok;
	if( rq->done ) rq->done( rq );

	return 0;
}




static errno_t		
simple_device_io_getmode( pod_device *dev, struct pod_video_rq_mode *m )
{
	// Your code to do actual non-blocking io here

	m->x_size = 1024;
	m->y_size = 768;
	m->buf_fmt = pod_pixel_rgba;
	m->vbuf = 0; // no direct access to video buffer

	return 0;
}










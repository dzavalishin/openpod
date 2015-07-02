
#include "simple_driver_skeleton.h"

static errno_t		simple_device_io_getmode( pod_device *dev, struct pod_video_rq_mode *m );
static errno_t		simple_device_io_setmode( pod_device *dev, struct pod_video_rq_mode *m );

static errno_t		(**class_interface)(struct pod_device *dev, void *arg) =
{
	0, // nop
	simple_device_io_getmode,
	simple_device_io_setmode,
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





static errno_t		
simple_device_io_getmode( pod_device *dev, struct pod_video_rq_mode *m )
{
	// Your code to do actual non-blocking io here - get current video mode

	m->x_size = 1024;
	m->y_size = 768;
	m->buf_fmt = pod_pixel_rgba;
	m->vbuf = 0; // no direct access to video buffer

	return 0;
}


static errno_t		
simple_device_io_setmode( pod_device *dev, struct pod_video_rq_mode *m )
{
	// Your code to do actual non-blocking io here - set new video mode

	return ENXIO;
}








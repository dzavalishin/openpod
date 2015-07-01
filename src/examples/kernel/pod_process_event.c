#include <openpod.h>
#include <pod_io_video.h>

#include <stdio.h>
#include <errno.h>

#include "pod_kernel_globals.h"


pod_driver *preselected_video_driver;
struct pod_video_rq_mode	preselected_video_mode;

//extern pod_device	*active_video_driver;


// ------------------------------------------------------------------
//
// Process events from driver/device
//
// ------------------------------------------------------------------


errno_t		
pod_dev_event( struct pod_driver *drv, struct pod_device *dev, int event_id, void *event_info )
{
	fprintf( stderr, "Driver '%s' device 0x%p event id %d\n", drv->name, dev, event_id );

	switch( event_id )
	{
	case POD_EVENT_LOG:
		fprintf( stderr, "Device says: '%s'\n", (const char *)event_info );
		break;

	case POD_EVENT_STATE:
		fprintf( stderr, "Device state is: %x\n", *((int *)event_info) );
		break;

	case POD_EVENT_PREVIEW:
		// Here we get preview of a non-started driver. If it's a video one,
		// find out the best possible video mode and choose driver to start.
		if( dev->class_id == POD_DEV_CLASS_VIDEO )
		{
			struct pod_video_rq_mode	m;
			if( pod_dev_method( dev, pod_video_getmode, &m ) ) // Error?
				break;

			if( 0 == preselected_video_driver ) // First one is better for sure :)
			{
better:
				preselected_video_driver = drv;
				preselected_video_mode = m;
				break;
			}

			if( cmp_video_mode( &preselected_video_mode, &m ) )
				goto better;
		}
		break;

	default:
		return EINVAL;	// Unknown event type
	}

	return 0;
}

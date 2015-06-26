#include <openpod.h>

// ------------------------------------------------------------------
//
// Link/unlink device.
//
// These functions, implemented in kernel, will be called by OpenPOD 
// drivers to presend found devices to kernel.
//
// ------------------------------------------------------------------

pod_device	*best_video_driver;


// Report a new available device to the OS kernel
errno_t		pod_dev_link( struct pod_driver *drv, struct pod_device *dev )	
{
	(void) drv;
	(void) dev;

	//fprintf( stderr, "Driver '%s' device link: 0x%p\n", drv->name, dev );

	switch( dev->class_id )
	{
	case POD_DEV_CLASS_VIDEO:
		if( best_video_driver == 0 ) { best_video_driver = dev; break; }

		// TODO get possible video mode, compare, choose best, make sure unchoosen video drivers get no activate		
		break;

	case POD_DEV_CLASS_NET:
		// TODO register a new network interface using given device
		break;

	case POD_DEV_CLASS_BLOCK:
		// TODO register a new disk using given device
		break;
	}

	return 0;
}

errno_t		pod_dev_unlink( struct pod_driver *drv, struct pod_device *dev )
{
	(void) drv;
	(void) dev;

	//fprintf( stderr, "Device unlink: 0x%p\n", dev );

	switch( dev->class_id )
	{
	case POD_DEV_CLASS_VIDEO:
		// Houston, we have a problem
		break;

	case POD_DEV_CLASS_NET:
		// TODO deregister corresponding network interface 
		// find_net_if_by_dev( dev ); ... stop and kill if...
		break;

	case POD_DEV_CLASS_BLOCK:
		// TODO do all we need when we loose disk 
		break;
	}

	// No point to return error, (sh)it happened already
	return 0;
}

#include <openpod.h>
#include <pod_kernel_api.h>


// Default implementation, supports just logging

// __attribute__((weak))

errno_t		
pod_dev_event( struct pod_driver *drv, struct pod_device *dev, int event_id, void *event_info ) 
{
	(void)dev;

	if( POD_EVENT_LOG != event_id ) return 0;

	// TODO dev name
	// TODO loglevel
	return pod_log_print( 0, "Driver '%s' device state: '%s'\n", drv->name, (const char*)event_info );
}

#include <openpod.h>
#include <pod_kernel_api.h>


// Default implementation, supports just logging, replace in a real kernel, if need something more

errno_t	__attribute__((weak))	
pod_dev_event( struct pod_driver *drv, struct pod_device *dev, int event_id, void *event_info ) 
{
	(void)dev;

	if( POD_EVENT_LOG != event_id ) return 0;

	// TODO dev name
	// TODO loglevel
	return pod_log_print( 0, "Driver '%s' device state: '%s'\n", drv->name, (const char*)event_info );
}

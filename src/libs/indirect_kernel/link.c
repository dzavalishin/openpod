#include <pod_kernel_ap.h>

#include <errno.h>

// I we have no corresponding functions available at link time,
// provide weak ones that call kernel through kernel_driver_api


errno_t __attribute__((weak))
pod_dev_link( struct pod_driver *drv, struct pod_device *dev )
{
	if( (0 == drv) || (0 == drv->kernel_driver_api) || (0 == drv->kernel_driver_api->pod_dev_link) ) return ENOSYS;

	return drv->kernel_driver_api->pod_dev_link( dev, dev );
}


errno_t	__attribute__((weak))	
pod_dev_unlink( struct pod_driver *drv, struct pod_device *dev )
{
	if( (0 == drv) || (0 == drv->kernel_driver_api) || (0 == drv->kernel_driver_api->pod_dev_unlink) ) return ENOSYS;

	return drv->kernel_driver_api->pod_dev_unlink( dev, dev );
}
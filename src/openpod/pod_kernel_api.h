

#include "pod_types.h"



errno_t		pod_dev_link( pod_driver *drv, pod_device *dev );	// Report a new available device to the OS kernel
errno_t		pod_dev_unlink( pod_driver *drv, pod_device *dev );	// Report device to be unavailable
errno_t		pod_dev_event( pod_driver *drv, pod_device *dev );	// Tell about device event (error? state change?)

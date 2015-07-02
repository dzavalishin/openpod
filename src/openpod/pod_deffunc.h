#include "openpod.h"

//*******************************************************************
//
// OpenPOD
//
// Default implementations of typical functions.
//
//*******************************************************************


// default pod_enqueue for device - calls syncronous interface.
errno_t	pod_default_enqueue( pod_device *dev, pod_request *rq );

// Empty, does nothing, returns failure. Can be used if above pod_default_enqueue used.
errno_t	pod_default_dequeue( pod_device *dev, pod_request *rq );


// Empty, does nothing, returns success. Can be used if above pod_default_enqueue used.
errno_t	pod_default_fence( pod_device *dev, pod_request *rq );

// Empty, does nothing, returns success. Can be used if above pod_default_enqueue used.
errno_t	pod_default_raise( pod_device *dev, pod_request *rq, uint32_t io_prio );

// Combine them to simplify rq-less drivers even more
extern static pod_dev_f pod_default_dev_func;


errno_t		pod_default_construct( struct pod_driver *drv );
errno_t		pod_default_destruct( struct pod_driver *drv );

// Can be used for single dev driver:
//     drv->private_data = dev

errno_t		single_dev_driver_activate( struct pod_driver *drv );
errno_t		single_dev_driver_deactivate( struct pod_driver *drv );


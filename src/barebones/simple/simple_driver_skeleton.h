#ifndef SKELETON_H
#define SKELETON_H

#include <openpod.h>
//#include <pod_device.h>
#include <pod_io_video.h>

//-------------------------------------------------------------------
//
// Pseudo-driver definitions
//
//-------------------------------------------------------------------

errno_t	simple_driver_enqueue( pod_device *dev, pod_request *rq ); 
errno_t	simple_driver_dequeue( pod_device *dev, pod_request *rq );
errno_t	simple_driver_fence( pod_device *dev, pod_request *rq );
errno_t	simple_driver_raise( pod_device *dev, pod_request *rq, uint32_t io_prio );


errno_t	simple_driver_construct( struct pod_driver *drv );
errno_t	simple_driver_destruct( struct pod_driver *drv );


errno_t	simple_driver_activate( struct pod_driver *drv );
errno_t	simple_driver_deactivate( struct pod_driver *drv );


errno_t	simple_driver_sense( struct pod_driver *drv );






#endif // SKELETON_H

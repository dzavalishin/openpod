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


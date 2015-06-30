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

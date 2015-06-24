
#include "simple_driver_skeleton.h"


//-------------------------------------------------------------------
//
// Driver code - non-implemented functions 
//
//-------------------------------------------------------------------





static errno_t	simple_driver_dequeue( pod_device *dev, pod_request *rq )
{
	(void) dev;
	(void) rq;

	return ENOENT; // Too late, rq is done already (as we do anything syncronously)
}

static errno_t	simple_driver_fence( pod_device *dev, pod_request *rq )
{
	(void) dev;
	(void) rq;

	return 0; // We have no q, so all requests are done in order
}

static errno_t	simple_driver_raise( pod_device *dev, pod_request *rq, uint32_t io_prio )
{
	(void) dev;
	(void) rq;
	(void) io_prio;

	return 0; // It's too late anyway, don't bother caller with error code.
}






errno_t		simple_driver_construct( struct pod_driver *drv )
{
	(void) drv;
	// Empty
	return 0;
}

errno_t		simple_driver_destruct( struct pod_driver *drv )
{
	(void) drv;
	// Empty
	return 0;
}



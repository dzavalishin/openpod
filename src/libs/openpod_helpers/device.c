#include <errno.h>

#include <openpod.h>

//-------------------------------------------------------------------
//
// Device methods
//
//-------------------------------------------------------------------


// Call dev (class/op specific) function from class_interface. This is 
// an interface identical to what is available via requests, but syncronous.

errno_t pod_dev_method( pod_device *dev, int op_id, void *param )
{
	if( dev == 0 ) return ENODEV;

	if( (op_id < 0) || (dev->class_interface == 0) ) return ENOSYS;

	errno_t (*class_func)(pod_device *dev, void *arg); // dev class specific interface

	// TODO check op id > max possible

	class_func = dev->class_interface[op_id];

	if( class_func == 0 ) return ENOSYS;

	return class_func( dev, param );
}


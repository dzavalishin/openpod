
#include "simple_driver_skeleton.h"



static pod_dev_f dev_func = 
{
	//simple_driver_enqueue,
	pod_default_enqueue,

	//simple_driver_dequeue,
	pod_default_dequeue,

	//simple_driver_fence,
	pod_default_fence,

	//simple_driver_raise,
	pod_default_raise
};


pod_driver simple_driver = {
	POD_DRIVER_MAGIC,
	POD_DRIVER_VERSION,
	POD_DRIVER_ARCH_UNKNOWN,

	POD_DEV_CLASS_VIDEO, 0, 0, 0,

	"simple driver skeleton - RENAME ME!",

	{
		simple_driver_construct,
		simple_driver_destruct,


		simple_driver_activate,
		simple_driver_deactivate,


		simple_driver_sense
	},

	0, // no optional entry points

	0, // no properties

	0, // Private state does not exist yet

	0, // Statically linked, no need for kernel entry points struct

};



pod_device simple_device =
{
	POD_DEVICE_MAGIC, // magic

	POD_DEV_CLASS_VIDEO, 
	0, 0,
	0, 	// flags

	&simple_driver,

	&dev_func, 	// dev io entry points

	0, 	// no properties

	0, 	// dev class specific interface
	0, 	// private data

	0,	// default request q
	0,	// request we do now
	0,	// thread used to run requests
	0,	// triggered to run next request

};




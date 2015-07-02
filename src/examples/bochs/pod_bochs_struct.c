
#include "pod_bochs.h"

#include <pod_deffunc.h>


pod_driver pod_bochs_driver = {
    POD_DRIVER_MAGIC,
    POD_DRIVER_VERSION,
    POD_DRIVER_ARCH_UNKNOWN,

    POD_DEV_CLASS_VIDEO, 0, 0, 0,

    "Bochs video",

    {
        pod_default_construct,          // Empty
        pod_default_destruct,           // Empty

        pod_bochs_activate,
        pod_bochs_deactivate,

        pod_bochs_sense
    },

    0, // no optional entry points
    0, // no properties
    0, // Private state does not exist yet
    0, // Statically linked, no need for kernel entry points struct

};



pod_device pod_bochs_device =
{
    POD_DEVICE_MAGIC, // magic
    POD_DEV_CLASS_VIDEO,
    0, 0, 0,    // flags

    "Bochs video",
    &pod_bochs_driver,
    &pod_default_dev_func,      // dev io entry points

    0,  // no properties

    0,  // dev class specific interface
    0,  // private data

    0,  // default request q
    0,  // request we do now
    0,  // thread used to run requests
    0,  // triggered to run next request

};




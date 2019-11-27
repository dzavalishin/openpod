========================
Simplest driver skeleton
========================

lets review a simplest driver example provided in project repository.

Please see ``barebones/simple`` directory for this example's source code.

-----------------
Driver descriptor
-----------------


First we need to provide driver descriptor structure.

.. code:: c

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

The only notable things here are:

* ``POD_DEV_CLASS_VIDEO`` - we declare that this is a video driver. OS might decide to start different 
  drivers sooner or later, or to group drivers start by classes.

* ``simple_driver_X`` - entry points to start and stop our driver by OS.


-----------------
Device descriptor
-----------------

Our driver will provide just one and only device, so device descriptor
can be static too.

.. code:: c

    pod_device simple_device =
    {
        POD_DEVICE_MAGIC, // magic

        POD_DEV_CLASS_VIDEO, 
        0, 0,
        0, 	// flags

        // Device name, either static or runtime-generated
        "simple driver skeleton device name - RENAME ME!",

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

Again, we note that dev class is ``POD_DEV_CLASS_VIDEO``. It is critical to be 
correct - OS will connect device to its users based on this.

We provide device entry points for async interface, but all of them are default
ones provided by OpenPOD library.

.. code:: c

    static pod_dev_f dev_func = 
    {
        pod_default_enqueue,
        pod_default_dequeue,
        pod_default_fence,
        pod_default_raise
    };

First of them, ``pod_default_enqueue``, will just call syncronous entry point of driver,
three other are no-ops.



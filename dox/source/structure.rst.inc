OpenPOD driver includes:

* **Driver descriptor.** A structure that defines global (not device specific) properties an
  entry points. 
  
  Driver descriptor is a required part of a driver.

* **Device descriptor.** A structure which describes each device exported by driver. Can be single and
static, or can be dynamically allocated in run time. For example, USB disk driver can create device
descriptors dynamically as devices come and go.

Device descriptor is not a required part - there can be driver which does not export devices at all
or just starts with empty set of devices. But most drivers will provide at least one device at start.

* **Driver and/or device properties.** Set of key=value parameters to control driver or device and 
  request meta information. For example, sound driver can set sampling rate with corresponding
  property.

  Properties are optional part of specification.


=======================
Driver and device class
=======================

Device class specifies API this device exports to the kernel.

.. code:: c

    #define POD_DEV_CLASS_VOID 0        // No API.
    #define POD_DEV_CLASS_SPECIAL 1     // Has user defined nonstandard interface
    #define POD_DEV_CLASS_VIDEO 2       // Framebuf, bitblt io
    #define POD_DEV_CLASS_BLOCK 3       // Disk, cdrom: block io
    #define POD_DEV_CLASS_CHARACTER 4   // Tty, serial, byte io
    #define POD_DEV_CLASS_NET 5         // Packet IO, has MAC address
    #define POD_DEV_CLASS_KEYBD 6       // Key (make/break) events
    #define POD_DEV_CLASS_MOUSE 7       // Mouse coordinate events
    #define POD_DEV_CLASS_MULTIPLE 0xFF // Driver only, has multiple dev types

.. note:: Sound driver class is missing.

  Sound device is, basically a character (byte stream) device, but separate class
  can help kernel to classify and connect driver in a special way.

=================
Driver descriptor
=================



--------------
Driver methods
--------------


**pod_construct**
  Driver initialization. Driver must allocate structures and do any setup required 
  but do not scan hardware or attempt to export devices. This stage can be empty.

.. code:: c

  errno_t pod_construct( pod_driver *drv ); // ENOMEM

**pod_destruct**
  Complete driver destruction. Driver must free all resources, stop threads,
  release OS objects (such as mutexes and conds). Driver can be unloaded after
  this call.

  It is assumed that ``deactivate`` was called before this call. Nevertheless
  if no ``deactivate`` was called driver must attempt to do its best to deactivate
  self before desctruction. If it is not possible, this call can fail.

.. code:: c

  errno_t pod_destruct( pod_driver *drv );

**pod_activate**

  Start driver. Prepare and activate hardware. Inform kernel
  about all devices this driver exports. See ``pod_dev_link``.
  
  This call is done before any IO is attempted by kernel.
  Until this call driver must refuse to do any IO.

  Hardware scan must be done before this call.

.. code:: c

  errno_t pod_activate( pod_driver *drv );

**pod_deactivate**

  Stop driver. Reset and stop hardware. Revoke all
  exported devices. See ``pod_dev_unlink``.

.. code:: c

  errno_t pod_deactivate( pod_driver *drv );

**pod_sense**

  Look for hardware to exist and be operational. Scan for
  devices that can be operated by this driver.

  Devices must be found but not exported to kernel or started.

  It is ok to call kernel entry points to scan hardware, such as
  PCI enumeration functions.

.. code:: c

  errno_t pod_sense( pod_driver *drv );

**pod_probe**

  This is a call from kernel with an offer of a device to handle.

  Driver must check if this device can be handled and accept or refuse
  to handle it. As with ``pod_sense`` accepted device should not be
  started or exported to kernel during processing of this call.

  This entry point is not finally defined. Request for comments.

.. code:: c

  errno_t pod_probe( pod_driver *drv, bus?, dev? ); // ENOMEM, EFAULT


-----------------
Driver life cycle
-----------------

Defines driver initialization, start, search for hardware, stop and resource release stages.

Basic life cycle:

* Kernel calls ``pod_construct``, driver allocates data strutures and resources (mutexes, threads, etc).

* Kernel calls ``pod_sense`` and/or ``pod_probe``, driver looks for hardware and decides on list of
  devices that exist and can be served.

* Kernel calls ``pod_activate``, driver exports to kernel known devices with calls to ``pod_dev_link``.

* Driver serves IO requests. If devices come or go driver calls ``pod_dev_link`` / ``pod_dev_unlink`` to
  inform kernel.

* Kernel calls ``pod_deactivate``, driver deregisters all devices.

* Kernel calls ``pod_destruct``, driver releases all resources.

* Kernel can unload driver at this point.


=================
Device descriptor
=================

.. code:: c

    struct pod_device {
        uint32_t        magic;
        uint8_t         class;

        uint8_t         pad0;
        uint8_t         pad1;

        uint8_t         flags;

        pod_driver      *drv;
        pod_dev_f       *calls;
        pod_properties  *prop;
        void            *class_interface;
        void            *private_data;
        
        // Fields below are used by default framework code

        // Request queue, used by pod_dev_q_ functions
        pod_q           *default_r_q;   // default request q
        pod_request     *curr_rq;       // request we do now
        pod_thread      *rq_run_thread; // thread used to run requests
        pod_cond        *rq_run_cond;   // triggered to run next request
    };



--------------
Device methods
--------------

**pod_dev_stop** 

  Stop device. Called after all IO is done and all possible users disconnected.
  In Unix like OS this entry point is called after device was closed by least
  process. No IO can be done after stop.

**pod_dev_start**

  Start device. Called before first IO attempt on device.

  In Unix like OS corresponds to open system call for this device.

.. code:: c

  errno_t pod_dev_stop( pod_device *dev );
  errno_t pod_dev_start( pod_device *dev );

**pod_rq_enqueue**

  Start asyncronous IO on device.

**pod_rq_dequeue**

  Revoke IO request previously enqueued by ``pod_rq_enqueue``, if 
  possible. Can fail if IO is already in progress. 
  
  This entry point is optional.

**pod_rq_fence**

  Make sure that all IO started before this call is finished. 
  Can return instantly or wait for all previous IO is complete.
  In any case must guarantee that on call to request callback 
  for this request all previously enqueued requests are complete
  and callbacks for them are finished too.

**pod_rq_raise**

  Change (usually - rise:) request priority.

  Optional, but for disk IO it is really good thing to have.

.. code:: c

    errno_t pod_rq_enqueue( pod_device *dev, pod_request *rq );
    errno_t pod_rq_dequeue( pod_device *dev, pod_request *rq );
    errno_t pod_rq_fence( pod_device *dev, pod_request *rq );
    errno_t pod_rq_raise( pod_device *dev, pod_request *rq, uint32_t io_prio );

-----------------
Device life cycle
-----------------


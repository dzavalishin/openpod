
#include <openpod.h>
//#include <pod_device.h>
#include <pod_io_video.h>

//-------------------------------------------------------------------
//
// Pseudo-driver definitions
//
//-------------------------------------------------------------------

static errno_t	simple_driver_enqueue( pod_device *dev, pod_request *rq ); 
static errno_t	simple_driver_dequeue( pod_device *dev, pod_request *rq );
static errno_t	simple_driver_fence( pod_device *dev, pod_request *rq );
static errno_t	simple_driver_raise( pod_device *dev, pod_request *rq, uint32_t io_prio );


errno_t		simple_driver_construct( struct pod_driver *drv );
errno_t		simple_driver_destruct( struct pod_driver *drv );


errno_t		simple_driver_activate( struct pod_driver *drv );
errno_t		simple_driver_deactivate( struct pod_driver *drv );


errno_t		simple_driver_sense( struct pod_driver *drv );


static pod_dev_f dev_func = 
{
	simple_driver_enqueue,
	simple_driver_dequeue,
	simple_driver_fence,
	simple_driver_raise,
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




//-------------------------------------------------------------------
//
// Driver code
//
//-------------------------------------------------------------------



static errno_t	simple_driver_enqueue( pod_device *dev, pod_request *rq )
{
	(void) dev;
	// Serve request - do it right now

	//printf( "rq->request_class = %d, dev->class_id = %d\n", rq->request_class, dev->class_id );

	if( rq->request_class != dev->class_id ) goto einval;

	switch( rq->operation )
	{
	case pod_video_getmode:
		{
		struct pod_video_rq_mode *rq_arg = rq->op_arg;

		rq_arg->x_size = 1024;
		rq_arg->y_size = 768;
		rq_arg->buf_fmt = pod_pixel_rgba;
		rq_arg->vbuf = 0; // no direct access to video buffer
		}
		break;

	default:
einval:
		rq->err = pod_rq_status_param;
		rq->done( rq );

		return 0;
	}

	rq->err = pod_rq_status_ok;
	rq->done( rq );

	return 0;
}


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


errno_t		simple_driver_activate( struct pod_driver *drv )
{
	// Register dev

	errno_t	rc;
	struct pod_device *dev	= drv->private_data; // we are very simple

	if( dev == 0 ) return ENOENT;

	// TODO update flag in device struct?

	rc = pod_dev_link( drv, dev );
	if( rc ) return rc;

	rc = pod_dev_event( drv, dev, POD_EVENT_LOG, "device activated" );
	if( rc ) return rc;

	return 0;
}

errno_t		simple_driver_deactivate( struct pod_driver *drv )
{
	// De-register dev

	errno_t	rc;
	struct pod_device *dev	= drv->private_data; // we are very simple

	if( dev == 0 ) return ENOENT;

	rc = pod_dev_event( drv, dev, POD_EVENT_LOG, "device deactivated" );
	if( rc ) return rc;

	// TODO update flag in device struct?


	rc = pod_dev_unlink( drv, dev );
	if( rc ) return rc;


	return 0;
}


errno_t		simple_driver_sense( struct pod_driver *drv )
{
	// 'find' hardware

	drv->private_data = &test_device;

	return 0;
}





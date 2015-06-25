#include <stdio.h>
#include <stdlib.h>
// sleep
#include <unistd.h>

#include <CUnit/Basic.h>

#include "utils.h"

#include <openpod.h>
//#include <pod_device.h>
#include <pod_io_video.h>

//-------------------------------------------------------------------
//
// Pseudo-driver definitions
//
//-------------------------------------------------------------------

static errno_t	test_driver_enqueue( pod_device *dev, pod_request *rq ); 
static errno_t	test_driver_dequeue( pod_device *dev, pod_request *rq );
static errno_t	test_driver_fence( pod_device *dev, pod_request *rq );
static errno_t	test_driver_raise( pod_device *dev, pod_request *rq, uint32_t io_prio );


errno_t		test_driver_construct( struct pod_driver *drv );
errno_t		test_driver_destruct( struct pod_driver *drv );


errno_t		test_driver_activate( struct pod_driver *drv );
errno_t		test_driver_deactivate( struct pod_driver *drv );


errno_t		test_driver_sense( struct pod_driver *drv );


static pod_dev_f dev_func = 
{
	test_driver_enqueue,
	test_driver_dequeue,
	test_driver_fence,
	test_driver_raise,
};


pod_driver test_driver = {
	POD_DRIVER_MAGIC,
	POD_DRIVER_VERSION,
	POD_DRIVER_ARCH_UNKNOWN,

	POD_DEV_CLASS_VIDEO, 0, 0, 0,

	"test suit driver mockup",

	{
		test_driver_construct,
		test_driver_destruct,


		test_driver_activate,
		test_driver_deactivate,


		test_driver_sense
	},

	0, // no optional entry points

	0, // no properties

	0, // Private state does not exist yet

	0, // Statically linked, no need for kernel entry points struct

};



pod_device test_device =
{
	0, // TODO magic

	POD_DEV_CLASS_VIDEO, 
	0, 0,
	0, 	// flags

	&test_driver,

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
// Test suit code
//
//-------------------------------------------------------------------


TEST_FUNCT(start_driver) 
{
    printf("constructing driver, ");

    //CU_ASSERT_EQUAL(0, test_driver.calls.pod_construct( &test_driver ) );
    CU_ASSERT_EQUAL(0, pod_construct( &test_driver ) );

    printf("HW scan, ");

    //CU_ASSERT_EQUAL(0, test_driver.calls.pod_sense( &test_driver ) );
    CU_ASSERT_EQUAL(0, pod_sense( &test_driver ) );

    printf("activating driver, ");

    //CU_ASSERT_EQUAL(0, test_driver.calls.pod_activate( &test_driver ) );
    CU_ASSERT_EQUAL(0, pod_activate( &test_driver ) );
    
}


TEST_FUNCT(run_driver) 
{
    printf("running driver\n");
    sleep(4);
}


TEST_FUNCT(stop_driver) 
{

    printf("deactivating driver, ");

    //CU_ASSERT_EQUAL(0, test_driver.calls.pod_deactivate( &test_driver ) );
    CU_ASSERT_EQUAL(0, pod_deactivate( &test_driver ) );

    printf("destructing driver, ");

    //CU_ASSERT_EQUAL(0, test_driver.calls.pod_destruct( &test_driver ) );
    CU_ASSERT_EQUAL(0, pod_destruct( &test_driver ) );
}

void runSuite(void) {
    /* Код тест-сьюта */

    //printf("test suite\n");

    CU_pSuite suite = CUnitCreateSuite("Driver lifecycle sequence");
    if (suite) {
        ADD_SUITE_TEST(suite, start_driver)
        ADD_SUITE_TEST(suite, run_driver)
        ADD_SUITE_TEST(suite, stop_driver)
    }
}







//-------------------------------------------------------------------
//
// Pseudo-driver code
//
//-------------------------------------------------------------------



static errno_t	test_driver_enqueue( pod_device *dev, pod_request *rq )
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
		goto done;
	}

	rq->err = pod_rq_status_ok;
done:
	if( rq->done ) rq->done( rq );
	return 0;
}


static errno_t	test_driver_dequeue( pod_device *dev, pod_request *rq )
{
	(void) dev;
	(void) rq;

	return ENOENT; // Too late, rq is done already (as we do anything syncronously)
}

static errno_t	test_driver_fence( pod_device *dev, pod_request *rq )
{
	(void) dev;
	(void) rq;

	return 0; // We have no q, so all requests are done in order
}

static errno_t	test_driver_raise( pod_device *dev, pod_request *rq, uint32_t io_prio )
{
	(void) dev;
	(void) rq;
	(void) io_prio;

	return 0; // It's too late anyway, don't bother caller with error code.
}






errno_t		test_driver_construct( struct pod_driver *drv )
{
	(void) drv;
	// Empty
	return 0;
}

errno_t		test_driver_destruct( struct pod_driver *drv )
{
	(void) drv;
	// Empty
	return 0;
}


errno_t		test_driver_activate( struct pod_driver *drv )
{
	// Register dev

	errno_t	rc;
	struct pod_device *dev	= drv->private_data; // we are very simple

	if( dev == 0 ) return ENOENT;

	POD_DEV_STATE_CLEAR( dev, POD_DEV_STATE_PAUSE );
	POD_DEV_STATE_SET( dev, POD_DEV_STATE_LINKED );

	rc = pod_dev_link( drv, dev );
	if( rc ) return rc;

	rc = pod_dev_event( drv, dev, POD_EVENT_LOG, "device activated" );
	if( rc ) return rc;

	return 0;
}

errno_t		test_driver_deactivate( struct pod_driver *drv )
{
	// De-register dev

	errno_t	rc;
	struct pod_device *dev	= drv->private_data; // we are very simple

	if( dev == 0 ) return ENOENT;


	rc = pod_dev_event( drv, dev, POD_EVENT_LOG, "device deactivated" );
	if( rc ) return rc;

	rc = pod_dev_unlink( drv, dev );
	if( rc ) return rc;

	//POD_DEV_STATE_SET( dev, POD_DEV_STATE_PAUSE );
	POD_DEV_STATE_CLEAR( dev, POD_DEV_STATE_LINKED );

	return 0;
}


errno_t		test_driver_sense( struct pod_driver *drv )
{

	drv->private_data = &test_device;

	POD_DEV_STATE_SET( dev, POD_DEV_STATE_INIT );

	// 'find' hardware

	POD_DEV_STATE_SET( dev, POD_DEV_STATE_FOUND );

	return 0;
}





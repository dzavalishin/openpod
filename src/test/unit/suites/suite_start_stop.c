#include <stdio.h>
#include <stdlib.h>
// sleep
#include <unistd.h>

#include <CUnit/Basic.h>

#include "utils.h"

#include <openpod.h>
#include <pod_deffunc.h>
#include <pod_io_video.h>
#include <pod_properties.h>

//-------------------------------------------------------------------
//
// Pseudo-driver definitions
//
//-------------------------------------------------------------------

static errno_t	test_driver_enqueue( pod_device *dev, pod_request *rq ); 
static errno_t	test_driver_dequeue( pod_device *dev, pod_request *rq );
static errno_t	test_driver_fence( pod_device *dev );
static errno_t	test_driver_raise( pod_device *dev, pod_request *rq, uint32_t io_prio );


errno_t		test_driver_construct( struct pod_driver *drv );
errno_t		test_driver_destruct( struct pod_driver *drv );


errno_t		test_driver_activate( struct pod_driver *drv );
errno_t		test_driver_deactivate( struct pod_driver *drv );


errno_t		test_driver_sense( struct pod_driver *drv );



static errno_t		test_device_io_getmode( pod_device *dev, void *arg );

static errno_t		(*test_driver_class_interface[])(struct pod_device *dev, void *arg) =
{
	0, // nop
	test_device_io_getmode,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
};


static int 	property_blit_queue_timeout = 100;
static char *	property_display_device_name = 0;
static errno_t  ro_getf(struct pod_properties *ps, void *context, size_t offset, void *vp, char *val, size_t len); // { strlcpy( val, "getf_value", len ); return 0; }
static errno_t  ro_convertf(struct pod_properties *ps, void *context, size_t offset, void *vp, char *val, size_t len); // { strlcpy( val, "getf_value", len ); return 0; }

static int 	wo_property = 0;
static int 	wo_activate_cout = 0;
static errno_t  wo_setf(struct pod_properties *ps, void *context, size_t offset, void *vp, const char *val);
static errno_t  wo_activate(struct pod_properties *ps, void *context, size_t offset, void *vp );

static int 	valp_properties[2] = { 1, 2 };
//! Get pointer to property value
static void *	valp(struct pod_properties *ps, void *context, size_t offset );

static pod_property plist[] =
{
	{	pt_int32,	"blit_queue_timeout",	0, &property_blit_queue_timeout, 0, 0, 0, 0 },
	{	pt_mstring,	"display_device_name",	0, &property_display_device_name, 0, 0, 0, 0 },
        {	pt_mstring,	"ro_func",		0, 0, 0, 0, 0, ro_getf },
        {	pt_mstring,	"ro_func_conv",		0, &property_blit_queue_timeout, 0, 0, 0, ro_convertf },
        {	pt_mstring,	"wo_func",		0, &wo_property, 0, wo_activate, wo_setf, 0 },
        {	pt_int32,	"valp_1",		1, 0, 0, 0, 0, 0 },
        {	pt_int32,	"valp_2",		2, 0, 0, 0, 0, 0 },
};

static pod_properties props =
{
	plist,
        sizeof(plist)/sizeof(pod_property),
        valp
};


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

	&props, // no properties

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
        {{0}},	// thread used to run requests
	0,	// triggered to run next request
	0,	// mutex
	0,	// rq start func

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


// Doesnt link, skip for now. Separate ADA test suite?
# if 0
void openpod__ada_print_drv( pod_driver *drv );

TEST_FUNCT(ada_driver) 
{
    printf("printing driver struct from ADA\n");
    openpod__ada_print_drv( &test_driver );
}
#endif


TEST_FUNCT(use_driver_properties)
{
	//errno_t	rc;
        char buf[128];
        // TODO test pod_gen_listproperties
        // TODO test combined valp()/get/set/activate

	// Int

        CU_ASSERT_EQUAL(0,  pod_drv_getproperty( &test_driver, "blit_queue_timeout", buf, sizeof(buf) ) );
        CU_ASSERT_STRING_EQUAL( buf, "100" );

        CU_ASSERT_EQUAL(0,  pod_drv_setproperty( &test_driver, "blit_queue_timeout", "200" ) );

        CU_ASSERT_EQUAL(0,  pod_drv_getproperty( &test_driver, "blit_queue_timeout", buf, sizeof(buf) ) );
        CU_ASSERT_STRING_EQUAL( buf, "200" );

        // String

        CU_ASSERT_EQUAL(0,  pod_drv_setproperty( &test_driver, "display_device_name", "Elbrus 4K" ) );

        CU_ASSERT_EQUAL(0,  pod_drv_getproperty( &test_driver, "display_device_name", buf, sizeof(buf) ) );
        CU_ASSERT_STRING_EQUAL( buf, "Elbrus 4K" );

        //printf( "property_display_device_name = '%s'\n", property_display_device_name );
        //printf( "display_device_name = '%s'\n", buf );

        // Readonly, through property value get func

        CU_ASSERT_EQUAL(0,  pod_drv_getproperty( &test_driver, "ro_func", buf, sizeof(buf) ) );
        CU_ASSERT_STRING_EQUAL( buf, "getf_value" );

        CU_ASSERT_EQUAL(0,  pod_drv_getproperty( &test_driver, "ro_func_conv", buf, sizeof(buf) ) );
        CU_ASSERT_STRING_EQUAL( buf, "ro_convertf='200'" );

        // Write only, set func, activate func

        CU_ASSERT_EQUAL(0, wo_property );
        CU_ASSERT_EQUAL(0, wo_activate_cout );

        CU_ASSERT_EQUAL(EINVAL, pod_drv_setproperty( &test_driver, "wo_func", "Elbrus 4K" ) );

        CU_ASSERT_EQUAL(0, wo_property );
        CU_ASSERT_EQUAL(0, wo_activate_cout );

        CU_ASSERT_EQUAL(0,  	pod_drv_setproperty( &test_driver, "wo_func", "33" ) );

        CU_ASSERT_EQUAL(33, wo_property );
        CU_ASSERT_EQUAL(1, wo_activate_cout );
        

        // Using ofset and valp func

        CU_ASSERT_EQUAL(0,  pod_drv_getproperty( &test_driver, "valp_1", buf, sizeof(buf) ) );
        CU_ASSERT_STRING_EQUAL( buf, "1" );
        //printf( "valp_1 = '%s'\n", buf );

        CU_ASSERT_EQUAL(0,  pod_drv_getproperty( &test_driver, "valp_2", buf, sizeof(buf) ) );
        CU_ASSERT_STRING_EQUAL( buf, "2" );
        //printf( "valp_2 = '%s'\n", buf );
        
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
//        ADD_SUITE_TEST(suite, ada_driver)
        ADD_SUITE_TEST(suite, use_driver_properties)
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

	//if( rq->request_class != dev->class_id ) goto einval;

#if 1
	return pod_default_enqueue( dev, rq );
#else
	switch( rq->operation )
	{
	case pod_video_getmode:
		{
		struct pod_video_rq_mode *rq_arg = rq->op_arg;

		// TODO
		//rc = simple_device_io_getmode( dev, rq->op_arg );

		rq_arg->x_size = 1024;
		rq_arg->y_size = 768;
		rq_arg->buf_fmt = pod_pixel_rgba;
		rq_arg->vbuf = 0; // no direct access to video buffer
		}
		break;

	default:
//einval:
		rq->err = pod_rq_status_param;
		goto done;
	}

	rq->err = pod_rq_status_ok;
done:
	if( rq->done ) rq->done( rq );
	return 0;
#endif
}


static errno_t	test_driver_dequeue( pod_device *dev, pod_request *rq )
{
	(void) dev;
	(void) rq;

	return ENOENT; // Too late, rq is done already (as we do anything syncronously)
}

static errno_t	test_driver_fence( pod_device *dev )
{
	(void) dev;
//	(void) rq;

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
	struct pod_device *dev	= drv->private_data; // we are very simple
	dev->class_interface = test_driver_class_interface;

	POD_DEV_STATE_SET( dev, POD_DEV_STATE_INIT );

	// 'find' hardware

	POD_DEV_STATE_SET( dev, POD_DEV_STATE_FOUND );

	//rc = 
	pod_dev_event( drv, dev, POD_EVENT_PREVIEW, 0 );
	//if( rc ) return rc;

	return 0;
}



static errno_t		
test_device_io_getmode( pod_device *dev, void *arg )
{
	struct pod_video_rq_mode *m = arg;
	(void) dev;
	// Your code to do actual non-blocking io here

	m->x_size = 1024;
	m->y_size = 768;
	m->buf_fmt = pod_pixel_rgba;
	m->vbuf = 0; // no direct access to video buffer

	return 0;
}


// Driver properties

static errno_t
ro_getf(struct pod_properties *ps, void *context, size_t offset, void *vp, char *val, size_t len)
{
    (void) ps;
    (void) vp;
    (void) context;
    (void) offset;

    strlcpy( val, "getf_value", len ); return 0;
}


static errno_t  ro_convertf(struct pod_properties *ps, void *context, size_t offset, void *vp, char *val, size_t len)
{
    (void) ps;
//    (void) vp;
    (void) context;
    (void) offset;

    snprintf( val, len, "ro_convertf='%d'", *((int*)vp) );
    return 0;
}


static errno_t  wo_setf(struct pod_properties *ps, void *context, size_t offset, void *vp, const char *val)
{
    (void) ps;
    (void) context;
    (void) offset;

    u_int32_t *ip = vp;

    if( 1 != sscanf( val, "%d", ip ) )
        return EINVAL;

    return 0;
}

static errno_t  wo_activate(struct pod_properties *ps, void *context, size_t offset, void *vp )
{
    (void) ps;
    (void) vp;
    (void) context;
    (void) offset;

    wo_activate_cout++;

    return 0;
}

static void *
valp(struct pod_properties *ps, void *context, size_t offset )
{
    (void) ps;
    (void) context;

    switch(offset)
    {
    case 1:
    case 2:
        return valp_properties+offset-1;
    }

    return 0;
}





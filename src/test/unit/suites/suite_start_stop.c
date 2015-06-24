#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>

#include "utils.h"

#include <openpod.h>
//#include <pod_device.h>


//-------------------------------------------------------------------
//
// Pseudo-driver definitions
//
//-------------------------------------------------------------------

errno_t		test_driver_construct( struct pod_driver *drv );
errno_t		test_driver_destruct( struct pod_driver *drv );


errno_t		test_driver_activate( struct pod_driver *drv );
errno_t		test_driver_deactivate( struct pod_driver *drv );


errno_t		test_driver_sense( struct pod_driver *drv );



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

	0, 	// dev io entry points

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


TEST_FUNCT(start_driver) {
    printf("constructing driver, ");

    CU_ASSERT_EQUAL(0, test_driver.calls.pod_construct( &test_driver ) );

    printf("HW scan, ");

    CU_ASSERT_EQUAL(0, test_driver.calls.pod_sense( &test_driver ) );

    printf("activating driver, ");

    CU_ASSERT_EQUAL(0, test_driver.calls.pod_activate( &test_driver ) );

    
}
TEST_FUNCT(stop_driver) {


    printf("deactivating driver, ");

    CU_ASSERT_EQUAL(0, test_driver.calls.pod_deactivate( &test_driver ) );

    printf("destructing driver, ");

    CU_ASSERT_EQUAL(0, test_driver.calls.pod_destruct( &test_driver ) );
}

void runSuite(void) {
    /* Код тест-сьюта */

    //printf("test suite\n");

    CU_pSuite suite = CUnitCreateSuite("Driver lifecycle sequence");
    if (suite) {
        ADD_SUITE_TEST(suite, start_driver)
        ADD_SUITE_TEST(suite, stop_driver)
    }
}







//-------------------------------------------------------------------
//
// Pseudo-driver code
//
//-------------------------------------------------------------------









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

	// TODO update flag in device struct?

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

	// TODO update flag in device struct?


	rc = pod_dev_unlink( drv, dev );
	if( rc ) return rc;


	return 0;
}


errno_t		test_driver_sense( struct pod_driver *drv )
{
	// 'find' hardware

	drv->private_data = &test_device;

	return 0;
}





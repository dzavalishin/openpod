#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <CUnit/Basic.h>

#include "utils.h"

void CUnitUInitialize(void)
{
    CU_cleanup_registry();
}

void CUnitInitialize(void)
{
    if (CU_initialize_registry() != CUE_SUCCESS) {
        //fprintf(stderr, "Failed to initialize the CUnit registry: %d\n", CU_get_error());
        printf( "Failed to initialize the CUnit registry: %d\n", CU_get_error() );
        exit(1);
    }
}

static int initSuite(void) {
    return 0;
}

static int cleanSuite(void) {
    return 0;
}

CU_pSuite CUnitCreateSuite(const char* title)
{
    CU_pSuite suite = NULL;
    suite = CU_add_suite(title, initSuite, cleanSuite);
    if (suite == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    return suite;
}



#if 0
/*
//void test_fail(int rc); // Call from any test to return to test runner and signal failure
//void test_fail_msg(int rc, const char *msg); // Call from any test to return to test runner and signal failure

int hal_mutex_init( void *a, const char *b ) { (void) a; (void) b; }
int hal_mutex_lock( void *a ) { (void) a; }
int hal_mutex_unlock( void *a ) { (void) a; }
int hal_mutex_is_locked( void *a ) { (void) a; return 1; }

int debug_max_level_error = 10;
*/



errno_t		pod_dev_link( struct pod_driver *drv, struct pod_device *dev )	// Report a new available device to the OS kernel
{
	// Empty - add dev to list? Do some requests?
	return 0;
}

errno_t		pod_dev_unlink( struct pod_driver *drv, struct pod_device *dev )
{
	// Empty - remove dev from list? Stop requests?
	return 0;
}

errno_t		pod_dev_event( struct pod_driver *drv, struct pod_device *dev, int event_id, void *event_info )
{
	switch( event_id )
	{
	case POD_EVENT_LOG:
		printf("Device says: '%s'\n", event_info );
		break;

	default:
		return EINVAL;	// Unknown event type
	}

	return 0;
}
	


#endif

#include <errno.h>

#include <openpod.h>
#include <pod_deffunc.h>


errno_t		pod_default_construct( struct pod_driver *drv )
{
	(void) drv;
	// Empty
	return 0;
}

errno_t		pod_default_destruct( struct pod_driver *drv )
{
	(void) drv;
	// Empty
	return 0;
}



//-------------------------------------------------------------------
//
// Driver code - default implementation, possibly won't be modified
//
//-------------------------------------------------------------------



errno_t		
single_dev_driver_activate( struct pod_driver *drv )
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


errno_t		
single_dev_driver_deactivate( struct pod_driver *drv )
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






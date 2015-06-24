#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <openpod.h>


// Bare bones implementation of OpenPOD kernel API


errno_t		pod_dev_link( struct pod_driver *drv, struct pod_device *dev )	// Report a new available device to the OS kernel
{
	(void) drv;
	(void) dev;

	fprintf( stderr, "Device link: 0x%p\n", dev );

	// Empty - add dev to list? Do some requests?
	return 0;
}

errno_t		pod_dev_unlink( struct pod_driver *drv, struct pod_device *dev )
{
	(void) drv;
	(void) dev;

	fprintf( stderr, "Device unlink: 0x%p\n", dev );

	// Empty - remove dev from list? Stop requests?
	return 0;
}

errno_t		pod_dev_event( struct pod_driver *drv, struct pod_device *dev, int event_id, void *event_info )
{
	(void) drv;
	(void) dev;

	fprintf( stderr, "Device event: 0x%p, id %d\n", dev, event_id );

	switch( event_id )
	{
	case POD_EVENT_LOG:
		fprintf( stderr, "Device says: '%s'\n", (const char *)event_info );
		break;

	default:
		return EINVAL;	// Unknown event type
	}

	return 0;
}
	


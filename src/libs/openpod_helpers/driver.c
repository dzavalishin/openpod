#include <errno.h>

#include <openpod.h>

//-------------------------------------------------------------------
//
// Driver methods
//
//-------------------------------------------------------------------





errno_t	pod_construct( pod_driver *drv )
{
	if( (drv == 0) || (drv->calls.pod_construct == 0) )
		return EFAULT;

	return drv->calls.pod_construct( drv );
}
	

errno_t	pod_destruct( pod_driver *drv )
{
	if( (drv == 0) || (drv->calls.pod_destruct == 0) )
		return EFAULT;

	return drv->calls.pod_destruct( drv );
}


errno_t	pod_activate( pod_driver *drv )
{
	if( (drv == 0) || (drv->calls.pod_activate== 0) )
		return EFAULT;

	return drv->calls.pod_activate( drv );
}


errno_t	pod_deactivate( pod_driver *drv )
{
	if( (drv == 0) || (drv->calls.pod_deactivate== 0) )
		return EFAULT;

	return drv->calls.pod_deactivate( drv );
}


errno_t	pod_sense( pod_driver *drv )
{
	if( (drv == 0) || (drv->calls.pod_sense== 0) )
		return EFAULT;

	return drv->calls.pod_sense( drv );
}


//errno_t	pod_probe( pod_driver *drv, bus?, dev? )


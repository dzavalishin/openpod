#include <pod_properties.h>
#include <openpod.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>


//-------------------------------------------------------------------
//
// Generic properties manipulation code
//
//-------------------------------------------------------------------


errno_t pod_gen_listproperties( pod_properties *ps, int nProperty, char *pValue, int vlen )
{
    //SHOW_FLOW( 8, "gen_listproperties %d", nProperty );
    if( ((unsigned)nProperty) >= ps->lsize )
        return ENOENT;

    size_t sz = strlcpy( pValue, ps->list[nProperty].name, vlen );
    return (sz >= vlen) ? E2BIG : 0;
}



#define stricmp strcasecmp

static pod_property *find_property( pod_properties *ps, const char *pName )
{
    // TODO check/eat prefix - not used
    // TODO binsearch
    unsigned int i;

    for( i = 0; i < ps->lsize; i++ )
        if( 0 == stricmp(pName, ps->list[i].name) )
            return ps->list+i;

    return 0;
}


errno_t	pod_gen_getproperty( pod_properties *ps, void *context, const char *pName, char *pValue, int vlen )
{
    pod_property *p = find_property( ps, pName );
    if( 0 == p )
        return ENOENT;

    void *vp;

    // If property has pointer to property value - just use it
    if( p->valp )
        vp = p->valp;
    else // Call function to get actual value pointer
        {
        if( 0 == ps->valp )
            //return EFAULT;
            vp = 0;
        else
            vp = ps->valp(ps, context, p->offset );
        }

    // If property has personal get func - use it
    if( p->getf )
    {
        errno_t rc = p->getf( ps, context, p->offset, vp, pValue, vlen );
        //if( rc )
        return rc;

        //goto act;
    }

    // No get func? Really need vp
    if( 0 == vp )
        return EFAULT;

	// convert property value to string

    switch( p->type )
    {
    case pt_int32:
        {
            u_int32_t *ip = vp;
            snprintf( pValue, vlen, "%d", *ip );
            return 0;
        }

    case pt_mstring:
        snprintf( pValue, vlen, "%s", *((char **)vp) );
        return 0;

    default:
        return ENXIO;

    }

	// Can't reach
    return ENOTTY;
}


errno_t	pod_gen_setproperty( pod_properties *ps, void *context, const char *pName, const char *pValue )
{
    pod_property *p = find_property( ps, pName );
    if( 0 == p )
        return ENOENT;

    void *vp;

    // If property has pointer to property value - just use it
    if( p->valp )
        vp = p->valp;
    else // Call function to get actual value pointer
        {
        if( 0 == ps->valp )
            //return EFAULT;
            vp = 0;
        else
            vp = ps->valp(ps, context, p->offset );
        }


    // If property has personal set func - just use it
    if( p->setf )
    {
        errno_t rc = p->setf( ps, context, p->offset, vp, pValue );
        if( rc )
            return rc;

        goto act;
    }

    // No set func? Really need vp
    if( 0 == vp )
        return EFAULT;

    switch( p->type )
    {
    case pt_int32:
        {
            u_int32_t *ip = vp;
            if( 1 != sscanf( pValue, "%d", ip ) )
                return EINVAL;
        }
        break;

    case pt_mstring:
        {
            size_t len = strlen( pValue )+1;
            if( len > 64*1024 )
                return EINVAL;

            char *olds = *(char **)vp;
            char *news = pod_malloc( len );

            if( olds ) pod_free( olds );

            memcpy( news, pValue, len );

            *((char **)vp) = news;
        }
        break;

    default:
        return ENXIO;

    }

act:
    // now tell 'em to activate new value
    if(p->activate) 
        {
        errno_t rc = p->activate(ps, context, p->offset, vp );
        if( rc ) return rc;
        }

    return 0;
}





//-------------------------------------------------------------------
//
// Generic property manipulation code for a device.
//
//-------------------------------------------------------------------


errno_t pod_drv_listproperties( struct pod_driver *drv, int nProperty, char *pValue, int vlen )
{
	return pod_gen_listproperties( drv->prop, nProperty, pValue, vlen );
}

errno_t	pod_drv_getproperty( struct pod_driver *drv, const char *pName, char *pValue, int vlen )
{
	return pod_gen_getproperty( drv->prop, drv, pName, pValue, vlen );
}

errno_t	pod_drv_setproperty( struct pod_driver *drv, const char *pName, const char *pValue )
{
	return pod_gen_setproperty( drv->prop, drv, pName, pValue );
}


//-------------------------------------------------------------------
//
// Generic property manipulation code for a device.
//
//-------------------------------------------------------------------


errno_t pod_dev_listproperties( struct pod_device *dev, int nProperty, char *pValue, int vlen )
{
	return pod_gen_listproperties( dev->prop, nProperty, pValue, vlen );
}

errno_t	pod_dev_getproperty( struct pod_device *dev, const char *pName, char *pValue, int vlen )
{
	return pod_gen_getproperty( dev->prop, dev, pName, pValue, vlen );
}

errno_t	pod_dev_setproperty( struct pod_device *dev, const char *pName, const char *pValue )
{
	return pod_gen_setproperty( dev->prop, dev, pName, pValue );
}

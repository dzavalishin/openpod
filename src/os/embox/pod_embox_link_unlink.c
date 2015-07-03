#include <openpod.h>

#include "pod_kernel_globals.h"

// ------------------------------------------------------------------
//
// Link/unlink device.
//
// These functions, implemented in kernel, will be called by OpenPOD
// drivers to presend found devices to kernel.
//
// ------------------------------------------------------------------

pod_device	*active_video_driver;
static void attach_pod_framebuf();


// Report a new available device to the OS kernel
errno_t		pod_dev_link( struct pod_driver *drv, struct pod_device *dev )
{
    (void) drv;
    (void) dev;

    //fprintf( stderr, "Driver '%s' device link: 0x%p\n", drv->name, dev );

    switch( dev->class_id )
    {
    case POD_DEV_CLASS_VIDEO:
        if( active_video_driver == 0 )
        {
            active_video_driver = dev;
            attach_pod_framebuf();
            break;
        }
        break;

    case POD_DEV_CLASS_NET:
        // Your code - register a new network interface using given device
        break;

    case POD_DEV_CLASS_BLOCK:
        // Your code - register a new disk device
        break;
    }

    return 0;
}

errno_t		pod_dev_unlink( struct pod_driver *drv, struct pod_device *dev )
{
    (void) drv;
    (void) dev;

    //fprintf( stderr, "Device unlink: 0x%p\n", dev );

    switch( dev->class_id )
    {
    case POD_DEV_CLASS_VIDEO:
        // Houston, we have a problem
        break;

    case POD_DEV_CLASS_NET:
        // Your code - deregister corresponding network interface
        // find_net_if_by_dev( dev ); ... stop and kill if...
        break;

    case POD_DEV_CLASS_BLOCK:
        // Your code - do all we need when we loose disk
        break;
    }

    // No point to return error, (sh)it happened already
    return 0;
}

#include <drivers/video/fb.h>

static int openpod_set_var(struct fb_info *info, const struct fb_var_screeninfo *var) {
    struct pod_video_rq_mode	m;

    m.x_size = var->xres;
    m.y_size = var->yres;

    //var->xres_virtual
    //var->yres_virtual

    if( (var->xoffset != 0) || (var->yoffset != 0) )
        return -EINVAL;

    switch( var->bits_per_pixel )
    {
    case 32:		m.buf_fmt = pod_pixel_rgba; 	break;
    case 24:		m.buf_fmt = pod_pixel_rgb; 	break;
    case 16:		m.buf_fmt = pod_pixel_r5g6b5; 	break;
    default:
        return -EINVAL;
    }

    errnp_t rc = pod_dev_method( active_video_driver, pod_video_setmode, &m );
    if( rc )
        return -rc; // TODO check rc;

    return 0;
}

static int openpod_get_var(struct fb_info *info, struct fb_var_screeninfo *var) {

    if( 0 == active_video_driver ) return -ENODEV; // TODO check rc

    struct pod_video_rq_mode	m;
    if( pod_dev_method( active_video_driver, pod_video_getmode, &m ) )
        return -EIO; // TODO check rc;

    memset(var, 0, sizeof(struct fb_var_screeninfo));


    var->xres           = m.x_size;
    var->yres           = m.y_size;

    switch( m.buf_fmt )
    {
    case pod_pixel_rgba:	ar->bits_per_pixel = 32; break;
    case pod_pixel_rgb:		ar->bits_per_pixel = 24; break;
    case pod_pixel_r5g6b5:	ar->bits_per_pixel = 16; break;
    case pod_pixel_r5g6b5:	ar->bits_per_pixel = 16; break;
    default: return ENXIO;
    }

    var->xres_virtual   = m.x_size;
    var->yres_virtual   = m.y_size;

    var->xoffset        = 0;
    var->yoffset        = 0;

    return 0;
}

static const struct fb_ops openpod_ops = {
    .fb_set_var = openpod_set_var,
    .fb_get_var = openpod_get_var,
};

static errno_t attach_pod_framebuf()
{
    struct fb_info *info;

    info = fb_create(&openpod_ops, mmap_base, mmap_len);

    if (info == NULL)
    {
        return EFAULT;
    }

    return 0;
}



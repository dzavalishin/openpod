/**
 * @file
 *
 * @date 02.07.15
 * @author dz
 */

#include "pod_bochs.h"
#include "pod_bochs_vbe.h"

#include <pod_io_video.h>
#include <pod_kernel_api.h>

#include <errno.h>
#include <string.h>

//#include <sys/mman.h>
//#include <drivers/pci/pci.h>
//#include <drivers/pci/pci_driver.h>
//#include <drivers/video/fb.h>
//#include <drivers/video/vbe.h>
//#include <util/binalign.h>
//#include <framework/mod/options.h>
//#include <mem/page.h>

static const int 	vbuf_max_bytes = VBE_DISPI_MAX_XRES * VBE_DISPI_MAX_YRES * VBE_DISPI_MAX_BPP / 8;
static physaddr_t 	vbuf_mem = 0;
static void *		vbuf_vaddr = 0;



#if 0

PCI_DRIVER("bochs", bochs_init, PCI_VENDOR_ID_BOCHS, PCI_DEV_ID_BOCHS_VGA);


static int bochs_init(struct pci_slot_dev *pci_dev) {
    char *mmap_base = (char *)(pci_dev->bar[0] & ~0xf); /* FIXME */
    size_t mmap_len = binalign_bound(VBE_DISPI_MAX_XRES
                                     * VBE_DISPI_MAX_YRES
                                     * VBE_DISPI_MAX_BPP / 8, PAGE_SIZE());
    struct fb_info *info;

    if (MAP_FAILED == mmap_device_memory(mmap_base,
                                         mmap_len,
                                         PROT_READ|PROT_WRITE|PROT_NOCACHE,
                                         MAP_FIXED,
                                         (unsigned long) mmap_base)) {
        return -EIO;
    }

    info = fb_create(&bochs_ops, mmap_base, mmap_len);
    if (info == NULL) {
        munmap(mmap_base, mmap_len);
        return -ENOMEM;
    }

    return 0;
}

#endif

static errno_t          pod_bochs_io_getmode( struct pod_device *dev, void *arg );
static errno_t          pod_bochs_io_setmode( struct pod_device *dev, void *arg );
static errno_t		pod_bochs_io_getbestmode( pod_device *dev, void *arg );

static errno_t          (*class_interface[])(struct pod_device *dev, void *arg) =
{
    0, // nop
    pod_bochs_io_getmode,
    pod_bochs_io_setmode,
    pod_bochs_io_getbestmode,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};



errno_t pod_bochs_activate( struct pod_driver *drv )
{
    errno_t rc;

    // Not actually found?
    if( vbuf_mem == 0 )
        return ENODEV;

    if( vbuf_vaddr == 0 )
    {
        rc = pod_alloc_vaddr( vbuf_max_bytes, &vbuf_vaddr );
        if( rc ) return rc;
    }

    rc = pod_map_mem( vbuf_mem, vbuf_vaddr, vbuf_max_bytes, POD_MAP_RW|POD_MAP_NOCACHE );
    if( rc ) return rc;

    return single_dev_driver_activate(drv);
}

errno_t pod_bochs_deactivate( struct pod_driver *drv )
{
    errno_t rc;

    if( vbuf_mem == 0 )
        return ENODEV;

    rc = single_dev_driver_deactivate(drv);
    if( rc ) pod_log_print( 0, "Bochs video drv: can't deactivate, rc = %d", rc );

    rc = pod_unmap_mem( vbuf_mem, vbuf_vaddr, vbuf_max_bytes, 0 );
    if( rc ) return rc;

    return 0;
}


errno_t pod_bochs_destruct( struct pod_driver *drv )
{
    errno_t rc = 0;

    if( vbuf_vaddr )
        rc = pod_free_vaddr( vbuf_max_bytes, vbuf_vaddr );

    vbuf_vaddr = 0;

    return rc;
}









static errno_t
pod_bochs_io_getbestmode( pod_device *dev, void *arg )
{
    struct pod_video_rq_mode *m = arg;
    // Get best possible video mode

    m->vbuf = 0; // no direct access to video buffer

    memset( m, 0, sizeof(struct pod_video_rq_mode));

    m->x_size  = VBE_DISPI_MAX_XRES;
    m->y_size  = VBE_DISPI_MAX_YRES;
    m->buf_fmt = pod_pixel_rgba; 	

    return 0;
}



static errno_t
pod_bochs_io_getmode( pod_device *dev, void *arg )
{
    struct pod_video_rq_mode *m = arg;
    // Your code to do actual non-blocking io here - get current video mode

    m->vbuf = 0; // no direct access to video buffer

    memset( m, 0, sizeof(struct pod_video_rq_mode));

    m->x_size           = pod_bochs_vbe_read(VBE_DISPI_INDEX_XRES);
    m->y_size           = pod_bochs_vbe_read(VBE_DISPI_INDEX_YRES);

    int bpp 		= pod_bochs_vbe_read(VBE_DISPI_INDEX_BPP);

    switch( bpp )
    {
    case 32:		m->buf_fmt = pod_pixel_rgba; 	break;
    case 24:		m->buf_fmt = pod_pixel_rgb; 	break;
    case 16:		m->buf_fmt = pod_pixel_r5g6b5; 	break;
    default:
        return ENXIO;
    }

    //var->xres_virtual   = pod_bochs_vbe_read(VBE_DISPI_INDEX_VIRT_WIDTH);
    //var->yres_virtual   = pod_bochs_vbe_read(VBE_DISPI_INDEX_VIRT_HEIGHT);
    //var->xoffset        = pod_bochs_vbe_read(VBE_DISPI_INDEX_X_OFFSET);
    //var->yoffset        = pod_bochs_vbe_read(VBE_DISPI_INDEX_Y_OFFSET);

    return 0;
}


static errno_t
pod_bochs_io_setmode( pod_device *dev, void *arg )
{
    struct pod_video_rq_mode *m = arg;

    // Set new video mode

    if( m->x_size > VBE_DISPI_MAX_XRES
        || m->y_size > VBE_DISPI_MAX_YRES )    // || var->bits_per_pixel > VBE_DISPI_MAX_BPP)
    {
        return EINVAL;
    }

    int bpp = 32;
    switch( m->buf_fmt )
    {
    case pod_pixel_rgba: bpp = 32; break;
    case pod_pixel_rgb:  bpp = 24; break;
    default:
        return EINVAL;
    }

    pod_bochs_vbe_write(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);

    pod_bochs_vbe_write(VBE_DISPI_INDEX_XRES, m->x_size);
    pod_bochs_vbe_write(VBE_DISPI_INDEX_YRES, m->y_size);

    pod_bochs_vbe_write(VBE_DISPI_INDEX_BPP, bpp );

    pod_bochs_vbe_write(VBE_DISPI_INDEX_VIRT_WIDTH, m->x_size );
    pod_bochs_vbe_write(VBE_DISPI_INDEX_VIRT_HEIGHT, m->y_size );

    pod_bochs_vbe_write(VBE_DISPI_INDEX_X_OFFSET, 0 );
    pod_bochs_vbe_write(VBE_DISPI_INDEX_Y_OFFSET, 0 );

    pod_bochs_vbe_write(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_ENABLED | VBE_DISPI_LFB_ENABLED);

    return 0;
}





//-------------------------------------------------------------------
//
// Detect hardware
//
//-------------------------------------------------------------------


errno_t
pod_bochs_sense( struct pod_driver *drv )
{
    pod_device *dev = &pod_bochs_device;
    drv->private_data = dev;

    dev->class_interface = class_interface;

    POD_DEV_STATE_SET( dev, POD_DEV_STATE_INIT );


    // Actually detect hardware
    pod_bochs_vbe_write(VBE_DISPI_INDEX_ID, VBE_DISPI_ID2);
    int id = pod_bochs_vbe_read(VBE_DISPI_INDEX_ID);

    if( id < VBE_DISPI_ID2 || id > VBE_DISPI_ID3 )
        return ENOENT;


    // TODO
    //vbuf_mem =

    POD_DEV_STATE_SET( dev, POD_DEV_STATE_FOUND );

    // Device is not activated, but give kernel chance
    // to take a look so that kernel can decide whether
    // corresponding driver is needed. Especially of
    // use for video drivers.
    pod_dev_event( drv, dev, POD_EVENT_PREVIEW, 0 );

    return 0;
}



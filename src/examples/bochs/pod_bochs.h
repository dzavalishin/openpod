#ifndef POD_BOCHS_H
#define POD_BOCHS_H

#include <openpod.h>
#include <errno.h>

#define BOCHS_VIDEO_DRV_DEFAULT_X_SIZE 1024
#define BOCHS_VIDEO_DRV_DEFAULT_Y_SIZE 768

// TODO FIXME define correct values

#define VBE_DISPI_MAX_XRES 1280
#define VBE_DISPI_MAX_YRES 960
#define VBE_DISPI_MAX_BPP  32

errno_t pod_bochs_activate( struct pod_driver *drv );
errno_t pod_bochs_deactivate( struct pod_driver *drv );

errno_t pod_bochs_destruct( struct pod_driver *drv );


errno_t pod_bochs_sense( struct pod_driver *drv );

int pod_bochs_vbe_read(unsigned short index);
void pod_bochs_vbe_write(unsigned short index, unsigned short value);



extern pod_device pod_bochs_device;
extern pod_driver pod_bochs_driver;


#endif // POD_BOCHS_H

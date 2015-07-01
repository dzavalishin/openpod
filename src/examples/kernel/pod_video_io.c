#include <openpod.h>
#include <pod_io_video.h>

#include "pod_kernel_globals.h"



// ------------------------------------------------------------------
//
// Work with video driver
//
// ------------------------------------------------------------------


// Clear screen
void
video_clear_screen(void)
{
    pod_dev_method( active_video_driver, pod_video_clear_all, 0 );
}



// Put bitmap on screen
void
video_paint( uint32_t x, uint32_t y, uint32_t x_size, uint32_t y_size, void *bitmap )
{
    struct pod_video_rq_rw arg;

    arg.x = x;
    arg.y = y;
    arg.x_size = x_size;
    arg.y_size = y_size;

    arg.z = 0;
    arg.flags = POD_VIDEO_IGNORE_A|POD_VIDEO_IGNORE_Z;

    arg.buf = bitmap;
    arg.buf_fmt = pod_pixel_rgba;

    pod_dev_method( active_video_driver, pod_video_write, &arg );
}

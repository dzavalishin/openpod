#ifndef POD_IO_VIDEO_H
#define POD_IO_VIDEO_H

#include "pod_types.h"
//#include "pod_rq.h"
#include "pod_device.h"

//*******************************************************************
//
// OpenPOD
//
// Video IO structs and operation ids
//
//*******************************************************************


//-------------------------------------------------------------------
//
// Constants
//
//-------------------------------------------------------------------

// Driver supports boolean alpha channel (blits if alpha byte is not zero)
#define POD_CLASS_VIDEO_FLAG_HAS_APLHA  (1<<0)
// Driver supports fractional alpha channel (blit mixes pixel according to alpha byte value)
#define POD_CLASS_VIDEO_FLAG_HAS_FAPLHA (1<<0)
// Driver supports Z buffer (blits if Z coordinate of pixel we paint is > than Z coordinate of onscreen pixel )
#define POD_CLASS_VIDEO_FLAG_HAS_ZBUF   (1<<2)


// значение поля operation или индекс в таблицу методов class_interface
typedef enum pod_video_operartions 
{
    pod_video_nop, 
    pod_video_getmode, pod_video_setmode,
    pod_video_clear_all, pod_video_clear, 
    pod_video_move, 
    pod_video_write, pod_video_read, 
    pod_video_write_part, pod_video_read_part
} pod_video_operartions;

// Pixel format, pod_pixel_rgba is preferred
typedef enum pod_pixel_fmt 
{
    pod_pixel_rgba,     // 32 bit RGBA, A byte is ignored by HW
    pod_pixel_rgb,      // 24 bit RGB
    pod_pixel_r5g6b5,   // 16 bit, 5-6-5
    pod_pixel_r5g5b5,   // 16 bit, 5-5-5
} pod_pixel_fmt;

// bitbil flags, encode
// buffer to screen copy mode. 
// screen to buffer copy ignores flags and copies everything?
typedef enum pod_v_flags {
    pod_video_ignore_zbuffer,   // ignore z coordinate (but update z buffer)
    pod_video_ignore_alpha,     // ignore A (aplha channel)
} pod_v_flags;




//-------------------------------------------------------------------
//
// Struct to fill into the class_specific field of request
//
//-------------------------------------------------------------------


// clear
struct pod_video_rq_sqare 
{
    uint32_t    x, y;
    uint32_t    x_size, y_size;
};

// move from screen to screen
struct pod_video_rq_2sqare 
{
    uint32_t    from_x, from_y;
    uint32_t    from_x_size, from_y_size;

    uint32_t    to_x, to_y;
    uint32_t    to_x_size, to_y_size;
};

// write, read - пересылка полного битмапа на экран или с экрана
struct pod_video_rq_rw 
{
    uint32_t    x, y;
    uint32_t    x_size, y_size;

    uint32_t    z;
    pod_v_flags flags;

    char        *buf;

    pod_pixel_fmt   buf_fmt;    // формат пикселя буфера ядра
};

// write_part, read_part - пересылка части битмапа на экран или с экрана
struct pod_video_rq_rw_part 
{
    uint32_t    from_x, from_y;     // point to start in buf
    uint32_t    from_x_size, from_y_size;   // full size of bitmap in buf

    uint32_t    to_x, to_y;         // point to start on screen

    uint32_t    move_x_size, move_y_size;   // size of sqare to move

    uint32_t    z;              // z position
    pod_v_flags flags;

    char        *buf;

    pod_pixel_fmt   buf_fmt;    // формат пикселя буфера ядра
};


// getmode, setmode
struct pod_video_rq_mode 
{
    // если невалидно - ставим ближайший вверх, если нет - ошибка
    uint32_t    x_size, y_size; 
// формат пикселя экрана, желательно rgba
    pod_pixel_fmt   buf_fmt;    

    // Возвращаемое значение.
    physaddr_t  vbuf;       // может быть 0? тогда всё через методы?

};







#endif // POD_IO_VIDEO_H

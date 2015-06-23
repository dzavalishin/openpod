#ifndef POD_IO_VIDEO_H
#define POD_IO_VIDEO_H

#include "pod_types.h"
#include "pod_rq.h"

//*******************************************************************
//
// OpenPOD
//
// Video IO structs and operation ids
//
//*******************************************************************


// значение поля operation или индекс в таблицу методов class_interface
enum pod_video_operartions 
{
	nop, 
	getmode, setmode,
	clear_all, clear, 
	move, 
	write, read, 
	write_part, read_part
};

// Pixel format, pod_pixel_rgba is preferred
enum pod_pixel_fmt 
{
	pod_pixel_rgb,		// 24 bit RGB
	pod_pixel_rgba,		// 32 bit RGBA, A byte is ignored by HW
	pod_pixel_r5g6b5,	// 16 bit, 5-6-5
	pod_pixel_r5g5b5,	// 16 bit, 5-5-5
};

// bitbil flags, encode
// buffer to screen copy mode. 
// screen to buffer copy ignores flags and copies everything?
enum pod_v_flags {
	pod_video_ignore_zbuffer,	// ignore z coordinate (but update z buffer)
	pod_video_ignore_alpha,		// ignore A (aplha channel)
};




//*******************************************************************
//
// Struct to fill into the class_specific field of request
//
//*******************************************************************

// TODO move defines to rq hdr - BUG, must return rq
#define create_op_rq( __type, __class, __op ) \
    do { \
        pod_request *rq = pod_malloc( sizeof(pod_request) + sizeof(__type) ); \
        rq->class_specific = ((void*)rq) + sizeof(pod_request); \
        rq->requset_class = (__class); \
        rq->operation = (__op); \
        rq->io_prio = 0x1000; \
        rq->err = not_started;\
        rq->done = 0;         \
        } while(0)

#define rq_specific( __rq, __type ) \
	( (__type *) ((__rq)->class_specific) )

// clear
struct pod_video_rq_sqare 
{
	uint32_t	x, y;
	uint32_t	x_size, y_size;
};

// move from screen to screen
struct pod_video_rq_2sqare 
{
	uint32_t	from_x, from_y;
	uint32_t	from_x_size, from_y_size;

	uint32_t	to_x, to_y;
	uint32_t	to_x_size, to_y_size;
};

// write, read - пересылка полного битмапа на экран или с экрана
struct pod_video_rq_rw 
{
	uint32_t	x, y;
	uint32_t	x_size, y_size;

	uint32_t	z;
	pod_v_flags 	flags;

	char		*buf;

	pod_pixel_fmt	buf_fmt;	// формат пикселя буфера ядра
};

// write_part, read_part - пересылка части битмапа на экран или с экрана
struct pod_video_rq_rw_part 
{
	uint32_t	from_x, from_y;		// point to start in buf
	uint32_t	from_x_size, from_y_size; 	// full size of bitmap in buf

	uint32_t	to_x, to_y;			// point to start on screen

	uint32_t	move_x_size, move_y_size;	// size of sqare to move

	uint32_t	z;				// z position
	pod_v_flags 	flags;

	char		*buf;

	pod_pixel_fmt	buf_fmt;	// формат пикселя буфера ядра
};


// getmode, setmode
struct pod_video_rq_mode 
{
	// если невалидно - ставим ближайший вверх, если нет - ошибка
	uint32_t	x_size, y_size;	
// формат пикселя экрана, желательно rgba
	pod_pixel_fmt	buf_fmt;	

	// Возвращаемое значение.
	physaddr_t	vbuf;		// может быть 0? тогда всё через методы?

};







#endif // POD_IO_VIDEO_H

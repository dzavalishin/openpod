#ifndef POD_DEVICE_H
#define POD_DEVICE_H

#include "pod_properties.h"
#include "pod_kernel_api.h"

typedef enum pod_rq_status 
{ 
	pod_rq_status_ok,
	pod_rq_status_unprocessed,	// Requset is not started in drv
	pod_rq_status_dequeued,		// Requset was aborted by call to ...dequeue
	pod_rq_status_in_progress,	// Is being served right now
	pod_rq_status_inknown_err,	// Error: Something we didn't think about
	pod_rq_status_ioerr,		// Error: IO
	pod_rq_status_stopped,		// Error: Driver is not active

} pod_rq_status;

struct pod_request;

typedef struct pod_request
{
	uint8_t		request_class; 	// block/character/net/video io
	uint8_t		operation;		// op id - read/write/clear screen/etc

	// request priority, more = earlier
	uint32_t	io_prio;

	pod_rq_status	err;			// результат исполнения, 0 = ок

	// Req struct part specific for request class and op id
	void		*class_specific;

	// Driver will call this when done
	void		(*done)( struct pod_request *rq );

} pod_request;

typedef struct pod_dev_f
{
} pod_dev_f;


struct pod_driver;
struct pod_q;

typedef struct pod_device
{
//	char			magic[4];
	uint32_t		magic;

	uint8_t			class_id;
	uint8_t			pad0;
	uint8_t			pad1;
	uint8_t			flags; // operating or not

	struct pod_driver	*drv;

	pod_dev_f		*calls; // dev io entry points

	pod_properties		*prop;

	void			*class_interface; // dev class specific interface
	void			*private_data;

	// ----------------------------------------------------------------------
	// Fields below are used by default framework code

	// Request queue, used by pod_dev_q_ functions
	struct pod_q		*default_r_q;		// default request q
	pod_request		*curr_rq;		// request we do now
	pod_thread		*rq_run_thread;	// thread used to run requests
	pod_cond		*rq_run_cond;	// triggered to run next request

} pod_device;












#endif // POD_DEVICE_H

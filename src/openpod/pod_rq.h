#ifndef POD_RQ_H
#define POD_RQ_H

#include "pod_types.h"
#include "pod_properties.h"
#include "pod_kernel_api.h"

struct pod_driver;
struct pod_device;
//struct pod_q;



//********************************************************************
//
// OpenPOD
//
// Driver request structure and methods.
//
//********************************************************************




typedef enum pod_rq_status 
{ 
	pod_rq_status_ok,
	pod_rq_status_unprocessed,	// Requset is not started in drv
	pod_rq_status_dequeued,		// Requset was aborted by call to ...dequeue
	pod_rq_status_in_progress,	// Is being served right now
	pod_rq_status_inknown_err,	// Error: Something we didn't think about
	pod_rq_status_ioerr,		// Error: IO
	pod_rq_status_stopped,		// Error: Driver is not active
	pod_rq_status_param,		// Error: Invalid request parameters

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
	void		*op_arg;

	// Driver will call this when done
	void		(*done)( struct pod_request *rq );

} pod_request;




#endif // POD_RQ_H

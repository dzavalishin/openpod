#ifndef POD_DEVICE_H
#define POD_DEVICE_H

#include "pod_properties.h"
#include "pod_kernel_api.h"

struct pod_driver;
struct pod_device;
struct pod_q;



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



typedef struct pod_dev_f
{

	errno_t	(*enqueue)( struct pod_device *dev, pod_request *rq ); 
	errno_t	(*dequeue)( struct pod_device *dev, pod_request *rq );
	errno_t	(*fence)( struct pod_device *dev, pod_request *rq );
	errno_t	(*raise_prio)( struct pod_device *dev, pod_request *rq, uint32_t io_prio );


} pod_dev_f;


#define POD_DEVICE_MAGIC 0xDEFF00A3

// TODO describe state flags in spec

// Device is initialized (constructed)
#define POD_DEV_STATE_INIT		(1<<0)
// Device hardware existst and operational
#define POD_DEV_STATE_FOUND		(1<<1)
// Device is linked to the kernel and supposed to be ready to serve requests
#define POD_DEV_STATE_LINKED	(1<<2)
// Device is still linked to the kernel, but is paused, no requests must 
// be started in this state, but device accepts requests
#define POD_DEV_STATE_PAUSE		(1<<3)
// Device is stopping, no requests are served, and will 
// return pod_rq_status_stopped err code
#define POD_DEV_STATE_EXIT		(1<<4)


#define POD_DEV_REPORT_STATE( ___dev ) do { int state_copy = (___dev)->state_flags; pod_dev_event( drv, dev, POD_EVENT_STATE, &state_copy ); } while(0)

#define POD_DEV_STATE_SET( ___dev, ___stfl ) do { (___dev)->state_flags |= (___stfl); POD_DEV_REPORT_STATE( ___dev );  } while(0)
#define POD_DEV_STATE_CLEAR( ___dev, ___stfl ) do { (___dev)->state_flags &= ~(___stfl); POD_DEV_REPORT_STATE( ___dev ); } while(0)

#define POD_DEV_STATE_CHECK( ___dev, ___stfl ) ( (  ((___dev)->state_flags) & (___stfl) ) != 0 )



typedef struct pod_device
{
//	char			magic[4];
	uint32_t			magic;

	uint8_t				class_id;
	uint8_t				class_flags; // Class specific flags, 
	uint8_t				pad1;
	uint8_t				state_flags; // Device state

	struct pod_driver	*drv;

	pod_dev_f			*calls; // dev io entry points

	pod_properties		*prop;

	void				*class_interface; // dev class specific interface
	void				*private_data;

	// ----------------------------------------------------------------------
	// Fields below are used by default framework code

	// Request queue, used by pod_dev_q_ functions
	struct pod_q		*default_r_q;		// default request q
	pod_request			*curr_rq;		// request we do now
	pod_thread			*rq_run_thread;	// thread used to run requests
	pod_cond			*rq_run_cond;	// triggered to run next request

} pod_device;




inline errno_t	pod_rq_enqueue( pod_device *dev, pod_request *rq )
{
	if( (dev == 0) || (dev->calls == 0) || (dev->calls->enqueue == 0 ) )
		return EFAULT;

	if( rq->request_class != dev->class_id ) 
	{
		rq->err = pod_rq_status_param;
		if( rq->done ) rq->done( rq );
		return 0;
	}

	if( !POD_DEV_STATE_CHECK( dev, POD_DEV_STATE_INIT ) )
		return ENXIO;

	if( !POD_DEV_STATE_CHECK( dev, POD_DEV_STATE_FOUND ) )
		return ENODEV;

	rq->err = pod_rq_status_unprocessed;

	return dev->calls->enqueue( dev, rq );
}


inline errno_t	pod_rq_dequeue( pod_device *dev, pod_request *rq )
{
	if( (dev == 0) || (dev->calls == 0) || (dev->calls->dequeue == 0 ) )
		return EFAULT;

	if( !POD_DEV_STATE_CHECK( dev, POD_DEV_STATE_INIT ) )
		return ENXIO;

	if( !POD_DEV_STATE_CHECK( dev, POD_DEV_STATE_FOUND ) )
		return ENODEV;

	return dev->calls->dequeue( dev, rq );
}

inline errno_t	pod_rq_fence( pod_device *dev, pod_request *rq )
{
	if( (dev == 0) || (dev->calls == 0) || (dev->calls->fence == 0 ) )
		return EFAULT;

	if( !POD_DEV_STATE_CHECK( dev, POD_DEV_STATE_INIT ) )
		return ENXIO;

	if( !POD_DEV_STATE_CHECK( dev, POD_DEV_STATE_FOUND ) )
		return ENODEV;

	return dev->calls->fence( dev, rq );
}

inline errno_t	pod_rq_raise( pod_device *dev, pod_request *rq, uint32_t io_prio )
{
	if( (dev == 0) || (dev->calls == 0) || (dev->calls->raise_prio == 0 ) )
		return EFAULT;

	if( !POD_DEV_STATE_CHECK( dev, POD_DEV_STATE_INIT ) )
		return ENXIO;

	if( !POD_DEV_STATE_CHECK( dev, POD_DEV_STATE_FOUND ) )
		return ENODEV;

	return dev->calls->raise_prio( dev, rq, io_prio );
}






#endif // POD_DEVICE_H

#ifndef POD_DEVICE_H
#define POD_DEVICE_H

#include "pod_types.h"
#include "pod_properties.h"
#include "pod_kernel_api.h"
#include "pod_rq.h"

struct pod_driver;
struct pod_device;
struct pod_q;



typedef struct pod_dev_f
{

    errno_t (*enqueue)( struct pod_device *dev, pod_request *rq ); 
    errno_t (*dequeue)( struct pod_device *dev, pod_request *rq );
    errno_t (*fence)( struct pod_device *dev );
    errno_t (*raise_prio)( struct pod_device *dev, pod_request *rq, uint32_t io_prio );


} pod_dev_f;


#define POD_DEVICE_MAGIC 0xDEFF00A3


// Device is initialized (constructed)
#define POD_DEV_STATE_INIT      (1<<0)
// Device hardware existst and operational
#define POD_DEV_STATE_FOUND     (1<<1)
// Device is linked to the kernel and supposed to be ready to serve requests
#define POD_DEV_STATE_LINKED    (1<<2)
// Device is still linked to the kernel, but is paused, no requests must 
// be started in this state, but device accepts requests
#define POD_DEV_STATE_PAUSE     (1<<3)
// Device is stopping, no requests are served, and will 
// return pod_rq_status_stopped err code
#define POD_DEV_STATE_EXIT      (1<<4)


#define POD_DEV_REPORT_STATE( ___dev ) do { int state_copy = (___dev)->state_flags; pod_dev_event( drv, dev, POD_EVENT_STATE, &state_copy ); } while(0)

#define POD_DEV_STATE_SET( ___dev, ___stfl ) do { (___dev)->state_flags |= (___stfl); POD_DEV_REPORT_STATE( ___dev );  } while(0)
#define POD_DEV_STATE_CLEAR( ___dev, ___stfl ) do { (___dev)->state_flags &= ~(___stfl); POD_DEV_REPORT_STATE( ___dev ); } while(0)

#define POD_DEV_STATE_CHECK( ___dev, ___stfl ) ( (  ((___dev)->state_flags) & (___stfl) ) != 0 )


#define OPENPOD_DEV_INTERNAL_THREAD_RUNNING (1<<0)
#define OPENPOD_DEV_INTERNAL_THREAD_RUN     (1<<1)


#define POD_DEV_INTERNAL_SET( ___dev, ___stfl ) do { (___dev)->internal_flags |= (___stfl);   } while(0)
#define POD_DEV_INTERNAL_CLEAR( ___dev, ___stfl ) do { (___dev)->internal_flags &= ~(___stfl);  } while(0)
#define POD_DEV_INTERNAL_CHECK( ___dev, ___stfl ) ( (  ((___dev)->internal_flags) & (___stfl) ) != 0 )


typedef struct pod_device
{
//  char            magic[4];
    uint32_t            magic;

    uint8_t             class_id;
    uint8_t             class_flags; // Class specific flags, 
    uint8_t             internal_flags; // Not exposed as state change to kernel
    uint8_t             state_flags; // Device state

    struct pod_driver   *drv;

    pod_dev_f           *calls; // dev io entry points

    pod_properties      *prop;

    void                (**class_interface)(struct pod_device *dev, void *arg); // dev class specific interface
    void                *private_data;

    // ----------------------------------------------------------------------
    // Fields below are used by default framework code

    // Request queue, used by pod_dev_q_ functions
    struct pod_q        *default_r_q;       // default request q
    pod_request         *curr_rq;       // request we do now
    pod_thread          rq_run_thread;  // thread used to run requests
    pod_cond            *rq_run_cond;   // triggered to run next request
    pod_mutex           *rq_run_mutex;
    errno_t             (*default_start_rq)( struct pod_device *dev, pod_request *rq ); // start executing an rq
} pod_device;



// Call dev (class/op specific) function from class_interface. This is 
// an interface identical to what is available via requests, but syncronous.

errno_t pod_dev_method( pod_device *dev, int op_id, void *param );






#endif // POD_DEVICE_H

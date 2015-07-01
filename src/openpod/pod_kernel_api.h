#ifndef POD_KERNEL_API_H
#define POD_KERNEL_API_H



#include "pod_types.h"
//#include "pod_driver.h"
//#include "pod_device.h"


struct pod_driver;
struct pod_device;

typedef struct pod_mutex
{   
    void *private_data;
} pod_mutex;

typedef struct pod_cond
{
    void *private_data;
} pod_cond;

typedef struct pod_thread
{
    union {
        void    *private_data;
        int system_thread_id;
    };
} pod_thread;

#define POD_THREAD_NOT_NULL( __t ) ( (__t).system_thread_id )
#define POD_THREAD_SET_NULL( __t ) ( (__t).system_thread_id = 0 )


// ------------------------------------------------------------------
//
// Device registration
//
// Required.
//
// ------------------------------------------------------------------


errno_t     pod_dev_link( struct pod_driver *drv, struct pod_device *dev ); // Report a new available device to the OS kernel
errno_t     pod_dev_unlink( struct pod_driver *drv, struct pod_device *dev );   // Report device to be unavailable

// ------------------------------------------------------------------
//
// Device state reporting
//
// With pod_dev_event driver reports state change and can send log messages tied to a specific device.
//
// Basically this callback can be safely ignored by kernel. Implement empty function and you're ok.
//
// ------------------------------------------------------------------


// Event types
#define     POD_EVENT_LOG       0   // Log a message about the device, pointer to msg string
#define     POD_EVENT_STATE     1   // Report driver state change, pointer to int state_flags (readonly)

// Report device as soon as it was detected, during pod_sense/pod_probe, so kernel can decide whether it wants
// to start corresponding driver. This is especially critical for video drivers because kernel needs to choose
// between different possible ones to choose the best one.
#define     POD_EVENT_PREVIEW   2   

// TODO
//#define       POD_EVENT_BLK_IOERR     2   // Report io error, must provide detailed info such as block, retry count, etc
//#define       POD_EVENT_CLASS_SPECIFIC_ERR        3   // Report device class specific error, detailed
//#define       POD_EVENT_INTERRUPT     4   // Report device activity (must be requested)
//#define       POD_EVENT_DISCONNECT    5   // Report loss of a device - to differ from driver stop

errno_t     pod_dev_event( struct pod_driver *drv, struct pod_device *dev, int event_id, void *event_info );    // Tell about device event (error? state change?)




// ------------------------------------------------------------------
//
// Threads
//
// Optional.
//
// ------------------------------------------------------------------

errno_t     pod_kernel_thread_start( pod_thread *tid, void (*thread_func)(void *), void *thread_func_arg );
errno_t     pod_kernel_thread_kill( pod_thread tid );

// TODO threadlets/dpc?


// ------------------------------------------------------------------
//
// Timers
//
// Required.
//
// ------------------------------------------------------------------

#define POD_TIMER_PERIODIC      (1<<0)
// Call will be postponed as long as spinlock is still taken
//#define POD_TIMER_CHECKLOCK       (1<<2)

// Request timer_func to be called in msec milliseconds
errno_t     pod_kernel_timer_start( int *timer_id, int msec, int timer_flags, void (*timer_func)(void *), void *timer_func_arg );
// Reset timer. Returns ENOENT if timer does not exist (one-shot timer kills itself automatically).
errno_t     pod_kernel_timer_stop( int timer_id );


// ------------------------------------------------------------------
//
// Sync
//
// Optional, must be implemented if threads exist.
//
// ------------------------------------------------------------------



errno_t     pod_kernel_create_mutex( pod_mutex **mutex );       // allocate and init
errno_t     pod_kernel_init_mutex( pod_mutex *mutex );      // init static one
errno_t     pod_kernel_destroy_mutex( pod_mutex *mutex );   

errno_t     pod_kernel_lock_mutex( pod_mutex *mutex );
errno_t     pod_kernel_unlock_mutex( pod_mutex *mutex );


errno_t     pod_kernel_create_cond( pod_cond **cond );      // allocate and init
errno_t     pod_kernel_init_cond( pod_cond *cond );     // init static one
errno_t     pod_kernel_destroy_cond( pod_cond *cond );

errno_t     pod_kernel_wait_cond( pod_cond *cond, pod_mutex *mutex );
errno_t     pod_kernel_signal_cond( pod_cond *cond );


// ------------------------------------------------------------------
//
// Spinlocks
//
// Required.
//
// ------------------------------------------------------------------

struct pod_spinlock;

// Push interrupt mask, disable interrupts, if SMP - check/take spin lock
errno_t     pod_kernel_spin_lock( struct pod_spinlock *l );
// Pop interrupt mask, if SMP - release spin lock
errno_t     pod_kernel_spin_unlock( struct pod_spinlock *l );


// ------------------------------------------------------------------
//
// Logging and panic
//
// Required.
//
// ------------------------------------------------------------------

errno_t     pod_log_print( int loglevel, const char *format, ... );
void        pod_panic(const char *format, ...);


// ------------------------------------------------------------------
//
// Memory and address space
//
// Required. TODO do we need map/unmap/vaddr space allocation? What to do with MMU-less systems?
//
// ------------------------------------------------------------------


errno_t     pod_alloc_physmem( size_t nbytes, physaddr_t *mem );  // NB! Mem size in bytes! Allocates pages. npages * pagesize >= nbytes
errno_t     pod_alloc_vaddr( size_t nbytes, void **vaddr );       // NB! Mem size in bytes! Allocates pages. npages * pagesize >= nbytes
errno_t     pod_alloc_kheap( size_t nbytes, void **mem );


errno_t     pod_free_physmem( physaddr_t mem );
errno_t     pod_free_kheap( void * );
errno_t     pod_free_vaddr( void * );


#define POD_MAP_FLAG_UNCACHED   (1<<1)

// Allocate x86 low (<1mb) memory - need?
//#define POD_MAP_FLAG_LOWMEM   (1<<16)


void *pod_malloc(size_t ___nb);
//inline void *pod_malloc(size_t ___nb)  { void *ret; errno_t rc = pod_alloc_kheap( (___nb), &ret ); if( rc ) return 0; return ret; }
#define pod_free    pod_free_kheap



errno_t     pod_map_mem( physaddr_t mem, void *vaddr, int flags );
errno_t     pod_unmap_mem( physaddr_t mem, void *vaddr, int flags );


errno_t     pod_wire_mem( void *mem, size_t nbytes );         // Make sure memory is paged in and non-pageable. Can block! Don't call in pod_rq_start!
errno_t     pod_unwire_mem( void *mem, size_t nbytes ); 


#endif // POD_KERNEL_API_H

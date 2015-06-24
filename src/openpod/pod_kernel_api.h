

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
		void 	*private_data;
		int	system_thread_id;
	};
} pod_thread;


// ------------------------------------------------------------------
//
// Device registration
//
// ------------------------------------------------------------------


errno_t		pod_dev_link( struct pod_driver *drv, struct pod_device *dev );	// Report a new available device to the OS kernel
errno_t		pod_dev_unlink( struct pod_driver *drv, struct pod_device *dev );	// Report device to be unavailable
errno_t		pod_dev_event( struct pod_driver *drv, struct pod_device *dev );	// Tell about device event (error? state change?)




// ------------------------------------------------------------------
//
// Threads
//
// ------------------------------------------------------------------

errno_t		pod_kernel_thread_start( pod_thread *tid, void (*thread_func)(void *), void *thread_func_arg );
errno_t		pod_kernel_thread_kill( pod_thread tid );

// TODO threadlets/dpc?

// TODO timers?

// ------------------------------------------------------------------
//
// Sync
//
// ------------------------------------------------------------------



errno_t		pod_kernel_create_mutex( pod_mutex **mutex );		// allocate and init
errno_t		pod_kernel_init_mutex( pod_mutex *mutex );		// init static one
errno_t		pod_kernel_destroy_mutex( pod_mutex *mutex );	

errno_t		pod_kernel_lock_mutex( pod_mutex *mutex );
errno_t		pod_kernel_unlock_mutex( pod_mutex *mutex );


errno_t		pod_kernel_create_cond( pod_cond **cond );		// allocate and init
errno_t		pod_kernel_init_cond( pod_cond *cond );		// init static one
errno_t		pod_kernel_destroy_cond( pod_cond *cond );

errno_t		pod_kernel_wait_cond( pod_cond *cond, pod_mutex *mutex );
errno_t		pod_kernel_signal_cond( pod_cond *cond );


// ------------------------------------------------------------------
//
// Logging and panic
//
// ------------------------------------------------------------------

errno_t		pod_log_print( int loglevel, const char **format, ... );
void 		pod_panic(const char *fmt, ...);


// ------------------------------------------------------------------
//
// Memory and address space
//
// ------------------------------------------------------------------

// TODO size_t

#define pod_malloc	pod_alloc_kheap
#define pod_free	pod_free_kheap


errno_t		pod_alloc_physmem( unsigned int nbytes, physaddr_t *mem );	// NB! Mem size in bytes! Allocates pages. npages * pagesize >= nbytes
errno_t		pod_alloc_vaddr( unsigned int nbytes, void **vaddr );		// NB! Mem size in bytes! Allocates pages. npages * pagesize >= nbytes
errno_t		pod_alloc_kheap( unsigned int nbytes, void **mem );


errno_t		pod_free_physmem( physaddr_t mem );
errno_t		pod_free_kheap( void * );
errno_t		pod_free_vaddr( void * );


#define POD_MAP_FLAG_UNCACHED	(1<<1)

// Allocate x86 low (<1mb) memory - need?
//#define POD_MAP_FLAG_LOWMEM	(1<<16)


errno_t		pod_map_mem( physaddr_t mem, void *vaddr, int flags );
errno_t		pod_unmap_mem( physaddr_t mem, void *vaddr, int flags );


errno_t		pod_wire_mem( void *mem, unsigned int nbytes ); 		// Make sure memory is paged in and non-pageable. Can block! Don't call in pod_rq_start!
errno_t		pod_unwire_mem( void *mem, unsigned int nbytes ); 


// Embox includes
#include <sys/mman.h>
#include <mem/sysmalloc.h>
#include <mem/phymem.h>
#include <errno.h>

// OpenPOD includes
#include <pod_kernel_api.h>

// ------------------------------------------------------------------
//
// OpenPOD binding for Embox OS
//
// Memory and address space
//
// Preliminary, experimental, untested.
//
// ------------------------------------------------------------------



#define BYTES_TO_PAGES(x) ((((x)-1)/PAGE_SIZE)+1)


// NB! Mem size in bytes! Allocates pages. npages * pagesize >= nbytes
errno_t
pod_alloc_physmem( size_t nBytes, physaddr_t *mem )
{
    int nPages = BYTES_TO_PAGES( nBytes );

    void * ret = phymem_alloc( nPages );

    if( ret == 0 ) return ENOMEM;

    *mem = (physaddr_t)ret;
    return 0;
}




errno_t
pod_free_physmem( size_t nBytes, physaddr_t mem )
{
    int nPages = BYTES_TO_PAGES( nBytes );

    phymem_free( mem, nPages );
    return 0;
}

errno_t
pod_free_vaddr( size_t nBytes, void * )
{
    int nPages = BYTES_TO_PAGES( nBytes );
    return ENOMEM; // TODO
}

// NB! Mem size in bytes! Allocates pages. npages * pagesize >= nbytes
errno_t
pod_alloc_vaddr( size_t nBytes, void **vaddr )
{
    int nPages = BYTES_TO_PAGES( nBytes );
    return ENOMEM; // TODO
}


errno_t
pod_alloc_kheap( size_t nBytes, void **mem )
{
    errno_t rc = 0;

    void *ret = syscalloc( 1, nBytes );
    if( ret == 0 ) return ENOMEM;

    *mem = ret;
    return 0;
}

errno_t
pod_free_kheap( void *ptr )
{
    sysfree( ptr );
    return 0;
}



errno_t
pod_map_mem( physaddr_t physmem, void *vaddr, size_t mmap_len, int pod_flags )
{
    int embox_mmap_flags = 0;

    if( pod_flags & POD_MAP_NOCACHE )	embox_mmap_flags |= PROT_NOCACHE;
    if( pod_flags & POD_MAP_READ)	embox_mmap_flags |= PROT_READ;
    if( pod_flags & POD_MAP_WRITE )	embox_mmap_flags |= PROT_WRITE;

    if( MAP_FAILED == mmap_device_memory(vaddr,
                                         mmap_len,
                                         embox_mmap_flags,
                                         0, // MAP_FIXED?
                                         (unsigned long) physmem))
    {
        return EIO;
    }

    return 0;

}

// TODO remove physaddr arg?
errno_t     pod_unmap_mem( physaddr_t mem, void *vaddr, size_t nBytes, int flags )
{
    munmap( vaddr, nBytes );
}


errno_t     pod_wire_mem( void *mem, size_t nbytes )
{
    return 0; // No paging on Embox, right?
}

errno_t     pod_unwire_mem( void *mem, size_t nbytes )
{
    return 0; // No paging on Embox, right?
}



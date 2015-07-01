#include <pod_kernel_api.h>


void *
pod_malloc(size_t ___nb)  
{ 
	void *ret; 
	errno_t rc = pod_alloc_kheap( (___nb), &ret ); 
	if( rc ) return 0; 
	return ret; 
}


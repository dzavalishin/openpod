#ifndef POD_QUEUES_H
#define POD_QUEUES_H

#include "pod_types.h"
#include "pod_rq.h"

//*******************************************************************
//
// OpenPOD
//
// Basic request queue implementation.
//
//*******************************************************************

typedef struct pod_qe 
{

	struct pod_qe		*next;
	struct pod_qe		*prev;

	pod_request		*rq

} pod_qe;


typedef struct pod_q 
{
	pod_qe			*first;
	pod_qe			*last;

	// Is not used by queue management code, outer code can use
	// to connect queue to driver's entry point
	errno_t (*pod_dev_q_exec)( pod_device *dev, pod_request *rq );

} pod_q;



// Create (allocate and init) queue
errno_t		pod_q_construct( pod_q **q );

// Destroy queue
errno_t		pod_q_destruct( pod_q *q );

// Enqueue request
errno_t		pod_q_enqueue( pod_q *q, pod_request *rq );

// Dequeue request - delete given request from queue
errno_t		pod_q_dequeue( pod_q *q, pod_request *rq ); 

// Add fence, all the requests enqueued after will be sorted
// separately and won't mix with previous ones
errno_t		pod_q_fence( pod_q *q, pod_request *rq ); 

// Sort taking fences in account
errno_t		pod_q_sort( pod_q *q, pod_request *rq, int (*cmp)( pod_request *rqa, pod_request *rqb) ); 


// Request prioty compare func., can be used with sort above
int		rq_prio_cmp(pod_request *rqa, pod_request *rqb);


// Dequeue request - return (and delete from q) last request.
// returns ENOENT and sets rq to 0 if q is empty.
errno_t		pod_q_get_last( pod_q *q, pod_request **rq ); 


#endif // POD_QUEUES_H


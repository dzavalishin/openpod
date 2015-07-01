#include <errno.h>

#include <openpod.h>
#include <pod_rq.h>
#include <pod_deffunc.h>

//-------------------------------------------------------------------
//
// Default function to convert async interface (pod_enqueue()) to
// sync interface (pod_dev_method).
//
//-------------------------------------------------------------------

errno_t	pod_default_enqueue( pod_device *dev, pod_request *rq )
{
	errno_t rc;

	rc = pod_dev_method( dev, rq->operation, rq->op_arg );

	if( rc == ENOSYS )	
		rq->err = pod_rq_status_param;
	else if( rc )
		rq->err = pod_rq_status_ioerr;
	else
		rq->err = pod_rq_status_ok;

	if( rq->done ) rq->done( rq );

	return 0;
}

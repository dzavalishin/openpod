#include <errno.h>

#include <openpod.h>
#include <pod_rq.h>
#include <pod_deffunc.h>


// These empty implementations can be used if driver has no request queue and
// supports sync interface only. See pod_default_enqueue for async to sync io
// interface converter.

errno_t	pod_default_dequeue( pod_device *dev, pod_request *rq )
{
	(void) dev;
	(void) rq;

	return ENOENT; // Too late, rq is done already (as we do anything syncronously)
}

errno_t	pod_default_fence( pod_device *dev, pod_request *rq )
{
	(void) dev;
	(void) rq;

	return 0; // We have no q, so all requests are done in order
}

errno_t	pod_default_raise( pod_device *dev, pod_request *rq, uint32_t io_prio )
{
	(void) dev;
	(void) rq;
	(void) io_prio;

	return 0; // It's too late anyway, don't bother caller with error code.
}


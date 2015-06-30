#ifndef POD_DEV_Q_H
#define POD_DEV_Q_H

#include "pod_types.h"
#include "pod_device.h"

//*******************************************************************
//
// OpenPOD
//
// Default implementation of a single request queue.
//
// Part of driver framework. Not part of OpenPOD specification.
//
// This is an implmementation of driver 'frontend' - request entry
// points, request queue, request execution thread, callback
// processing. Implementation is quite conservative and not really
// optimized.
//
//*******************************************************************



errno_t pod_dev_q_construct( pod_device *dev );
errno_t pod_dev_q_destruct( pod_device *dev );
errno_t pod_dev_q_enqueue( pod_device *dev, pod_request *rq );
errno_t pod_dev_q_dequeue( pod_device *dev, pod_request *rq );
errno_t pod_dev_q_fence( pod_device *dev ); 
errno_t pod_dev_q_raise( pod_device *dev, pod_request *rq, uint32_t io_prio );

// должна быть реализована в драйвере, запускает запрос на исполнение 
//errno_t   pod_dev_q_exec( pod_device *dev, pod_request *rq );

// реализована в фреймворке, вызывается кодом драйвера по завершении в/в, можно из прерывания
errno_t pod_dev_q_iodone( pod_device *dev );




#endif // POD_DEV_Q_H

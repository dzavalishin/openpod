#ifndef POD_IO_NET_H
#define POD_IO_NET_H

#include "pod_types.h"
#include "pod_rq.h"

//*******************************************************************
//
// OpenPOD
//
// network IO structs and operation ids
//
//*******************************************************************


// значение поля operation или индекс в таблицу методов class_interface
enum pod_video_operartions 
{
	nop, get_mac_addr,
	mac_send, mac_recv,
	//ip_send, ip_recv, 
};


//*******************************************************************
//
// Struct to fill into the class_specific field of request
//
//*******************************************************************

#define POD_NET_MAX_MAC 16

// get_mac_addr
struct pod_net_rq_mac
{
	uint8_t	mac_len;
	uint8_t	mac[POD_NET_MAX_MAC];
};








#endif // POD_IO_NET_H

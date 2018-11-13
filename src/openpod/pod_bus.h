#include "pod_types.h"

#include <errno.h>

//*******************************************************************
//
// OpenPOD
//
// Default implementations of typical functions.
//
//*******************************************************************



typedef struct pod_bus_pci_dev 
{

	/* normal header stuff */
    u_int16_t vendor_id;
    u_int16_t device_id;
	
    u_int16_t command;
    u_int16_t status;
	
    u_int8_t revision_id;
    u_int8_t interface;
    u_int8_t sub_class;
    u_int8_t base_class;
	
    u_int8_t cache_line_size;
    u_int8_t latency_timer;
    u_int8_t header_type;
    u_int8_t bist;
	
	/* device info */
    u_int8_t bus;
    u_int8_t dev;
    u_int8_t func;
    u_int8_t _pad; // why?

	/* base registers */	
    u_int32_t base[6];
    u_int32_t size[6];
    u_int8_t is_mem[6]; // true if this is memory addr

    u_int32_t interrupt;

} pod_bus_pci_dev;


errno_t pod_bus_pic_find_dev( int pci_vendor_id, int pci_dev_id, pod_bus_pci_dev *pdev );


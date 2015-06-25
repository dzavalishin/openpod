#ifndef POD_DRIVER_H
#define POD_DRIVER_H

#include "pod_properties.h"

//*******************************************************************
//
// OpenPOD
//
// Driver level definitions
//
//*******************************************************************

struct pod_driver;

//-------------------------------------------------------------------
//
// Driver methods, required
//
//-------------------------------------------------------------------

typedef struct {

errno_t		(*pod_construct)( struct pod_driver *drv );
errno_t		(*pod_destruct)( struct pod_driver *drv );


errno_t		(*pod_activate)( struct pod_driver *drv );
errno_t		(*pod_deactivate)( struct pod_driver *drv );


errno_t		(*pod_sense)( struct pod_driver *drv );
//errno_t		(*pod_probe)( struct pod_driver *drv, pod_bus *bus, pod_bus_dev *bdev );

} pod_dev_required_f;

//-------------------------------------------------------------------
//
// Driver methods, optional
//
//-------------------------------------------------------------------


typedef enum 
{ 
	pod_sleep_undefined,	// Do the best you can
	pod_sleep_fast,		// Must take uSeconds to wake
	pod_sleep_norm,		// Must take mSeconds to wake
	pod_sleep_slow,		// Must take seconds to wake
	pod_sleep_forever,	// Can take forever (disk spin) to wake
} pod_sleep_level;


typedef struct {

errno_t		(*pod_get_property)( pod_properties *p, const char *pName, char *pValue, int vlen );
errno_t		(*pod_set_property)( pod_properties *p, const char *pName, const char *pValue );
errno_t		(*pod_list_properties)( pod_properties *p, int nProperty, char *pName, int vlen );


errno_t		(*pod_save_state)( struct pod_driver *drv, char **state_xml );
errno_t		(*pod_load_state)( struct pod_driver *drv, const char *state_xml );

errno_t		(*pod_sleep)( struct pod_driver *drv, pod_sleep_level l );
errno_t		(*pod_awake)( struct pod_driver *drv );

} pod_dev_optional_f;


typedef struct kernel_f
{
} kernel_f;

//-------------------------------------------------------------------
//
// Driver descriptor
//
//-------------------------------------------------------------------

#define POD_DRIVER_MAGIC	0xA601B702
#define POD_DRIVER_VERSION	0, 1
#define POD_DRIVER_ARCH_UNKNOWN	0, 0

typedef struct pod_driver 
{
//	char 			magic[4];	// magic number
	uint32_t		magic;

	// Minor change means API is extended, but compatible
	uint8_t			API_version_major; 
	uint8_t			API_version_minor;	

	// i32, i64, arm32, arm64, mips32, mips64, ...
	uint8_t			arch_major; 
	uint8_t			arch_minor;	// undefined, must be 0

	uint8_t			class_id;
	uint8_t			pad0;
	uint8_t			pad1;
	uint8_t			state_flags; // Driver state - TODO

//uint32_t		capabilities; // tbd.

	char			*name;

	// lifecycle entry points function pointers
	pod_dev_required_f	calls;
	pod_dev_optional_f	*optional;

	pod_properties		*prop;

	// private driverТs data structure
	void			*private_data; 

/**
 * таблица функций €дра дл€ драйвера - используетс€ если нельз€ пролинковать
 * напр€мую (функции замен€ютс€ матросами, которые через эту таблицу идут в
 *  €дро - см. pod_kernel_api.h) 
**/
	kernel_f		*kernel_driver_api;	


} pod_driver;


// Drivar is initialized (constructed)
#define POD_DRV_STATE_INIT		(1<<0)
// Driver hardware search done (pod_sense called)
#define POD_DRV_STATE_FOUND		(1<<1)



errno_t	pod_construct( pod_driver *drv );
errno_t	pod_destruct( pod_driver *drv );

errno_t	pod_activate( pod_driver *drv );
errno_t	pod_deactivate( pod_driver *drv );

errno_t	pod_sense( pod_driver *drv );

#if 0

inline errno_t	pod_construct( pod_driver *drv )
{
	if( (drv == 0) || (drv->calls.pod_construct == 0) )
		return EFAULT;

	return drv->calls.pod_construct( drv );
}
	

inline errno_t	pod_destruct( pod_driver *drv )
{
	if( (drv == 0) || (drv->calls.pod_destruct == 0) )
		return EFAULT;

	return drv->calls.pod_destruct( drv );
}


inline errno_t	pod_activate( pod_driver *drv )
{
	if( (drv == 0) || (drv->calls.pod_activate== 0) )
		return EFAULT;

	return drv->calls.pod_activate( drv );
}


inline errno_t	pod_deactivate( pod_driver *drv )
{
	if( (drv == 0) || (drv->calls.pod_deactivate== 0) )
		return EFAULT;

	return drv->calls.pod_deactivate( drv );
}


inline errno_t	pod_sense( pod_driver *drv )
{
	if( (drv == 0) || (drv->calls.pod_sense== 0) )
		return EFAULT;

	return drv->calls.pod_sense( drv );
}


//inline errno_t	pod_probe( pod_driver *drv, bus?, dev? )

#endif




#endif // POD_DRIVER_H

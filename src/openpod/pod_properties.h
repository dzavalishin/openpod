#ifndef POD_PROPERTIES_H
#define POD_PROPERTIES_H

#include <pod_types.h>

//*******************************************************************
//
// OpenPOD
//
// Driver level definitions
//
//*******************************************************************

typedef enum
{
    pt_int32,
    pt_mstring,         // malloced string
    pt_enum32,         	// enum int32 - unimpl!
} pod_property_type_t;


struct pod_property;

typedef struct pod_properties
{
    //u_int32_t           prefix;         // 4-byte char prefix of this group, like 'dev.', 'gen.' or 'fsp.'
    //const char *          prefix;         // 4-byte char prefix of this group, like 'dev.', 'gen.' or 'fsp.'

    struct pod_property *list;
    size_t              lsize;

    //! Get pointer to property value
    void *              (*valp)(struct pod_properties *ps, void *context, size_t offset );



} pod_properties;



typedef struct pod_property {
    pod_property_type_t type;
    const char 			*name;
    size_t              offset;
    void                *valp;

    char                **val_list; // for enums

    void                (*activate)(struct pod_properties *ps, void *context, size_t offset, void *vp );
    errno_t             (*setf)(struct pod_properties *ps, void *context, size_t offset, void *vp, const char *val);
    // unused yet
    errno_t             (*getf)(struct pod_properties *ps, void *context, size_t offset, void *vp, char *val, size_t len);
} pod_property;



#define PROP_COUNT(__plist) (sizeof(__plist)/sizeof(__plist[0]))



//-------------------------------------------------------------------
//
// Generic property manipulation code
//
//-------------------------------------------------------------------


errno_t	pod_gen_setproperty( pod_properties *ps, void *context, const char *pName, const char *pValue );
errno_t	pod_gen_getproperty( pod_properties *ps, void *context, const char *pName, char *pValue, int vlen );
errno_t pod_gen_listproperties( pod_properties *ps, int nProperty, char *pValue, int vlen );


//-------------------------------------------------------------------
//
// Generic property manipulation code for a driver.
//
// Can be filled into the driver optional methods table.
//
//-------------------------------------------------------------------


struct pod_driver;

errno_t gen_drv_listproperties( struct pod_driver *drv, int nProperty, char *pValue, int vlen );
errno_t	gen_drv_getproperty( struct pod_driver *drv, const char *pName, char *pValue, int vlen );
errno_t	gen_drv_setproperty( struct pod_driver *drv, const char *pName, const char *pValue );





//-------------------------------------------------------------------
//
// Generic property manipulation code for a device.
//
//-------------------------------------------------------------------


struct pod_device;

errno_t gen_dev_listproperties( struct pod_device *dev, int nProperty, char *pValue, int vlen );
errno_t	gen_dev_getproperty( struct pod_device *dev, const char *pName, char *pValue, int vlen );
errno_t	gen_dev_setproperty( struct pod_device *dev, const char *pName, const char *pValue );




#endif // POD_PROPERTIES_H

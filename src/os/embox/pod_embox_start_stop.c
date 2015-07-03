#include <openpod.h>

#include "pod_kernel_globals.h"

// ------------------------------------------------------------------
//
// Start/stop driver
//
// ------------------------------------------------------------------

// List of OpenPOD drivers we know about. Here all the drivers are statically linked,
// but you can build this list dynamically, if your drivers are loadable.

extern pod_driver pod_video_driver_vga, pod_video_driver_bochs_vbe;


pod_driver	*pod_drivers[] = 
{
	&pod_video_driver_vga,
	&pod_video_driver_bochs_vbe,
};

#define POD_NDRIVERS ( sizeof(pod_drivers) / sizeof(pod_driver) )

#define POD_FOREACH( ___list, ___method ) do { int ___i; for( ___i = 0; ___i < POD_NDRIVERS; ___i++ ) ___method( pod_drivers[___i] ); } while(0)

#define POD_DOALL( ___what )			                \
void                                                    \
pod_##___what##_all(void) 				                \
{                                                       \
	POD_FOREACH( pod_drivers, pod_##___what );      	\
}                                                       \

// Call on kernel init/deinit. 
POD_DOALL( construct )
POD_DOALL( destruct )

// When your kernel scans for hardware, call pod_sense_all.

POD_DOALL( sense )



// During graceful shutdown call pod_deactivate_all and pod_destruct_all.
// If driver is dynamically loaded, call pod_destruct for that driver. 

POD_DOALL( deactivate )


// When your kernel is ready to use drivers, call pod_activate_all.
//
// NB. There are two problems with this code.
//
// 1. If preselected video driver fails, we must cycle once again through all
//    the video drives and attempt all others in order. Or, better, have list
//    of video drivers sorted by their preferredness.
//
// 2. Frankly, class_id can be POD_DEV_CLASS_MULTIPLE. if such a driver has 
//    video device, it will be started.


void                 
pod_activate_all(void)
{                    
    int i; 

    for( i = 0; i < POD_NDRIVERS; i++ ) 
    {
        // Start just selected video driver
        if( pod_drivers[i]->class_id == POD_DEV_CLASS_VIDEO )
        {
            // Have no selection for some reason? Attempt first video driver we met
            if( 0 == preselected_video_driver )
                preselected_video_driver = pod_drivers[i];

            if( pod_drivers[i] != preselected_video_driver )
                continue;

            // Video driver actiovation failed? Attempt next one!
            if( pod_activate( pod_drivers[i] ) ) 
                preselected_video_driver = 0;

        }
        else
            pod_activate( pod_drivers[i] ); 
    }
}
























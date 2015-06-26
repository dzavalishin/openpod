#include <openpod.h>

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

// When your kernel is ready to use drivers, call pod_activate_all.

POD_DOALL( activate )

// During graceful shutdown call pod_deactivate_all and pod_destruct_all.
// If driver is dynamically loaded, call pod_destruct for that driver. 

POD_DOALL( deactivate )



/*

void
pod_construct_all(void) __attribute__((destructor))
{
	POD_FOREACH( pod_drivers, pod_construct );
}

void
pod_construct_all(void) __attribute__((constructor))
{
	POD_FOREACH( pod_drivers, pod_construct );
}

void
pod_construct_all(void) __attribute__((destructor))
{
	POD_FOREACH( pod_drivers, pod_construct );
}

*/

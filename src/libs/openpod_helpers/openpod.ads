with Interfaces.C;
with Interfaces.C.Strings;

package OpenPOD is


type Byte is range 0 .. 255;
for Byte'Size use 8;

type Pod_Driver is record
	magic			: Integer;

	-- Minor change means API is extended, but compatible
	API_version_major	: Byte; 
	API_version_minor	: Byte;	

	-- i32, i64, arm32, arm64, mips32, mips64, ...
	arch_major		: Byte;  
	arch_minor		: Byte; 	-- undefined, must be 0

	class_id		: Byte; 
	pad0			: Byte;
	pad1			: Byte;
	state_flags		: Byte;

-- uint32_t		capabilities; // tbd.

	name			: Interfaces.C.Strings.chars_ptr; --access Character;

-- TODO rest

--	// lifecycle entry points function pointers
--	pod_dev_required_f	calls;
--	pod_dev_optional_f	*optional;

--	pod_properties		*prop;

--	// private driver's data structure
--	void			*private_data; 

--	kernel_f		*kernel_driver_api;	

end record;

type Pod_Device is record
	magic		: Integer;

	class_id	: Byte;
	class_flags	: Byte;		-- Class specific flags,
	internal_flags	: Byte;		-- Not exposed as state change to kernel
    state_flags	: Byte;		-- Device state

	name			: Interfaces.C.Strings.chars_ptr; --access Character;

	drv		: access Pod_Driver;

-- TODO rest

--	pod_dev_f			*calls; // dev io entry points

--	pod_properties		*prop;

--	void				(**class_interface)(struct pod_device *dev, void *arg); // dev class specific interface
--	void				*private_data;

--	// ----------------------------------------------------------------------
--	// Fields below are used by default framework code

--	// Request queue, used by pod_dev_q_ functions
--	struct pod_q		*default_r_q;		// default request q
--	pod_request			*curr_rq;		// request we do now
--	pod_thread			rq_run_thread;	// thread used to run requests
--	pod_cond			*rq_run_cond;	// triggered to run next request
--	pod_mutex			*rq_run_mutex;
--	errno_t				(*default_start_rq)( struct pod_device *dev, pod_request *rq ); // start executing an rq

end record;



procedure ada_print_drv( drv : access Pod_Driver );



end OpenPOD;


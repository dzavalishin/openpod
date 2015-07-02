// Empty, to satisfy include from pod_types.h

// Actually this include must bring types such as size_t, uint32_t, etc from host OS
// See pod_types.h for list of types


// Example of possible definition

// #include <sys/errno.h> // defines errno_t;
// #define POD_ERRNO_T_DEFINED


// OR just define it here

// typedef unsigned int      size_t;
// #define POD_SIZE_T_DEFINED

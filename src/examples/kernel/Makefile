include ../../config.mk

# TODO move pod_local_types.h from here and tests to some common place
CFLAGS += -I../../openpod -I.

all:	pseudo_kernel

install::
	# Nothing to install here

clean::
	-rm *.o
	-rm *.a
	-rm *.~c
	-rm *.~h


pseudo_kernel: pod_link_unlink.o pod_process_event.o pod_start_stop.o pod_video_io.o
#	ar crsv $@ *.o
#	cp $@ ..


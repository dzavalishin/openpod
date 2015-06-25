include ../../config.mk

# TODO move pod_local_types.h from here and tests to some common place
CFLAGS += -I../../openpod -I.

all:	libopenpod_helpers.a

install::
	# TODO

clean::
	-rm *.o
	-rm *.a
	-rm *.~c
	-rm *.~h


libopenpod_helpers.a: pod_dev_event.o request.o driver.o
	ar crsv $@ *.o
	cp $@ ..

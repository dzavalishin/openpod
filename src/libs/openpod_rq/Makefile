include ../../config.mk

LIB=libopenpod_rq.a

# TODO move pod_local_types.h from here and tests to some common place
CFLAGS += -I../../openpod -I../..

all:	$(LIB)

install::
	# TODO

clean::
	-rm *.o
	-rm *.a
	-rm *.c~
	-rm *.h~


$(LIB): pod_dev_q.o
	ar crsv $@ *.o
	cp $@ ..

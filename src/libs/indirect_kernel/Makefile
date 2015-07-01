include ../../config.mk

LIB=libopenpod_kernel.a

CFLAGS += -I../../openpod -I../..

all:	$(LIB)

install::
# Nothing

clean::
	-rm *.o
	-rm *.a
	-rm *.~c
	-rm *.~h


$(LIB): link.o
	ar crsv $@ *.o
	cp $@ ..

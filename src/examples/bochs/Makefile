include ../../config.mk

CFLAGS += -I../../openpod -I../..

TARGET=bochs_driver

all: $(TARGET)

install::
	# Nothing to install here

clean::
	-rm *.o
	-rm *.a
	-rm *.~c
	-rm *.~h


$(TARGET): pod_bochs_struct.o pod_bochs.o pod_bochs_util.o
#	ar crsv $@ *.o
#	cp $@ ..


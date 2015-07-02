include ../../config.mk

CFLAGS += -I../../openpod -I../..

all:	libopenpod_helpers.a

install::
# Nothing

clean::
	-rm *.o
	-rm *.a
	-rm *.c~
	-rm *.h~
	-rm *.ad?~
	-rm *.ali

# turned ADA off - openpod.o
libopenpod_helpers.a: pod_dev_event.o request.o driver.o device.o rq_to_sync.o rq_stubs.o properties.o mem.o
	ar crsv $@ *.o
	cp $@ ..


#openpod.o: openpod.adb

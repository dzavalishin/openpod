include ../../config.mk

TARGET=openpod_embox
EMBOX_SRC_ROOT=p:/embox/trunk

CFLAGS += -I../../openpod -I../.. -I$(EMBOX_SRC_ROOT)/src -I$(EMBOX_SRC_ROOT)/src/include

all:	$(TARGET)

install::
	# Nothing to install here

clean::
	-rm *.o
	-rm *.a
	-rm *.c~
	-rm *.h~


$(TARGET): pod_embox_link_unlink.o pod_embox_mem.o pod_embox_process_event.o pod_embox_start_stop.o pod_embox_video_io.o
	ar crsv $@ *.o
#	cp $@ ..



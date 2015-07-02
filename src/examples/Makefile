include ../config.mk

all::
	$(MAKE) -C kernel all
	$(MAKE) -C bochs  all
#	$(MAKE) test

install::
# Nothing

clean::
	$(MAKE) -C kernel clean
	$(MAKE) -C bochs  clean
#	-rm *.a

include config.mk

default::
	@echo make all or make install
	@echo doing make all by default
	$(MAKE) all

install::
	$(MAKE) all
	# TODO
	$(MAKE) -C libs install

all::
	$(MAKE) -C libs all
	$(MAKE) -C test all

clean::
	$(MAKE) -C libs clean
	$(MAKE) -C test clean


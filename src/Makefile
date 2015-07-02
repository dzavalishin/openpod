include config.mk

default::
	@echo make all or make install
	@echo doing make all by default
	$(MAKE) all

install::
	$(MAKE) all
	$(MAKE) -C openpod install
	$(MAKE) -C libs install

all::
	$(MAKE) -C libs all
	$(MAKE) -C examples all
	$(MAKE) -C test all

clean::
	$(MAKE) -C libs clean
	$(MAKE) -C test clean
	$(MAKE) -C examples clean
	$(MAKE) -C openpod clean


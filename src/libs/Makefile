include ../config.mk

all::
	$(MAKE) -C openpod_helpers	all
	$(MAKE) -C indirect_kernel 	all
#	$(MAKE) -C openpod_rq 		all
#	$(MAKE) test

install::
	# TODO

clean::
	$(MAKE) -C openpod_helpers	clean
	$(MAKE) -C indirect_kernel 	clean
#	$(MAKE) -C openpod_rq		clean
	-rm *.a

include ../config.mk

all::
	$(MAKE) -C openpod_helpers	all
	$(MAKE) -C indirect_kernel 	all
	$(MAKE) -C openpod_rq 		all
#	$(MAKE) test

install::
ifdef LIB_INST_DIR
	echo cp *.h $(LIB_INST_DIR)
else
#$(info Please define LIB_INST_DIR in config.mk)
	@echo 
	@echo Please define LIB_INST_DIR in config.mk
	@echo 
	@false
endif

clean::
	$(MAKE) -C openpod_helpers	clean
	$(MAKE) -C indirect_kernel 	clean
	$(MAKE) -C openpod_rq		clean
	-rm *.a

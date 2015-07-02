include ../config.mk

default::
	@echo type 'make install' to install headers

install::
ifdef INCLUDE_INST_DIR
	echo cp *.h $(INCLUDE_INST_DIR)
else
	@echo 
	@echo Please define INCLUDE_INST_DIR in config.mk
	@echo 
	@false
#$(info Please define INCLUDE_INST_DIR in config.mk)
endif

clean::
	-rm *.h~


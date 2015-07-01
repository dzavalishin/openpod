include ../config.mk

all::
	$(MAKE) test

install::
# Nothing

clean::
	cd unit; sh clean.sh; cd ..
	

test::
	cd unit; sh prepare.sh; sh test.sh cd ..

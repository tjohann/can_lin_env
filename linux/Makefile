#
# my simple makefile act as something like a user interface
#

MODULES = $(shell ls -d */ | cut -f1 -d '/')
PWD=$(shell pwd)

all: libhelper libcan can_send can_recv liblin lin_send lin_recv

libhelper:
	@echo "+------------------------------------------------------------+"
	@echo "|   build $@ in $(PWD)"
	@echo "+------------------------------------------------------------+"
	(cd $@ && $(MAKE))

libcan:
	@echo "+------------------------------------------------------------+"
	@echo "|   build $@ in $(PWD)"
	@echo "+------------------------------------------------------------+"
	(cd $@ && $(MAKE))

can_send:
	@echo "+------------------------------------------------------------+"
	@echo "|   build $@ in $(PWD)"
	@echo "+------------------------------------------------------------+"
	(cd $@ && $(MAKE))

can_recv:
	@echo "+------------------------------------------------------------+"
	@echo "|   build $@ in $(PWD)"
	@echo "+------------------------------------------------------------+"
	(cd $@ && $(MAKE))

liblin:
	@echo "+------------------------------------------------------------+"
	@echo "|   build $@ in $(PWD)"
	@echo "+------------------------------------------------------------+"
	(cd $@ && $(MAKE))

lin_send:
	@echo "+------------------------------------------------------------+"
	@echo "|   build $@ in $(PWD)"
	@echo "+------------------------------------------------------------+"
	(cd $@ && $(MAKE))

lin_recv:
	@echo "+------------------------------------------------------------+"
	@echo "|   build $@ in $(PWD)"
	@echo "+------------------------------------------------------------+"
	(cd $@ && $(MAKE))

install: all
	for dir in $(MODULES); do (cd $$dir && $(MAKE) $@); done

uninstall:
	for dir in $(MODULES); do (cd $$dir && $(MAKE) $@); done

clean:
	rm -f *~ .*~
	for dir in $(MODULES); do (cd $$dir && $(MAKE) $@); done

distclean: clean

setup_vcan:
	(../scripts/vcan_init.sh)

delete_vcan:
	(../scripts/vcan_delete.sh)

rmmod_vcan:
	(../scripts/vcan_rmmod.sh)

.PHONY: clean libhelper libcan can_send can_recv liblin lin_send lin_recv uninstall setup_vcan delete_vcab rmmod_vcan

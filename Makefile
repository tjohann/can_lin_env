#
# my simple makefile act as something like a user interface
#

MODULES = $(shell ls -d */ | cut -f1 -d'/')
BUILD = linux bare_metal hypervisor

all::
	@echo "  +----------------------------------------------------------+"
	@echo " /                                                          /|"
	@echo "+----------------------------------------------------------+ |"
	@echo "| Usage:                                                   | |"
	@echo "| make build     -> build everthing                        | |"
	@echo "|                                                          | |"
	@echo "| make install   -> build_all + install (driver + examples)| |"
	@echo "| make uninstall -> uninstall all                          | |"
	@echo "|                                                          | |"
	@echo "| make set_caps  -> set file capabilities (run as non-root)| |"
	@echo "|                                                          | |"
	@echo "| make setup_vcan  -> setup vcan0                          | |"
	@echo "| make delete_vcan -> delete vcan0 node                    | |"
	@echo "| make rmmod_vcan  -> unload vcan kernel driver            | |"
	@echo "|                                                          | |"
	@echo "| make get_external_repos -> get ALL git repos from below  | |"
	@echo "| make get_can-utils      -> clone can-utils               | |"
	@echo "| make get_jailhouse      -> clone jailhouse hypervisor    | |"
	@echo "| make get_lin            -> clone lin kernel driver       | |"
	@echo "|                                                          | |"
	@echo "| make clean     -> clean all dir/subdirs                  | +"
	@echo "| make distclean -> complete cleanup                       |/ "
	@echo "+----------------------------------------------------------+  "

build::
	for dir in $(BUILD); do (cd $$dir && $(MAKE) all); done

clean::
	rm -f *~ .*~
	for dir in $(MODULES); do (cd $$dir && $(MAKE) $@); done

distclean::
	rm -f *~ .*~
	for dir in $(MODULES); do (cd $$dir && $(MAKE) $@); done

install::
	for dir in $(BUILD); do (cd $$dir && $(MAKE) $@); done
	(./set_file_caps.sh)

uninstall::
	for dir in $(BUILD); do (cd $$dir && $(MAKE) $@); done

set_caps::
	@echo "Set caps to all binary"
	(./set_file_caps.sh)

setup_vcan::
	(./scripts/vcan_init.sh)

delete_vcan::
	(./scripts/vcan_delete.sh)

rmmod_vcan::
	(./scripts/vcan_rmmod.sh)

get_external_repos::
	(./scripts/get_external_git_repos.sh -p "https")

get_can-utils::
	(./scripts/get_external_git_repos.sh -r "can-utils" -p "https")

get_jailhouse::
	(./scripts/get_external_git_repos.sh -r "jailhouse" -p "https")

get_lin::
	(../scripts/get_external_git_repos.sh -r "lin" -p "git")


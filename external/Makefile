#
# my simple makefile act as something like a user interface
#


MODULES = $(shell ls -d */ | cut -f1 -d'/')

all::
	@echo "  +----------------------------------------------------------+"
	@echo " /                                                          /|"
	@echo "+----------------------------------------------------------+ |"
	@echo "| Usage:                                                   | |"
	@echo "| make get_external_repos -> get ALL git repos like u-boot | |"
	@echo "| make update_all         -> git pull in all subdir        | |"
	@echo "|                                                          | |"
	@echo "| make get_can-utils      -> clone can-utils               | |"
	@echo "| make get_jailhouse      -> clone jailhouse hypervisor    | |"
	@echo "| make get_lin            -> clone lin kernel driver       | |"
	@echo "|                                                          | |"
	@echo "| make clean              -> clean all dir/subdirs         | |"
	@echo "| make distclean          -> complete cleanup/delete       |/"
	@echo "+----------------------------------------------------------+"

clean::
	rm -f *~ *.o .*~
	for dir in $(MODULES); do (cd $$dir/ && $(MAKE) $@); done

distclean: clean
	rm -rf can-utils
	rm -rf jailhouse
	rm -rf linux-lin

update_all::
	for dir in $(MODULES); do (cd $$dir && git pull); done

get_external_repos::
	(../scripts/get_external_git_repos.sh -p "git")

get_can-utils::
	(../scripts/get_external_git_repos.sh -r "can-utils" -p "https")

get_jailhouse::
	(../scripts/get_external_git_repos.sh -r "jailhouse" -p "https")

get_lin::
	(../scripts/get_external_git_repos.sh -r "lin" -p "git")

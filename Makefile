#
# Entry Makefile of command `make`
# 

include ./Makefile-config

DIR_PROJ_ROOT := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
DIR_MAKE_FILE := $(dir $(lastword $(MAKEFILE_LIST)))

# recursively compile subfolders
subdirs = app socket test interview/akuna interview/belvedere interview/drw interview/fitch
targets = all clean

$(targets):
	@for subdir in $(subdirs); do cd $$subdir && echo "\nEntering dir \"$$subdir\"" && ${MAKE} $@; cd $(DIR_PROJ_ROOT); done

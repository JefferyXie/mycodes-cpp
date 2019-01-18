include ./Makefile-config

DIR_PROJ_ROOT := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
DIR_MAKE_FILE := $(dir $(lastword $(MAKEFILE_LIST)))

# recursively compile subfolders
#subdirs = main socket akuna fitch # TODO: cleanup socket directory!!
subdirs = main interview/akuna interview/fitch interview/hrt interview/optiver interview/volant misc
targets = all clean

$(targets):
	@for dir in $(subdirs); do cd $$dir && ${MAKE} $@; cd $(DIR_PROJ_ROOT); done
#	@for dir in $(subdirs); do cd $$dir && ${MAKE} $@ && cd .. ; done # why doesn't this work??


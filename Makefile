include ./Makefile-config

# recursively compile subfolders
#subdirs = main socket akuna fitch # TODO: cleanup socket directory!!
subdirs = main akuna fitch
targets = all clean

$(targets):
	@for dir in $(subdirs); do cd $$dir && ${MAKE} $@; cd ..; done
#	@for dir in $(subdirs); do cd $$dir && ${MAKE} $@ && cd .. ; done # why doesn't this work??



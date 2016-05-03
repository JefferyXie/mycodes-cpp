include ./Makefile-config

# recursively compile subfolders
subdirs = main XSocket akuna fitch
targets = all clean

$(targets):
	@for dir in $(subdirs); do cd $$dir && ${MAKE} $@; cd ..; done
#	@for dir in $(subdirs); do cd $$dir && ${MAKE} $@ && cd .. ; done # why doesn't this work??



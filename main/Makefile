include ../Makefile-config

# http://stackoverflow.com/questions/9253200/undefined-reference-to-pthread-create
# when linking object files, add dependent library params after object files
$(EXE): $(OBJ_FILES)
	$(CC) $< -o $@ $(CFLAGS_DEBUG) $(INCLUDES)

#$(OBJ_FILES):: $(CPP_FILES)
#	$(CC) $(CFLAGS_DEBUG) $(INCLUDES) -c $< -o $@
# below lines are same as above rule
.cpp.o:
	$(CC) $(CFLAGS_DEBUG) $(INCLUDES) -c $< -o $@

#%.o: $(CPP_FILES) $(H_FILES)
#	$(CC) $(CFLAGS) -c -o $@ $< 

# what is used for?
all: $(EXE)

clean:
	rm $(OBJ_FILES) $(EXE)


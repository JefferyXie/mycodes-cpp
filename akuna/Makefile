include ../Makefile-config

#object path
OBJ_PATH = ./

#The definitions for main folder object files
CPP_FILES:= $(wildcard   ./*.cpp)
#OBJ_FILES:=$(patsubst  ./%.cpp, $(OBJ_PATH)/%.o, $(CPP_FILES))

# compiler settings
CFLAGS = $(C11) -w -lpthread # disable warning

$(EXE): $(OBJ_FILES)
	$(CC) $^ -o $@ $(CFLAGS_DEBUG) 
#	$(CC) $< -o $@ $(CFLAGS_DEBUG) # this won't work, check what '$<' means?

#$(OBJ_FILES): $(CPP_FILES) # why doesn't this work? how to utilize $(OBJ_FILES)?
$(OBJ_FILES): $(OBJ_PATH)/%.o:./%.cpp
	$(CC) $(CFLAGS_DEBUG) -c $< -o $@

# what is used for?
all: $(EXE) 

clean:
	rm $(OBJ_PATH)/*.o $(OBJ_PATH)/*.a $(OBJ_PATH)/*.out $(EXE)


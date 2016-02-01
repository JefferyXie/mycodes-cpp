# makefile - main.cpp

EXE = main_exe
# related files
#CPP_FILES = main.cpp sizeofClass.cpp
CPP_FILES = main.cpp 
OBJ_FILES = $(CPP_FILES:%.cpp=%.o)
H_FILES = *.h # header.h utility.h node.h 
LIB_DIR = ./lib 
SRC_DIR = ./ 
INCLUDES = -I/opt/gtest/include

# compiler settings
CC = g++
C11 = -std=c++0x 
#CFLAGS = -Wall -W -lpthread $(C11)
CFLAGS = $(C11) -w -L/opt/gtest/lib -lgtest -lgtest_main -lpthread # disable warning
CFLAGS_DEBUG = -g $(CFLAGS) 

# http://stackoverflow.com/questions/9253200/undefined-reference-to-pthread-create
# when linking object files, add dependent library params after object files
$(EXE): $(OBJ_FILES)
	$(CC) $< -o $@ $(CFLAGS_DEBUG) $(INCLUDES)
#	echo $(OBJ_FILES)

#$(OBJ_FILES): $(CPP_FILES)
#	$(CC) $(CFLAGS) -c  
# below lines are same as above rule
.cpp.o:
	$(CC) $(CFLAGS_DEBUG) $(INCLUDES) -c $< -o $@

#%.o: $(CPP_FILES) $(H_FILES)
#	$(CC) $(CFLAGS) -c -o $@ $< 

clean: 
	rm $(OBJ_FILES) $(EXE)


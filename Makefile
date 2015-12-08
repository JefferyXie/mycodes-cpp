CC=g++

#Compile options
#Point to local dependency folder
#CXXFLAGS = -c -O3 -std=c++11 -w -D__TRACE_PROF__
#CXXFLAGS = -c -g -std=c++11 -w -D__TRACE_PROF__
CXXFLAGS = -g -std=c++11 -w -D__TRACE_PROF__
#Macros
#Macros=-D _UNICODE -D UNICODE

#object path
OBJ_PATH = ./

#The definitions for main folder object files
MainSrc= $(wildcard   ./*.cpp)
MainObject=$(patsubst  ./%.cpp, $(OBJ_PATH)/%.o, $(MainSrc))

$(MainObject):$(OBJ_PATH)/%.o:./%.cpp
	$(CC) $(CXXFLAGS) -o $@ $<
#	$(CC) $(CXXFLAGS) -o $@ $<

clean:
	rm $(OBJ_PATH)/*.o $(OBJ_PATH)/*.a $(OBJ_PATH)/*.out

mkdir:
	mkdir -p $(OBJ_PATH)


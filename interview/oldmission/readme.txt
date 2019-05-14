
Author: Jeffery Xie (jeffery.xie@gmail.com)
Date: 05/06/2019

1) Compile the program under linux, the code works well under my CentOS 7
> g++ -g -std=c++14 main.cpp -o main_exe

2) Get help
> ./main_exe -h

3) Run program with all default parameters
> ./main_exe

4) Code structure is flat and straight forward
main.cpp:   main function file, parse cmd paras in a simple way and call
            strategy.h to run
header.h:   common included files
messages.h: messages definition and utility to_string methods
tcp_conn.h: simple wrapper of tcp connection
feed.h:     read market data by using tcp_conn.h
strategy.h: strategy implementation, execute in a single thread

5) TODO
- simplify the log output
- response to client action, i.e. client changes paras without stop program
- use multi-thread, such as put feed in a separate thread? Probably no, this
  single thread design can handle the execution in a much cleaner & efficient
  way.

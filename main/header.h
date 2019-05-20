#ifndef HEADER_H
#define HEADER_H

#ifdef __APPLE__
#include <mach/error.h>
#else
#include <error.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// head files for socket
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>

// head files for posix thread
#include <pthread.h>
#include <semaphore.h>

// head files for std
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <stack>
#include <vector>
#include <list>
#include <queue>
#include <iterator>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <algorithm>
#include <typeinfo>
#include <assert.h>
#include <utility>
#include <cstring>
#include <cstdio>
#include <type_traits>
#include <random>
#include <functional>
#include <iomanip>
#include <cmath>
#include <tuple>
#include <regex>
#include <ctime>
#include <chrono>
#include <ratio>
#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <condition_variable>
#include <climits>

using namespace std;


#define ERROR       -1
#define MAX_CLIENTS 2
#define MAX_DATA    1024


#endif

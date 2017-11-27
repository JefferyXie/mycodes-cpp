#ifndef GAUGE_TIME_H
#define GAUGE_TIME_H

#include "../main/header.h"

// 
// http://www.geeksforgeeks.org/how-to-measure-time-taken-by-a-program-in-c/
// http://www.geeksforgeeks.org/how-to-find-time-taken-by-a-program-on-linux-shell/
// https://service.futurequest.net/index.php?/Knowledgebase/Article/View/407/0/difference-between-cpu-time-and-wall-time
// 
// clock(): compute CPU time used
// command time: shows breakdown of real-time, user CPU time, and kernel CPU time
// 
// https://stackoverflow.com/questions/12244153/behaviour-of-clocks-per-sec-in-different-operating-systems
// CLOCKS_PER_SEC: is implementation specific, and can be anything. Posix
//                 requires it to be 1,000,000. It just indicates the units
//                 returned by clock().
 

constexpr long int fib(int n)
{
    return (n <= 1)? n : fib(n-1) + fib(n-2);
}
 
// > time ./a.out
void
run_gauge_time()
{
    cout << "CLOCKS_PER_SEC: " << CLOCKS_PER_SEC << endl;

    clock_t t;
    {
        t = clock();

        // value of res1 is computed at compile time
        const long int res1 = fib(30);
        cout << res1 << endl;

        t = clock() - t;
        double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
     
        printf("fib(30) took %f seconds to execute \n", time_taken);
    }

    {
        t = clock();

        // value of res2 is computed at run time
        long int res2 = fib(30);
        cout << res2 << endl;

        t = clock() - t;
        double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
     
        printf("fib(30) took %f seconds to execute \n", time_taken);
    }
}


#endif

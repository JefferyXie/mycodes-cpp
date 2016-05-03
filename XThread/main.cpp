#include "../main/header.h"
#include "xposix.h"
#include "xfork.h"
#include "mutex_spinlock.h"
#include "memoryorder.h"
#include "join_detach.h"

int main(int argc, char* argv[])
{
    Run_join_detach();
    //Run_memoryorder();
    //Run_mutex_spinlock();
/*    if (argc != 2)
    {
        fprintf(stderr, "usage: <integer value>\n");
        exit(-1);
    }
    if (atoi(argv[1] < 0))
    {
        fprintf(stderr, "argument %d must be non-negative\n", atoi(argv[1]));
        exit(-1);
    }

    int num = atoi(argv[1]);
    Run_posix(num);
*/
    return 0;
}


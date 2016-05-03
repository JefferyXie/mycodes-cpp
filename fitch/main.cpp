#include "./include.h"
#include "./SearchWorker.h"
#include "./unittest.h"

int main(int argc, char** argv) {
    if (argc == 2 && string("test") == string(argv[1])) {
        UNITTEST_RUN(proximitysearch);
        return 0;
    }
    if (argc != 5) return -1;

#ifndef _MSC_VER // for visual studio
    clock_t begin = clock();
#endif
    int count = 0;
    try {
        string w1 = argv[1];
        string w2 = argv[2];
        int range = atoi(argv[3]);
        string file = argv[4];
        SearchWorker o;
        count = o.RunSearch(file, w1, w2, range);
    } catch (...) {
        cout << "invalid paramters." << endl;
    }
    cout << count << endl;
#ifndef _MSC_VER // for visual studio
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    //cout << "time used (s): " << elapsed_secs << endl;
#endif
    return 0;
}


#include "./include.h"
#include "./Protocol.h"
#include "./OrderHandler.h"

//int main(int argc, char** argv) {
int main(int argc, char** argv) {
    if (argc < 2) return -1;

    OrderHandler handler(argv[1]);
    if (!handler.Parse()) return -1;

    string result = handler.AnswerQ1() + ", " +
                    handler.AnswerQ2() + ", " +
                    handler.AnswerQ3() + ", " +
                    handler.AnswerQ4() + ", " +
                    handler.AnswerQ5();
    cout << result << endl;

    return 0;
}



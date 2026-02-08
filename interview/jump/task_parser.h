#include <iostream>
#include <string>
#include <vector>
#include "../../core/util.h"

struct FuncArgs {
    std::string func;
    std::string args;
};
using Frame = std::vector<FuncArgs>;

inline void parse(const std::vector<std::string>& tokens, std::vector<Frame>& stk)
{
    // Parse tokens into a useful stack structure.
    // All parallel tasks should live in the same stack Frame

    // TODO: I am not sure what the issue is, but I know
    // that this stack is not being populated entirely as intended

    // Initial stack frame
    stk.push_back(Frame());

    for (size_t i = 0; i < tokens.size(); ++i) {
        const std::string& tok = tokens[i];
        // New stack frame
        // if (tok == "," || tok == ";") { // Jeffery: don't add new Frame for ";"
        if (tok == ",") {
            stk.push_back(Frame());
        }
        // Jeffery: case 2.b
        else if (tok == "}") {
            const auto& prevTok = tokens[i - 1];
            if (prevTok == ";") {
                // add empty function to the frame
                Frame&   frame = stk.back();
                FuncArgs fa;
                frame.push_back(fa);
            }
        }
        // Tokens within (...) are args that should be kept together
        else if (tok == "(") {
            std::string args;
            while (tokens[++i] != ")") {
                args += tokens[i];
            }
            Frame&    frame = stk.back();
            FuncArgs& fa    = frame.back();
            fa.args         = args;
        }
        // Every other non separator token is a func
        // else if (tok != "{" && tok != "}" && tok != ")") {
        else if (tok != "{" && tok != "}" && tok != ")" && tok != ";") {    // Jeffery: ignore ";"
            Frame&   frame = stk.back();
            FuncArgs fa;
            fa.func = tok;
            frame.push_back(fa);
        }
    }
}

inline void
eval(const std::vector<Frame>& stk, const std::string& partial, size_t index, std::vector<std::string>& expanded)
{
    // Evaluate the stack to expand all function compositions

    // Base case: done building expansion, save result and return
    if (index == stk.size()) {
        expanded.push_back(partial);
        return;
    }

    // Recursive case: build each current FuncArgs around partial and recurse
    const Frame& frame = stk[index];
    for (const FuncArgs& fa : frame) {
        // TODO: Hmmm, I am not sure how to handle args here
        std::string newPartial = fa.func + "(" + partial + ")";

        // Jeffery: case 2.b
        if (fa.func.empty()) {
            newPartial = partial;
        } else {
            // regular args case
            newPartial = fa.func + "(" + partial;
            if (!fa.args.empty()) {
                newPartial += ",";
                newPartial += fa.args;
            }
            newPartial += ")";
        }

        eval(stk, newPartial, index + 1, expanded);
    }
}

inline std::string do_solution(std::string& spec)
{
    // Split specification into tokens
    auto tokens = util::split_any(",;(){}", spec);

    // Parse tokens into a useful stack structure.
    std::vector<Frame> stk;
    parse(tokens, stk);

    // Evaluate the stack to expand all function compositions
    std::vector<std::string> expanded;
    eval(stk, "input", 0, expanded);

    // Combine results into single new line delimited string
    std::string result;
    for (const auto& e : expanded) {
        result += "\n" + e;
    }
    return result;
}

inline void run_task_parser()
{
    for (std::string spec : {
             "task1,task2,task3",                      // task3(task2(task1(input)))
             "task1,{task2;task3}",                    // task2(task1(input)), task3(task1(input))
             "task1,task2,{task3;}",                   // task3(task2(task1(input))), task2(task1(input))
             "task1('b',kw=1),task2(1,2,var1='a')",    // task2(task1(input,'b',kw=1),1,2,var1='a')
             "{func1;func2},{func3;func4}"             // func3(func1(input)), func4(func1(input)), func3(func2(input)),
                                                       // func4(func2(input))
         }) {
        std::cout << "\n--------------------------" << std::endl;
        std::cout << "Spec: " << spec;
        std::cout << do_solution(spec) << std::endl;
    }
}


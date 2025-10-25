#pragma once

#include "../main/node.h"
#include "../main/utility.h"

// There are two types of persons:
// Good person: The person who always tells the truth.
// Bad person:  The person who might tell the truth and might lie.
//
// You are given 2 arguments:
// statementCounts[]: an array tells the number of statements a person will make.
// statements[][]:    a 2D array that describes statements of each person (corresponding to statementCounts[i]).
// statements[i]:     represents a statement made by person (i+1) that person statements[i][0] is a bad person
//                    (statements[i][1] == 0) or a good person (statements[i][1] == 1).
//
// Return the maximum number of people who can be good based on the statements made by the n people.
//
// This is same question as leetcode 2151: https://leetcode.com/problems/maximum-good-people-based-on-statements/
int good_or_bad(const std::vector<int>& statementCounts, const std::vector<std::vector<int>>& statements)
{
    const int peopleCount = statementCounts.size();

    // given specific mask value, find the number of good person in terms of statements
    // this mask value contains information of good person
    // e.g.
    // 3  > 11:    tells 0-based index 0 and 1 are good
    // 18 > 10010: tells 0-based index 1 and 4 are good
    auto impl = [&](int64_t mask) {
        int goodCount    = 0;
        int statementIdx = 0;
        for (int i = 0; i < peopleCount; ++i) {
            // ignore this person if it is not good since its statements are not reliable
            if (!((mask >> i) & 0x01)) {
                // NOTICE: this @statementIdx is very confusing, this code depends on the description that
                // @statements[all_people_statements][2], where @statements[i][0] tells the target person index and
                // @statements[i][1] says good or bad person, the 'i' should be deduced from @statementCounts[]
                statementIdx += statementCounts[i];
                continue;
            }

            // person i is good, so all its statements must be correct
            for (int j = 0; j < statementCounts[i]; ++j) {
                const auto& statement = statements[statementIdx++];
                const auto  personIdx = statement[0];
                const auto  isGood    = statement[1];
                // ignore mask if current statement value not match the expected
                if (isGood != ((mask >> (personIdx - 1)) & 0x01)) {
                    return 0;
                }
            }
            // count person i as a good person
            ++goodCount;
        }
        // this count should be the number of 1 exists in @mask
        return goodCount;
    };

    const int64_t maxMask = (1 << peopleCount);
    int           result  = 0;
    int64_t       bitMask = 1;
    while (bitMask <= maxMask) {
        result = std::max(result, impl(bitMask));
        ++bitMask;
    }
    return result;
}
void run_good_or_bad()
{
    {
        std::vector<int>              statementCounts = {1, 1};
        std::vector<std::vector<int>> statements      = {
            {2, 0},
            {1, 0},
        };    // 1
        std::cout << good_or_bad(statementCounts, statements) << std::endl;
    }
    {
        std::vector<int>              statementCounts = {1, 1, 1};
        std::vector<std::vector<int>> statements      = {
            {2, 1},
            {1, 1},
            {2, 0},
        };    // 2
        std::cout << good_or_bad(statementCounts, statements) << std::endl;
    }
    {
        std::vector<int>              statementCounts = {2, 3, 4, 1, 2};
        std::vector<std::vector<int>> statements      = {
            {3, 1}, {2, 1}, {1, 0}, {3, 0}, {5, 1}, {5, 1}, {1, 0}, {2, 1}, {4, 1}, {3, 1}, {3, 0}, {1, 0},
        };    // 2
        std::cout << good_or_bad(statementCounts, statements) << std::endl;
    }
    {
        std::vector<int>              statementCounts = {2, 2, 2};
        std::vector<std::vector<int>> statements      = {
            {2, 1}, {3, 0}, {3, 1}, {1, 0}, {1, 1}, {2, 0},
        };    // 0
        std::cout << good_or_bad(statementCounts, statements) << std::endl;
    }
}

//
// You are given 6 dominoes. A domino has 2 halves each with a number of spots. You are building a 3-level pyramid of
// dominoes. The bottom level has 3 dominoes, the middle level has 2, and the top has 1.
//
// The arrangement is such that each level is positioned over the center of the level below it. Here is a visual:
//
//          [ 3 | 4 ]
//     [ 2 | 3 ] [ 4 | 5 ]
// [ 1 | 2 ][ 3 | 4 ][ 5 | 6 ]
//
// The pyramid must be set up such that the number of spots on each domino half should be the same as the number on the
// half beneath it. This doesn't apply to neighboring dominoes on the same level.
//
bool validity_pyramid_dominoes(const std::vector<std::array<int, 2>>& dominoes)
{
    // [smaller, bigger]
    using dominoe_key_t = std::pair<int, int>;
    // [freq, dominoe key index]
    using outcome_t = std::pair<int, int>;

    std::unordered_map<dominoe_key_t, outcome_t, boost::hash<dominoe_key_t>> outcomes_by_key;
    for (size_t i = 0; i < dominoes.size(); ++i) {
        const auto key =
            (dominoes[i][0] < dominoes[i][1] ? std::make_pair(dominoes[i][0], dominoes[i][1])
                                             : std::make_pair(dominoes[i][1], dominoes[i][0]));
        auto& record = outcomes_by_key[key];

        const int prev_dominoe_idx = record.second;
        record.first += 1;
        record.second = i;

        // find two equivalent dominoes; same dominoe may happen more than twice, it's enough to dig into once
        if (record.first != 2) {
            continue;
        }

        std::vector<bool> visited(dominoes.size(), false);

        // mark the equivalent dominoes - current and first
        visited[i]                = true;
        visited[prev_dominoe_idx] = true;

        // given a value pair, find two not-yet-visited dominoes whose elements is equal to one of the pair repectively
        using lambda_t       = std::function<bool(const std::array<int, 2>& target)>;
        lambda_t find_2_doms = [&](const std::array<int, 2>& target) {
            int        visited_count = 0;
            const auto x             = target[0];
            const auto y             = target[1];
            // idx of all dominoes that contain x
            std::vector<int> contain_x_idx_all;
            // idx of all dominoes that contain y
            std::vector<int> contain_y_idx_all;
            for (size_t j = 0; j < dominoes.size(); ++j) {
                if (visited[j]) {
                    ++visited_count;
                    continue;
                }

                const auto d = dominoes[j];
                if (x == d[0] || x == d[1]) {
                    contain_x_idx_all.push_back(j);
                }
                if (y == d[0] || y == d[1]) {
                    contain_y_idx_all.push_back(j);
                }
            }

            for (auto x_idx : contain_x_idx_all) {
                for (auto y_idx : contain_y_idx_all) {
                    if (y_idx == x_idx) {
                        continue;
                    }

                    // don't need to handle the last 2 since they don't matter, this is the ending point!
                    if (visited_count + 2 == dominoes.size()) {
                        return true;
                    }

                    visited[x_idx] = true;
                    visited[y_idx] = true;

                    std::array<int, 2> new_target = {
                        x == dominoes[x_idx][0] ? dominoes[x_idx][1] : dominoes[x_idx][0],
                        y == dominoes[y_idx][0] ? dominoes[y_idx][1] : dominoes[y_idx][0],
                    };

                    if (find_2_doms(new_target)) {
                        return true;
                    }

                    visited[x_idx] = false;
                    visited[y_idx] = false;
                }
            }
            return false;
        };

        if (find_2_doms(dominoes[i])) {
            return true;
        }
    }
    return false;
}

void run_validity_pyramid_dominoes()
{
    using dominoes_t = std::vector<std::array<int, 2>>;
    for (const auto& arr : {
             /*
                     [1, 1]
                  [1, 1][1, 1]
               [1, 1][1, 1][1, 1]
             */
             dominoes_t{{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}},    // true
             /*
                     [1, 1]
                  [1, 1][1, 1]
               [5, 1][1, 1][1, 5]
             */
             dominoes_t{{1, 1}, {1, 1}, {1, 1}, {1, 5}, {1, 1}, {1, 5}},    // true
             /*
                     [3, 4]
                  [2, 3][4, 5]
               [1, 2][3, 4][5, 6]
             */
             dominoes_t{{3, 4}, {2, 3}, {4, 5}, {1, 2}, {3, 4}, {5, 6}},    // true
             dominoes_t{{2, 3}, {4, 5}, {3, 4}, {1, 2}, {3, 4}, {5, 6}},    // true
             dominoes_t{{2, 3}, {4, 5}, {1, 2}, {3, 4}, {5, 6}, {3, 4}},    // true
             dominoes_t{{1, 2}, {3, 4}, {5, 6}, {3, 4}, {2, 3}, {4, 5}},    // true
             /*
                     [3, 4]
                  [5, 3][4, 2]
               [6, 5][3, 4][2, 1]
             */
             dominoes_t{{3, 4}, {2, 4}, {3, 5}, {1, 2}, {3, 4}, {5, 6}},    // true

             dominoes_t{{3, 5}, {2, 3}, {4, 5}, {1, 2}, {3, 4}, {5, 6}},    // false
             dominoes_t{{3, 4}, {3, 3}, {4, 5}, {1, 2}, {3, 4}, {5, 6}},    // false
             dominoes_t{{3, 4}, {2, 3}, {4, 5}, {1, 2}, {3, 4}, {3, 6}},    // false
         }) {
        std::cout << std::boolalpha << validity_pyramid_dominoes(arr) << std::endl;
    }
}

void addToken(std::string token, std::vector<std::string>& tokens)
{
    // Trim leading whitespace
    if (!token.empty()) {
        size_t start = token.find_first_not_of(" ");
        token        = start != std::string::npos ? token.substr(start) : "";
    }

    // Do not add empty tokens
    if (token.empty()) {
        return;
    }
    tokens.push_back(token);
}

void tokenize(const std::string& spec, const std::string& sep, std::vector<std::string>& tokens)
{
    // Split specification into tokens

    // Loop over every char in specification.
    size_t start = 0;
    for (size_t end = 0; end < spec.size(); ++end) {
        // If current char is a separator, then add the
        // current token and separator to the tokens list
        if (sep.find(spec[end]) != std::string::npos) {
            addToken(spec.substr(start, end - start), tokens);
            addToken(spec.substr(end, 1), tokens);
            start = end + 1;
        }
    }
    // Add the last token to tokens list
    addToken(spec.substr(start), tokens);
}

struct FuncArgs {
    std::string func;
    std::string args;
};
typedef std::vector<FuncArgs> Frame;

void parse(const std::vector<std::string>& tokens, std::vector<Frame>& stack)
{
    // Parse tokens into a useful stack structure.
    // All parallel tasks should live in the same stack Frame

    // TODO: I am not sure what the issue is, but I know
    // that this stack is not being populated entirely as intended

    // Initial stack frame
    stack.push_back(Frame());

    for (size_t i = 0; i < tokens.size(); ++i) {
        const std::string& tok = tokens[i];
        // New stack frame
        // if (tok == "," || tok == ";") { // Jeffery: don't add new Frame for ";"
        if (tok == ",") {
            stack.push_back(Frame());
        }
        // Jeffery: case 2.b
        else if (tok == "}") {
            const auto& prevTok = tokens[i - 1];
            if (prevTok == ";") {
                // add empty function to the frame
                Frame&   frame = stack.back();
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
            Frame&    frame = stack.back();
            FuncArgs& fa    = frame.back();
            fa.args         = args;
        }
        // Every other non separator token is a func
        // else if (tok != "{" && tok != "}" && tok != ")") {
        else if (tok != "{" && tok != "}" && tok != ")" && tok != ";") {    // Jeffery: ignore ";"
            Frame&   frame = stack.back();
            FuncArgs fa;
            fa.func = tok;
            frame.push_back(fa);
        }
    }
}

void eval(const std::vector<Frame>& stack, const std::string& partial, size_t index, std::vector<std::string>& expanded)
{
    // Evaluate the stack to expand all function compositions

    // Base case: done building expansion, save result and return
    if (index == stack.size()) {
        expanded.push_back(partial);
        return;
    }

    // Recursive case: build each current FuncArgs around partial
    // and recurse
    const Frame& frame = stack[index];
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

        eval(stack, newPartial, index + 1, expanded);
    }
}

std::string solution(std::string& spec)
{
    // Split specification into tokens
    std::vector<std::string> tokens;
    tokenize(spec, ",;(){}", tokens);

    // Parse tokens into a useful stack structure.
    std::vector<Frame> stack;
    parse(tokens, stack);

    // Evaluate the stack to expand all function compositions
    std::vector<std::string> expanded;
    eval(stack, "input", 0, expanded);

    // Combine results into single new line delimited string
    std::string result;
    for (const auto& e : expanded)
        result += "\n" + e;

    return result;
}

struct repo_recovery_t {

    using commit_id_t       = int;
    using timestamp_t       = int;
    using file_path_t       = std::string;
    using opaque_id_t       = std::string;
    using unique_file_key_t = std::string;    // globally unique, file path + " " + opaque id

    struct repo_t {
        std::map<timestamp_t, std::set<commit_id_t>> ts_to_commit_map;
        std::unordered_map<file_path_t, opaque_id_t> file_to_opaque_map;
        std::unordered_map<opaque_id_t, file_path_t> opaque_to_file_map;
    };
    std::unordered_map<unique_file_key_t, std::shared_ptr<repo_t>> global_file_to_repo_map_;

    unique_file_key_t make_unique_file_key(const file_path_t& file_path, const opaque_id_t& opaque_id)
    {
        return file_path + " " + opaque_id;
    }

    bool push_commit(const std::vector<std::string>& commit_tokens)
    {
        const auto num_tokens = commit_tokens.size();
        if (num_tokens < 5 || num_tokens % 2 != 0) {
            return false;
        }

        commit_id_t commit_id = std::stoi(commit_tokens[1]);
        timestamp_t timestamp = std::stoi(commit_tokens[3]);

        std::shared_ptr<repo_t>                          repo;
        std::vector<std::pair<file_path_t, opaque_id_t>> unknown_repo_files;

        auto add_commit_to_repo = [](auto& repo, const auto& ts, const auto& id) {
            repo->ts_to_commit_map[ts].emplace(id);
        };

        auto add_file_to_repo = [commit_id, timestamp](auto& repo, const auto& file_path, const auto& opaque_id) {
            auto report_ambiguous = [&]() {
                std::cout << "Ambiguous: commit=" << commit_id << ", timestamp=" << timestamp << ", file=" << file_path
                          << ", opaque=" << opaque_id << std::endl;
            };

            if (auto [it_file, added] = repo->file_to_opaque_map.emplace(file_path, opaque_id);
                !added && it_file->second != opaque_id) {
                report_ambiguous();
                return false;
            }
            if (auto [it_opaque, added] = repo->opaque_to_file_map.emplace(opaque_id, file_path);
                !added && it_opaque->second != file_path) {
                report_ambiguous();
                return false;
            }
            return true;
        };

        auto add_file_to_global = [this, &add_commit_to_repo,
                                   &add_file_to_repo](auto& repo, const auto& file_path, const auto& opaque_id) {
            const auto file_key = make_unique_file_key(file_path, opaque_id);

            // the file along with opaque_id must be unique globally, and we need this check to ensure this file belongs
            // to 'repo' only
            //
            // it is possible when we are adding a file but the file has already been in some different repo, in this
            // case, we should consolidate it into 'repo' - commit 1: f1.h ab12 f2.h cd12    -> repo1 is created after
            // this step commit 2: f3.h xx12 f4.h yy12    -> repo2 is created after this step commit 3: f1.h ab12 f3.h
            // xx12    -> with 'f1.h ab12', we find repo1; when handling 'f3.h xx12', we find repo2; obviously we should
            // consolidate repo2 into repo1.

            if (auto [it_global, added] = global_file_to_repo_map_.emplace(file_key, repo);
                !added && repo != it_global->second) {
                int  other_repo_commits = 0;
                auto other_repo         = it_global->second;
                for (auto& [ts, ids] : other_repo->ts_to_commit_map) {
                    for (auto id : ids) {
                        add_commit_to_repo(repo, ts, id);
                        ++other_repo_commits;
                    }
                }

                std::cout << "Trying to consolidate repos (#commits=" << other_repo_commits
                          << ", #files=" << other_repo->file_to_opaque_map.size() << ") for file=" << file_key
                          << ", other_repo=" << other_repo.get() << std::endl;

                for (auto& [f_path, o_id] : other_repo->file_to_opaque_map) {
                    add_file_to_repo(repo, f_path, o_id);

                    const auto f_key = make_unique_file_key(f_path, o_id);
                    if (auto it = global_file_to_repo_map_.find(f_key); it != global_file_to_repo_map_.end()) {
                        if (it->second != other_repo) {
                            std::cout << "Impossible, file belongs to different repo, file key=" << f_key << std::endl;
                        }

                        // switch to desired 'repo', 'other_repo' will destroy by itself since none reference after this
                        // loop
                        it->second = repo;
                    } else {
                        // this is impossible, this file must have existed in the map for a while
                        std::cout << "Impossible, file must have existed in the global map for a while, file key="
                                  << f_key << std::endl;
                        return false;
                    }

                    // NOTICE: do we need to recursively check if 'f_key' is owned by another repo? i don't think so.
                }
            }
            return true;
        };

        auto add_file = [&](auto& file_path, auto& opaque_id) {
            if (!repo)
                return false;
            return add_file_to_repo(repo, file_path, opaque_id) && add_file_to_global(repo, file_path, opaque_id);
        };

        for (size_t i = 4; i < num_tokens; i += 2) {
            const file_path_t file_path = commit_tokens[i];
            const opaque_id_t opaque_id = commit_tokens[i + 1];

            if (!repo) {
                const auto file_key = make_unique_file_key(file_path, opaque_id);
                auto       iter     = global_file_to_repo_map_.find(file_key);
                if (iter == global_file_to_repo_map_.end()) {
                    unknown_repo_files.emplace_back(file_path, opaque_id);
                    continue;
                }
                repo = iter->second;

                add_commit_to_repo(repo, timestamp, commit_id);
            }

            if (!add_file(file_path, opaque_id)) {
                return false;
            }
        }

        // two cases if unknown_repo_files is not empty:
        // 1) the beginning file entries in the commit are new even though repo exists;
        // 2) none of the entries in the commit match any existing repo, we shall create a repo and reconcile later:
        //    a) this commit belongs to a new repo; or,
        //    b) this commit belongs to one of existing repo but we are in lack of information to find it;
        if (unknown_repo_files.empty())
            return true;

        if (!repo) {
            repo = std::make_shared<repo_t>();

            add_commit_to_repo(repo, timestamp, commit_id);
        }

        for (auto& [f_path, o_id] : unknown_repo_files) {
            if (!add_file(f_path, o_id)) {
                return false;
            }
        }
        return true;
    }

    bool query(const std::vector<std::string>& query_tokens)
    {
        const auto num_tokens = query_tokens.size();
        if (num_tokens != 4) {
            return false;
        }

        const timestamp_t ts_start  = std::stoi(query_tokens[0]);
        const timestamp_t ts_end    = std::stoi(query_tokens[1]);
        const file_path_t file_path = query_tokens[2];
        const opaque_id_t opaque_id = query_tokens[3];
        const auto        file_key  = make_unique_file_key(file_path, opaque_id);

        auto iter = global_file_to_repo_map_.find(file_key);
        if (iter == global_file_to_repo_map_.end()) {
            std::cout << std::endl;
            return false;
        }

        auto repo           = iter->second;
        auto it_commits     = repo->ts_to_commit_map.lower_bound(ts_start);
        auto it_commits_end = repo->ts_to_commit_map.upper_bound(ts_end);
        while (it_commits != it_commits_end) {
            for (auto commit : it_commits->second) {
                std::cout << commit << " ";
            }
            ++it_commits;
        }
        std::cout << std::endl;
        return true;
    }
};
void run_repo_recovery()
{
    {
        repo_recovery_t recovery;
        for (const auto& commit : {
                 "id 8 timestamp 200 quicksort.cpp 839ad0 mergesort.cpp 0cdde1 bubblesort.cpp 248dd1",
                 "id 0 timestamp 500 array.h 163111 sequence.h 294d3f",
                 "id 6 timestamp 200 mergesort.cpp 0cdde1 bogosort.cpp 4213ff",
                 "id 4 timestamp 1000 array.h 163111 vector.h fcc2af",
                 "id 2 timestamp 300 bubblesort.cpp 248dd1 bogosort.cpp 4213ff",
                 "id 3 timestamp 300 bubblesort.cpp eaf88a bogosort.cpp 4f11aa",
             }) {
            std::vector<std::string> tokens;
            tokenize(commit, " ", tokens);

            std::cout << commit << std::endl;
            if (!recovery.push_commit(tokens)) {
                std::cout << "Failed to push commit: " << commit << std::endl;
            }
        }

        for (const auto& query : {
                 "0 10000 quicksort.cpp 839ad0",
                 "0 500 vector.h fcc2af",
                 "0 100000 no_found.h empty_response",
                 "100 200 bogosort.cpp 4213ff",
             }) {
            std::vector<std::string> tokens;
            tokenize(query, " ", tokens);

            recovery.query(tokens);
        }
    }

    {
        repo_recovery_t recovery;
        for (const auto& commit : {
                 "id 38024 timestamp 74820 foo.py ac819f bar.py 0d82b9",
                 "id 49283 timestamp 19837 bar.py 0d82b9 baz.py f28dc2",
                 "id 20391 timestamp 23488 baz.py f28dc2 foo.py f918ca",
                 "id 2938 timestamp 101 qux.h d139af qux.cpp 718bc3",
                 "id 2939 timestamp 102 qux.h d139af",
             }) {
            std::vector<std::string> tokens;
            tokenize(commit, " ", tokens);

            std::cout << commit << std::endl;
            if (!recovery.push_commit(tokens)) {
                std::cout << "Failed to push commit: " << commit << std::endl;
            }
        }

        for (const auto& query : {
                 "0 1000000 bar.py 0d82b9",
                 "0 1000000 qux.h d139af",
             }) {
            std::vector<std::string> tokens;
            tokenize(query, " ", tokens);

            recovery.query(tokens);
        }
    }
}

void run_task_parser()
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
        std::cout << solution(spec) << std::endl;
    }
}

// https://www.youtube.com/watch?v=kPh8pod0-gk
// 0) sometimes this is called 'sticky counter' - it gets stuck at zero
// 1) required by std::weak_ptr<T>::lock;
// 2) also useful for atomic memory management / concurrent data structures, i.e. atomic<shared_ptr>
class Counter
{
private:
    static constexpr uint64_t is_zero = 1ull << 63;
    static constexpr uint64_t helped  = 1ull << 62;
    // precondition: counter is not zero
    std::atomic<uint64_t> counter_ = 1;

public:
    // increase counter and return true if counter is greater than zero, otherwise do nothing and return false
    bool increment_if_not_zero() { return (counter_.fetch_add(1) & is_zero) == 0; }

    // always decrease counter, return true if counter after decrease is zero, otherwise return false
    bool decrement()
    {
        if (counter_.fetch_sub(1) == 1) {
            uint64_t e = 0;
            if (counter_.compare_exchange_strong(e, is_zero)) {
                return true;
            }
            if ((e & helped) && (counter_.exchange(is_zero) & helped)) {
                return true;
            }
        }
        return false;
    }

    // return current value of counter
    uint64_t read()
    {
        auto val = counter_.load();
        if (val == 0 && counter_.compare_exchange_strong(val, is_zero | helped)) {
            return 0;    // in helping
        }
        return (val & is_zero) ? 0 : val;
    }
};

void run_perf_test()
{
    const int     length             = 512 * 1024 * 1024;
    constexpr int cache_line_size    = 64;
    const int     num_per_cache_line = cache_line_size / sizeof(int);
    const int     num_items          = length / num_per_cache_line;

    std::vector<int> arr(length);
    auto             start = std::chrono::high_resolution_clock::now();

    // access num_items/16 cache lines
    /*
        for (int i = 0; i < num_items; i++)
            arr[i]++;
    */
    /*
        // access num_items cache lines: much higher cost than above
        for (int i = 0; i < num_items*num_per_cache_line; i+=num_per_cache_line)
            arr[i]++;
    */
    // access num_items cache lines, twice of index access: similar cost as above
    for (int i = 0; i < num_items * num_per_cache_line; i += num_per_cache_line) {
        arr[i]++;
        arr[i + num_per_cache_line - 1]++;
    }

    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << (float)duration.count() << std::endl;
}

void run_thread_atom_perf_test()
{
    using int_t = int32_t;

    auto do_test = []() {
        const auto Int_Max = std::numeric_limits<int_t>::max();

        int_t                    cnt = 0;
        std::vector<std::thread> threadVec;
        for (int i = 0; i < 100; i++) {
            threadVec.emplace_back(std::move(std::thread([&cnt]() {
                int_t prev = 0, tmp = 0;
                while (prev < Int_Max) {
                    tmp = cnt;
                    // If it is not safe, tmp will not increase all the time.
                    if (tmp < prev)
                        std::cout << "Error cnt declined" << std::endl;
                    // if (tmp % 1000000 == 0) cout << tmp << endl;
                    prev = tmp;
                }
            })));
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));

        auto start = std::chrono::high_resolution_clock::now();

        while (cnt < Int_Max)
            cnt++;
        for (auto& t : threadVec)
            t.join();

        auto stop     = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        std::cout << (float)duration.count() / 1000 << std::endl;
    };

    do_test();

    auto do_atom = []() {
        using atom_int_t   = std::atomic<int_t>;
        const auto Int_Max = std::numeric_limits<int_t>::max();

        atom_int_t               cnt = 0;
        std::vector<std::thread> threadVec;
        for (int i = 0; i < 100; i++) {
            threadVec.emplace_back(std::move(std::thread([&cnt]() {
                int_t prev = 0, tmp = 0;
                while (prev < Int_Max) {
                    tmp = cnt.load(std::memory_order_relaxed);
                    // If it is not safe, tmp will not increase all the time.
                    if (tmp < prev)
                        std::cout << "Error cnt declined" << std::endl;
                    // if (tmp % 1000000 == 0) cout << tmp << endl;
                    prev = tmp;
                }
            })));
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));

        auto start = std::chrono::high_resolution_clock::now();

        int_t v = 0;
        while (v < Int_Max) {
            cnt.store(++v, std::memory_order_relaxed);
        }
        for (auto& t : threadVec)
            t.join();

        auto stop     = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        std::cout << (float)duration.count() / 1000 << std::endl;
    };
    do_atom();
}

list_node_int_t* reverse_list(list_node_int_t* head)
{
    list_node_int_t* prev = nullptr;
    list_node_int_t* curr = head;
    while (curr) {
        auto tmp   = curr->next;
        curr->next = prev;
        prev       = curr;
        curr       = tmp;
    }
    return prev;
}
void run_reverse_list()
{
    list_node_int_t* head = new list_node_int_t{
        .data = 10,
        .next = nullptr,
    };
    for (int i = 1; i < 10; ++i) {
        auto tmp   = head->next;
        head->next = new list_node_int_t{
            .data = i,
            .next = tmp,
        };
    }

    auto dump = [](list_node_int_t* nd) {
        while (nd) {
            std::cout << nd->data << " ";
            nd = nd->next;
        }
        std::cout << std::endl;
    };

    dump(head);

    auto rhead = reverse_list(head);

    dump(rhead);
}

std::pair<size_t, std::string> find_max_non_dupe_substring(const std::string& str)
{
    const auto length    = str.size();
    size_t     result_l  = 0;
    size_t     result_r  = 0;
    size_t     left      = 0;
    size_t     right     = 0;
    size_t     freq[256] = {0};
    while (right < length) {
        if (++freq[str[right]] == 1) {
            ++right;
            continue;
        }

        // TODO: probably the rest codes in the loop can be simplified...

        if (right - left > result_r - result_l) {
            result_l = left;
            result_r = right;
        }

        while (--freq[str[left]] == 0) {
            ++left;
        }
        ++left;
        ++right;
    }
    return {result_r - result_l, str.substr(result_l, result_r - result_l)};
}
void run_find_max_non_dupe_substring()
{
    for (auto str : {
             "abcabcbb",    // 3
             "bbbbb",       // 1
             "pwwkew",      // 3
         }) {
        const auto [len, s] = find_max_non_dupe_substring(str);
        std::cout << "Input=" << str << ", max_non_dupe_substring=[" << len << ", " << s << "]" << std::endl;
    }
}

std::string find_longest_palindrome(const std::string& str)
{
    const int len = str.size();

    // given index l & r as starting positions, find the longest palindrome by decreasing l (to the left) and increasing
    // r (to the right), <--- l, r --->
    auto impl = [&](int l, int r) {
        while (l <= r && l >= 0 && r < len) {
            if (str[l] != str[r]) {
                break;
            }
            --l;
            ++r;
        }
        return std::make_pair(l + 1, r - l - 1);
    };

    int max_len   = 0;
    int max_start = 0;
    for (int i = 0; i < len; ++i) {
        const auto v = std::max(impl(i, i), impl(i, i + 1));
        if (v.second > max_len) {
            max_start = v.first;
            max_len   = v.second;
        }
    }
    return str.substr(max_start, max_len);
}
void run_find_longest_palindrome()
{
    for (auto str : {
             "abcabcbb",    // 3
             "bbbbb",       // 1
             "pwwkew",      // 3
             "babad",       // "bab", "bab"
             "cbbd",        // "bb"
         }) {
        std::cout << "Input=" << str << ", longest_palindrome=" << find_longest_palindrome(str) << std::endl;
    }
}

std::string transform_str_zigzag(const std::string& str, int rows)
{
    std::vector<std::string> matrix(rows);
    const int                len   = str.size();
    int                      count = 0;
    int                      col   = 1;
    while (count < len) {
        // fill rows from top to down
        for (int r = 0; r < rows && count < len; ++r, ++count) {
            matrix[r] += str[count];
        }
        // fill diaganal from left-bottom to upper-right, only one character is taken from raw string
        for (int r = rows - 2; r > 0 && count < len; --r, ++count, ++col) {
            // fill all rows on current col
            for (int i = 0; i < rows; ++i) {
                if (i == r) {
                    matrix[i] += str[count];
                } else {
                    matrix[i] += ' ';
                }
            }
        }
    }

    std::string result;
    for (const auto& s : matrix) {
        for (auto c : s) {
            if (c != ' ') {
                result += c;
            }
        }
    }
    return result;
}
std::string transform_str_zigzag_2(const std::string& str, int rows)
{
    std::vector<std::string> matrix(rows);
    int                      r    = 0;
    int                      step = -1;
    for (auto c : str) {
        matrix[r] += c;
        // NOTICE: this is the most interesting part, just reverse the step sign at boundary points which gain better
        // performance than above version; and it fully meets the goal by adding row string in the matrix, if the
        // question asks for the zigzag matrix, this solution will not work.
        if (r == rows - 1 || r == 0) {
            step *= -1;
        }
        r += step;
    }

    std::string result;
    for (const auto& s : matrix) {
        result += s;
    }
    return result;
}
void run_transform_str_zigzag()
{
    using pair_t = std::pair<std::string, int>;
    for (auto [str, rows] : {
             pair_t("PAYPALISHIRING", 3),    // "PAHNAPLSIIGYIR"
             pair_t("PAYPALISHIRING", 4),    // "PINALSIGYAHRPI"
             pair_t("A", 1),                 // "A"
         }) {
        std::cout << "Input=[" << str << ", " << rows << "], zigzag=" << transform_str_zigzag(str, rows) << std::endl;
        std::cout << "Input=[" << str << ", " << rows << "], zigzag=" << transform_str_zigzag_2(str, rows) << std::endl;
    }
}

int reverse_signed_int(int v)
{
    int result = 0;
    while (v) {
        if (result > INT_MAX / 10 || result < INT_MIN / 10) {
            return 0;
        }
        /*
        // NOTICE: below case is impossible
        if ((result == INT_MAX / 10 && v % 10 > INT_MAX % 10) ||
            (result == INT_MIN / 10 && v % 10 < INT_MIN % 10)) {
            return 0;
        }
        */
        result = result * 10 + v % 10;
        v /= 10;
    }
    return result;
}
void run_reverse_signed_int()
{
    using use_case_t = std::tuple<int, int>;
    for (auto [a, exp_v] : {
             use_case_t(123, 321),
             use_case_t(-123, -321),
             use_case_t(120, 21),
             use_case_t(0, 0),
             use_case_t(INT_MAX, 0),
             use_case_t(INT_MIN, 0),
             use_case_t(2147483647, 0),
             use_case_t(1212121212, 2121212121),
             use_case_t(1212121213, 0),
             use_case_t(1463847412, 2147483641),
             use_case_t(2063847412, 2147483602),
             use_case_t(-1212121212, -2121212121),
             use_case_t(-1212121213, 0),
             use_case_t(-1463847412, -2147483641),
             use_case_t(-2063847412, -2147483602),
         }) {
        const auto v = reverse_signed_int(a);
        std::cout << "Input=" << a << ", reverse_signed_int=" << v << ", " << (exp_v == v ? "SUCCESS" : "FAILED")
                  << std::endl;
    }
}

// Rotational Cipher
//
// One simple way to encrypt a string is to "rotate" every alphanumeric character by a certain amount. Rotating a
// character means replacing it with another character that is a certain number of steps away in normal alphabetic or
// numerical order.
//
// For example, if the string "Zebra-493?" is rotated 3 places, the resulting string is "Cheud-726?". Every alphabetic
// character is replaced with the character 3 letters higher (wrapping around from Z to A), and every numeric character
// replaced with the character 3 digits higher (wrapping around from 9 to 0). Note that the non-alphanumeric characters
// remain unchanged.
std::string rotational_cipher(std::string input, int factor)
{
    for (auto& c : input) {
        // use int than char in order to avoid overflow!
        int v = c;
        for (const auto& [min_v, max_v] : {
                 std::pair<int, int>('0', '9'),
                 std::pair<int, int>('A', 'Z'),
                 std::pair<int, int>('a', 'z'),
             }) {
            if (v < min_v || v > max_v) {
                continue;
            }

            v += factor;
            // ah man, this simple equation takes a while!
            // 1) attention: overflow handling
            // 2) attention: the rotation is starting from same set, i.e. the next character of 'Z' is 'A' than 'a'
            // 3) attention: the starting point is not 0, but '0', 'A', 'a' respectively, so we should do offset
            //    (v - min_v) before the mode
            v = (v - min_v) % (max_v - min_v + 1) + min_v;
            c = (char)v;
            break;
        }
    }
    return input;
}
void run_rotational_cipher()
{
    using use_case_t = std::tuple<std::string, int, std::string>;
    for (const auto [str, factor, exp] : {
             use_case_t{"Zebra-493?", 3, "Cheud-726?"},
             use_case_t("All-convoYs-9-be:Alert1.", 4, "Epp-gsrzsCw-3-fi:Epivx5."),
             use_case_t("abcdZXYzxy-999.@", 200, "stuvRPQrpq-999.@"),
         }) {
        std::cout << "String=\"" << str << "\", factor=" << factor << ", expected=\"" << exp << "\", result=\""
                  << rotational_cipher(str, factor) << "\"" << std::endl;
    }
}

// Matching Pairs
//
// Given two strings s and t of length N, find the maximum number of possible matching character pairs in strings s and
// t after swapping exactly two characters within s.
//
// A swap is switching s[i] and s[j], where s[i] and s[j] denotes the character that is present at the ith and jth index
// of s, respectively.
// The matching pairs of the two strings are defined as the number of indices for which s[i] and t[i] are equal.
//
// Note: you must swap two characters at different indices.
//
// Example 1
// s = "abcd"
// t = "adcb"
// output = 4
//
// Example 2
// s = "mno"
// t = "mno"
// output = 1
int matching_pairs_after_one_swap(const std::string& s1, const std::string& s2)
{
    if (s1.size() != s2.size())
        return -1;

    int matches = 0;
    // assume no duplicate characters in s1 and s2, otherwise should change this container
    std::unordered_map<char, int> locations_1;
    std::unordered_map<char, int> locations_2;
    for (int i = 0; i < s1.size(); ++i) {
        if (s1[i] != s2[i]) {
            locations_1[s1[i]] = i;
            locations_2[s2[i]] = i;
        } else {
            ++matches;
        }
    }

    // 1) s1 == s2, after swap, it will reduce the total matched characters by 2;
    // 2) there is only one position not matched, after swap, it will reduce the total matched by 1;
    // 3) there are more than one positions not matched, and none of them match, so the total matches have no change;
    // 4) there are some not matched, and only one from s1 & s2 matches, after swap the total matches increases by 1;
    // 5) there are some not matched, and at least two from s1 & s2 matches, after swap the matches increases by 2;
    //
    // by default case 1)
    int increment = -2;
    for (auto [c, i_1] : locations_1) {
        if (auto iter = locations_2.find(c); iter != locations_2.end()) {
            auto i_2 = iter->second;
            // case 5)
            if (s1[i_2] == s2[i_1]) {
                increment = 2;
                break;
            }
            // case 4)
            increment = 1;
        }
        // case 2) & 3)
        if (increment < 0) {
            ++increment;
        }
    }
    matches += increment;
    return matches;
}
void run_matching_pairs_after_one_swap()
{
    using use_case_t = std::tuple<std::string, std::string, int>;
    for (auto [s1, s2, exp_v] : {
             use_case_t("abcde", "adcbe", 5),
             use_case_t("abcd", "abcd", 2),
             use_case_t("zbcd", "abcd", 2),
             use_case_t("zbcx", "abcd", 2),
             use_case_t("abcdefg", "abcdfeh", 6),
         }) {
        const auto v = matching_pairs_after_one_swap(s1, s2);
        std::cout << "Input=[" << s1 << ", " << s2 << "], matching_pairs_after_one_swap=" << v << ", "
                  << (exp_v == v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

// Given string s and t, find the min length of substring in s so that the substring contains all characters in t
std::string min_length_substring_cover_all(const std::string& s, const std::string& t)
{
    if (s.empty() || t.empty() || t.size() > s.size()) {
        return "";
    }

    std::array<int, 256> freq_t = {0};
    for (auto c : t) {
        ++freq_t[c];
    }

    int                  found = 0;
    int                  left  = 0;
    int                  right = 0;
    std::string          result;
    std::array<int, 256> freq_u = {0};
    for (; right < s.size(); ++right) {
        const auto u = s[right];
        // continue if u is not in t, or u has happened more that that needed in t
        if (freq_t[u] == 0 || ++freq_u[u] > freq_t[u]) {
            continue;
        }
        // count current u as needed character by t
        if (++found != t.size()) {
            continue;
        }

        // so far the range s[left, right] contains all needed characters in t, but we need to clean up - shrink from
        // left to find desired starting point
        while (left <= right) {
            const auto v = s[left];
            if (freq_t[v] > 0 && freq_u[v] == freq_t[v]) {
                // should not delete more than 1 necessary character, stop here and start forward right index
                if (found == t.size() - 1) {
                    break;
                }

                if (result.empty() || result.size() > right - left + 1) {
                    result = s.substr(left, right - left + 1);
                }
                --found;
            } else if (freq_u[v] > 1) {    // redundant character
                --freq_u[v];
            }
            ++left;
        }
    }
    return result;
}
void run_min_length_substring_cover_all()
{
    using use_case_t = std::tuple<std::string, std::string, std::string>;
    for (auto [s1, s2, exp_v] : {
             use_case_t("ab", "ac", ""),
             use_case_t("ab", "abc", ""),
             use_case_t("abc", "c", "c"),
             use_case_t("abc", "x", ""),
             use_case_t("abc", "cba", "abc"),
             use_case_t("dcbefebce", "fd", "dcbef"),
             use_case_t("bfbeadbcbcbfeaaeefcddcccbbbfaaafdbebedddf", "cbccfafebccdccebdd", ""),
         }) {
        const auto str = min_length_substring_cover_all(s1, s2);
        std::cout << "Input=[" << s1 << ", " << s2 << "], min_length_substring_cover_all=["
                  << (str.empty() ? -1 : (int)str.size()) << ", " << str << "], "
                  << (exp_v == str ? "SUCCESS" : "FAILED") << std::endl;
    }
}

// https://leetcode.com/discuss/post/1923727/meta-facebook-recruiting-portal-1-billio-rt7s/
//
// We have N different apps with different user growth rates. At a given time t, measured in days, the number of users
// using an app is g^t (for simplicity we allow fractional users), where g is the growth rate for that app. These
// apps will all be launched at the same time and no user ever uses more than one of the apps.
// We want to know how many total users there are when you add together the number of users from each app.
//
// After how many full days will we have 1 billion total users across the N apps?
int get_billion_users_day(const std::vector<float>& growth_rates)
{
    constexpr auto one_billion = 1'000'000'000;
    auto           iter        = std::max_element(growth_rates.begin(), growth_rates.end());
    if (iter == growth_rates.end()) {
        return -1;
    }
    const int max_days = std::ceil(std::log(one_billion) / std::log(*iter));

    int left  = 0;
    int right = max_days;
    while (left < right) {
        const int mid = (left + right) / 2;

        int cumu_value = 0;
        for (auto rate : growth_rates) {
            cumu_value += std::pow(rate, mid);
            if (cumu_value > one_billion) {
                break;
            }
        }

        if (cumu_value > one_billion) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    return left;
}
void run_get_billion_users_day()
{
    using use_case_t = std::tuple<std::vector<float>, int>;
    for (const auto& [rates, exp_v] : {
             use_case_t({1.5}, 52),
             use_case_t({1.1, 1.2, 1.3}, 79),
             use_case_t({1.01, 1.02}, 1047),
         }) {
        const auto v = get_billion_users_day(rates);
        std::cout << "get_billion_users_day=" << v << ", " << (exp_v == v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

// Queue Removals
//
// You're given a list of n integers arr, which represent elements in a queue (in order from front to back). You're also
// given an integer x, and must perform x iterations of the following 3-step process:
//
// 1) Pop x elements from the front of queue (or, if it contains fewer than x elements, pop all of them);
//
// 2) Of the elements that were popped, find the one with the largest value (if there are multiple such elements, take
// the one which had been popped the earliest), and remove it;
//
// 3) For each one of the remaining elements that were popped (in the order they had been popped), decrement its value
// by 1 if it's positive (otherwise, if its value is 0, then it's left unchanged), and then add it back to the queue;
//
// Compute a list of x integers output, the ith of which is the 1-based index in the original array of the element which
// had been removed in step 2 during the ith iteration.
std::vector<int> queue_removal_find_positions(std::vector<int> arr, int x)
{
    std::vector<int> result(x, -1);
    const auto       len              = arr.size();
    int              times            = 0;
    int              idx_global_count = 0;
    while (times < x) {
        int local_pop_count = 0;
        int local_max_idx   = -1;
        int local_max_value = -1;
        while (local_pop_count < x && local_pop_count < len - times) {
            // get real idx
            const auto idx = idx_global_count % len;
            ++idx_global_count;

            // ignore the element that has been removed
            if (arr[idx] < 0) {
                continue;
            }

            ++local_pop_count;
            if (local_max_idx < 0 || local_max_value < arr[idx]) {
                local_max_idx   = idx;
                local_max_value = arr[local_max_idx];
            }
            if (arr[idx] > 0) {
                --arr[idx];
            }
        }

        if (local_max_idx < 0) {
            break;
        }

        arr[local_max_idx] = -1;
        result[times]      = local_max_idx + 1;

        ++times;
    }

    return result;
}

std::vector<int> queue_removal_find_positions_2(std::vector<int> arr, int x)
{
    std::queue<std::pair<int, int>> q;
    for (int i = 0; i < arr.size(); ++i) {
        q.push({arr[i], i + 1});
    }

    int              times = 0;
    std::vector<int> result;
    while (!q.empty() && times < x) {
        int                              poped = 0;
        std::pair<int, int>              max_pair(-1, -1);
        std::vector<std::pair<int, int>> cache;
        while (!q.empty() && poped++ < x) {
            cache.push_back(q.front());
            q.pop();
            if (max_pair.first < cache.back().first) {
                max_pair = cache.back();
            }
        }
        for (auto& p : cache) {
            if (p == max_pair) {
                result.push_back(p.second);
            } else if (p.first > 0) {
                q.push({p.first - 1, p.second});
            } else {
                q.push(p);
            }
        }

        ++times;
    }
    return result;
}
void run_queue_removal_find_positions()
{
    auto dump_arr = [](const auto& vs) {
        std::ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < vs.size(); ++i) {
            oss << vs[i];
            if (i != vs.size() - 1) {
                oss << ",";
            }
        }
        oss << "]";
        return oss.str();
    };

    using use_case_t = std::tuple<std::vector<int>, int, std::vector<int>>;
    for (const auto [arr, k, exp_arr] : {
             use_case_t({1, 2, 2, 3, 4, 5}, 5, {5, 6, 4, 1, 2}),
             use_case_t({2, 4, 2, 4, 3, 1, 2, 2, 3, 4, 3, 4, 4}, 4, {2, 5, 10, 13}),
         }) {
        const auto ans_1 = queue_removal_find_positions(arr, k);
        std::cout << "queue_removal_find_positions=" << dump_arr(ans_1) << ", "
                  << (ans_1 == exp_arr ? "SUCCESS" : "FAILED") << std::endl;

        const auto ans_2 = queue_removal_find_positions_2(arr, k);
        std::cout << "queue_removal_find_positions_2=" << dump_arr(ans_2) << ", "
                  << (ans_2 == exp_arr ? "SUCCESS" : "FAILED") << std::endl;
    }
}

// https://www.geeksforgeeks.org/dsa/minimize-operations-to-make-array-a-permutation/
//
// Given an array arr of n integers. You want to make this array a permutation of integers 1 to n. In one operation you
// can choose two integers i (0 ≤ i < n) and x (x > 0), then replace arr[i] with arr[i] mod x, the task is to determine
// the minimum number of operations to achieve this goal otherwise return -1.
int min_oper_mod_for_permutation(std::vector<int> arr)
{
    std::sort(arr.begin(), arr.end(), std::less<>());

    const auto        len = arr.size();
    std::vector<bool> exists(len + 1, false);
    std::vector<int>  leftovers;
    for (size_t i = 0; i < len; ++i) {
        if (arr[i] <= len && !exists[arr[i]]) {
            exists[arr[i]] = true;
            continue;
        }

        // two cases, we must do mod operation to get desired permutation, since arr was sorted, the leftovers are
        // sorted as well after pushing value:
        // 1) arr[i] <= n but exists[arr[i]] == true, means duplicate arr[i];
        // 2) arr[i] > n;
        //
        // NOTICE: it is always true number N mod value is less than its half!!
        const auto max_v_after_mod = (arr[i] % 2 == 0 ? (arr[i] / 2 - 1) : arr[i] / 2);
        leftovers.push_back(max_v_after_mod);
    }

    int min_opers = 0;
    int j         = 1;
    // all integers in leftovers need mod operations to form desired permutation
    for (auto v : leftovers) {
        while (exists[j]) {
            ++j;
        }
        // there is no way to mod a small number for big number
        if (v < j) {
            return -1;
        }
        ++j;
        ++min_opers;
    }
    return min_opers;
}
void run_min_oper_mod_for_permutation()
{
    using use_case_t = std::pair<std::vector<int>, int>;
    for (auto [arr, exp_v] : {
             use_case_t({1, 7}, 1),
             use_case_t({1, 5, 4}, -1),
         }) {
        const auto v = min_oper_mod_for_permutation(arr);
        std::cout << v << ", " << (v == exp_v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

// https://www.geeksforgeeks.org/dsa/minimum-number-platforms-required-railwaybus-station/
//
// Given two arrays arr[] and dep[], that represent the arrival and departure time of i-th train respectively. Find the
// minimum number of platforms required so that no train has to wait. If the departure time of one train is the same as
// the arrival time of another train, both trains cannot use the same platform at that time.
int min_platforms_required(const std::vector<int>& arrivals, const std::vector<int>& departures)
{
    auto arr = arrivals;
    auto dep = departures;
    std::sort(arr.begin(), arr.end(), std::less<>());
    std::sort(dep.begin(), dep.end(), std::less<>());
    int    result  = 0;
    int    count   = 0;
    size_t idx_dep = 0;
    for (auto t : arr) {
        // traverse and decrement count (required platforms so far) as further as possible
        // the count is the max platforms needed so far, this works since the two lists are sorted
        while (idx_dep < dep.size() && dep[idx_dep] < t) {
            ++idx_dep;
            --count;
        }
        ++count;
        result = std::max(result, count);
    }
    return result;
}
int min_platforms_required_2(const std::vector<int>& arrivals, const std::vector<int>& departures)
{
    if (arrivals.size() != departures.size()) {
        return -1;
    }

    int min_v = std::numeric_limits<int>::max();
    int max_v = 0;
    for (size_t i = 0; i < arrivals.size(); ++i) {
        min_v = std::min(min_v, arrivals[i]);
        max_v = std::max(max_v, departures[i]);
    }

    // at specific time, tell the number of more (positive) or less (negative) platforms required
    // enlarge the container size by 1 to keep an extra element for departure
    std::vector<int> timed_platforms(max_v - min_v + 2, 0);
    for (size_t i = 0; i < arrivals.size(); ++i) {
        // increment at the itme of arrival
        const auto idx_arr = arrivals[i] - min_v;
        ++timed_platforms[idx_arr];

        // decrement right after the departure
        const auto idx_dep = departures[i] - min_v + 1;
        --timed_platforms[idx_dep];
    }

    int result = 0;
    int count  = 0;
    for (auto v : timed_platforms) {
        count += v;
        // NOTICE: should check the max value at each step
        result = std::max(result, count);
    }
    return result;
}
void run_min_platforms_required()
{
    using use_case_t = std::tuple<std::vector<int>, std::vector<int>, int>;
    for (auto [arrs, deps, exp_v] : {
             use_case_t({1000, 935, 1100}, {1200, 1240, 1130}, 3),
             use_case_t({900, 1235, 1100}, {1000, 1240, 1200}, 1),
         }) {
        const auto v1 = min_platforms_required(arrs, deps);
        const auto v2 = min_platforms_required_2(arrs, deps);
        std::cout << "min_platforms_required=" << v1 << (v1 == exp_v ? ", SUCCESS" : ", FAILED") << std::endl;
        std::cout << "min_platforms_required_2=" << v2 << (v2 == exp_v ? ", SUCCESS" : ", FAILED") << std::endl;
    }
}

// https://www.geeksforgeeks.org/dsa/policemen-catch-thieves/
//
// Given an array arr[], where each element represents either a policeman (P) or a thief (T). Each policeman can catch
// only one thief and policeman can only catch a thief if the distance between them is at most k units. Find the maximum
// number of thieves that can be caught.
//
// Input: arr[] = ['P', 'T', 'T', 'P', 'T'], k = 1
// Output: 2.
// Explanation: policeman of 0th index catches thief  at index 1 and policeman of 3rd index can catch thief at either
// 2nd or 4th index.
//
// Input: arr[] = ['T', 'T', 'P'], k = 1
// Output: 1
// Explanation: Policeman of 2th index catches thief at index 1.
//
// stack/queue alike way, space inefficient
int max_thieves_caught(const std::vector<char>& arr, int k)
{
    std::queue<int> thieves;
    std::queue<int> policemen;
    int             result = 0;

    auto impl = [k](int idx, auto& updates, auto& existing) {
        while (!existing.empty()) {
            if (std::abs(existing.front() - idx) <= k) {
                existing.pop();
                return true;
            }
            existing.pop();
        }
        updates.push(idx);
        return false;
    };

    for (int i = 0; i < (int)arr.size(); ++i) {
        if (arr[i] == 'P') {
            result += impl(i, policemen, thieves);
        } else if (arr[i] == 'T') {
            result += impl(i, thieves, policemen);
        }
    }
    return result;
}
// two pointers alike way
int max_thieves_caught_2(const std::vector<char>& arr, int k)
{
    int    result        = 0;
    size_t idx_thief     = 0;    // track the last unchecked thief
    size_t idx_policeman = 0;    // track the last unchecked policeman
    while (true) {
        while (idx_thief < arr.size() && arr[idx_thief] != 'T') {
            ++idx_thief;
        }
        while (idx_policeman < arr.size() && arr[idx_policeman] != 'P') {
            ++idx_policeman;
        }

        if (idx_thief == arr.size() || idx_policeman == arr.size()) {
            break;
        }

        if (std::max(idx_thief, idx_policeman) - std::min(idx_thief, idx_policeman) <= k) {
            ++result;
            ++idx_thief;
            ++idx_policeman;
        } else if (idx_thief > idx_policeman) {
            ++idx_policeman;
        } else {
            ++idx_thief;
        }
    }
    return result;
}
void run_max_thieves_caught()
{
    using use_case_t = std::tuple<std::vector<char>, int, int>;
    for (auto [arr, k, exp_v] : {
             use_case_t({'T', 'T', 'P'}, 1, 1),
             use_case_t({'P', 'T', 'T', 'P', 'T'}, 1, 2),
             use_case_t({'P', 'T', 'T', 'P', 'T'}, 2, 2),
             use_case_t({'P', 'T', 'T', 'T', 'T', 'P', 'T', 'P'}, 1, 3),
             use_case_t({'P', 'T', 'T', 'T', 'T', 'P', 'T', 'P'}, 2, 3),
             use_case_t({'P', 'P', 'T', 'T', 'T', 'P', 'T', 'T'}, 1, 2),
             use_case_t({'P', 'P', 'T', 'T', 'T', 'P', 'T', 'T'}, 2, 3),
         }) {
        const auto v1 = max_thieves_caught(arr, k);
        const auto v2 = max_thieves_caught_2(arr, k);
        std::cout << "max_thieves_caught=" << v1 << ", " << (v1 == exp_v ? "SUCCESS" : "FAILED") << std::endl;
        std::cout << "max_thieves_caught_2=" << v2 << ", " << (v2 == exp_v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

// https://www.geeksforgeeks.org/dsa/minimum-number-of-candies-required-to-distribute-among-children-based-on-given-conditions/
//
// Given an array arr[] where each element represents the rating of a child, find the minimum number of candies required
// to distribute among all children under the following conditions:
// 1) Every child must receive at least one candy.
// 2) A child with a higher rating than their immediate neighbor(s) must receive more candies than that neighbor.
int min_candy_distributed(const std::vector<int>& ratings)
{
    const auto len = ratings.size();
    if (len == 0) {
        return 0;
    }

    int result       = 1;
    int last_candies = 1;
    int idx          = 1;
    while (idx < len) {
        while (idx < len && ratings[idx] == ratings[idx - 1]) {
            result += last_candies;
            ++idx;
        }

        while (idx < len && ratings[idx] > ratings[idx - 1]) {
            ++last_candies;
            result += last_candies;
            ++idx;
        }

        int bottom = 1;
        while (idx < len && ratings[idx] < ratings[idx - 1]) {
            result += bottom;
            ++bottom;
            ++idx;
        }
        // some extra work for downside trend in the case when the previously determined peak value is not satisfied
        if (bottom > 1) {
            // adjust the last peak value if necessary
            result += std::max(0, bottom - last_candies);

            // always reset last_candies to 1
            last_candies = 1;
        }
    }
    return result;
}
int min_candy_distributed_2(const std::vector<int>& ratings)
{
    const auto len = ratings.size();
    if (len == 0) {
        return 0;
    }

    // from left to right, find the min required candies
    std::vector<int> candies_left(len, 1);
    for (size_t i = 1; i < len; ++i) {
        if (ratings[i] > ratings[i - 1]) {
            candies_left[i] = candies_left[i - 1] + 1;
        }
    }
    // from right to left, find the min required candies
    std::vector<int> candies_right(len, 1);
    for (size_t i = len - 1; i > 0; --i) {
        if (ratings[i - 1] > ratings[i]) {
            candies_right[i - 1] = candies_right[i] + 1;
        }
    }

    int result = 0;
    // NOTICE: get the required candies that meet both side
    for (size_t i = 0; i < len; ++i) {
        result += std::max(candies_left[i], candies_right[i]);
    }
    return result;
}
void run_min_candy_distributed()
{
    using use_case_t = std::tuple<std::vector<int>, int>;
    for (auto [ratings, exp_v] : {
             use_case_t({1, 2, 3, 4, 5}, 15),
             use_case_t({9, 9, 9, 9}, 4),
             use_case_t({15, 40, 32, 12, 17, 23}, 12),
         }) {
        const auto v1 = min_candy_distributed(ratings);
        const auto v2 = min_candy_distributed_2(ratings);
        std::cout << "min_candy_distributed=" << v1 << ", " << (v1 == exp_v ? "SUCCESS" : "FAILED") << std::endl;
        std::cout << "min_candy_distributed_2=" << v2 << ", " << (v2 == exp_v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

// https://www.geeksforgeeks.org/dsa/job-sequencing-problem/
//
// You are given two arrays: deadline[], and profit[], which represent a set of jobs, where each job is associated with
// a deadline, and a profit. Each job takes 1 unit of time to complete, and only one job can be scheduled at a time. You
// will earn the profit associated with a job only if it is completed by its deadline.
//
// Your task is to find:
// 1) The maximum number of jobs that can be completed within their deadlines.
// 2) The total maximum profit earned by completing those jobs.
std::pair<int, int> max_profile_job_sequence(const std::vector<int>& deadlines, const std::vector<int>& profits)
{
    const int len = (int)deadlines.size();
    if (len != (int)profits.size()) {
        return {};
    }

    // <deadline, profit>
    using entry_t = std::pair<int, int>;
    // sort the jobs by deadline in ascending order, which is the order of jobs to be picked up
    std::priority_queue<entry_t, std::vector<entry_t>, std::greater<>> jobs;
    for (int i = 0; i < len; ++i) {
        jobs.push({deadlines[i], profits[i]});
    }

    // container that keeps all picked jobs for best profit, sort by profit in ascending order
    std::priority_queue<int, std::vector<int>, std::greater<>> scheduler;
    while (!jobs.empty()) {
        const auto [deadline, profit] = jobs.top();
        // scheduler.size() is the ending time of all jobs in the scheduler finish at, since each job takes 1 unit time
        // so if ending time is earlier than new job's deadline, just add this job
        if (scheduler.size() < deadline) {
            scheduler.push(profit);
        } else if (scheduler.top() < profit) {
            // if ending time is later, seek the way for best profit
            scheduler.pop();
            scheduler.push(profit);
        }
        jobs.pop();
    }

    // the final scheduler contains all jobs that can be processed one by one with best total profits
    std::pair<int, int> result;
    while (!scheduler.empty()) {
        ++result.first;
        result.second += scheduler.top();
        scheduler.pop();
    }
    return result;
}
void run_max_profile_job_sequence()
{
    using use_case_t = std::tuple<std::vector<int>, std::vector<int>, std::pair<int, int>>;
    for (auto [deadlines, profits, exp_v] : {
             use_case_t({4, 1, 1, 1}, {20, 10, 40, 30}, {2, 60}),
             use_case_t({2, 1, 2, 1, 1}, {100, 19, 27, 25, 15}, {2, 127}),
             use_case_t({3, 1, 2, 2}, {50, 10, 20, 30}, {3, 100}),
         }) {
        const auto v = max_profile_job_sequence(deadlines, profits);
        std::cout << "max_profile_job_sequence=[" << v.first << ", " << v.second << "], "
                  << (exp_v == v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

// https://www.geeksforgeeks.org/dsa/largest-number-with-one-swap-allowed/
//
// Given a numeric string s, find the lexicographically largest string by swapping at most one pair of characters.
std::string largest_number_by_one_swap(std::string str)
{
    int idx_max             = -1;
    int idx_candidate_small = -1;
    int idx_candidate_big   = -1;
    for (int i = (int)str.size() - 1; i >= 0; --i) {
        if (str[i] >= str[idx_max]) {
            idx_max = i;
        } else {
            idx_candidate_small = i;
            idx_candidate_big   = idx_max;
        }
    }
    if (idx_candidate_big > 0) {
        std::swap(str[idx_candidate_small], str[idx_candidate_big]);
    }
    return str;
}
void run_largest_number_by_one_swap()
{
    using use_case_t = std::pair<std::string, std::string>;
    for (auto [s, exp_s] : {
             use_case_t("768", "867"),
             use_case_t("876", "876"),
             use_case_t("678", "876"),
             use_case_t("333", "333"),
         }) {
        const auto v = largest_number_by_one_swap(s);
        std::cout << "s=" << s << ", largest_number_by_one_swap=" << v << ", " << (v == exp_s ? "SUCCESS" : "FAILED")
                  << std::endl;
    }
}

// https://www.geeksforgeeks.org/dsa/find-a-tour-that-visits-all-stations/
//
// Given two arrays gas[] and cost[], each gas station i provides gas[i] fuel, and it takes cost[i] fuel to travel to
// the next station. Starting with an empty tank, find the index of the gas station from which you can complete a full
// circular tour. If it’s not possible, return -1.
//
// Note: If a solution exists, it is guaranteed to be unique.
int gas_station(const std::vector<int>& refills, const std::vector<int>& costs)
{
    const int len       = refills.size();
    int       idx_start = 0;
    while (true) {
        int stops    = 0;
        int left_gas = 0;
        while (stops < len) {
            auto idx = (idx_start + stops) % len;
            left_gas = left_gas + refills[idx] - costs[idx];
            if (left_gas < 0) {
                // NOTICE: we originally start from idx 0, should not use any stop as starting point more than once
                if (idx_start + stops >= len) {
                    return -1;
                }
                idx_start = (idx + 1) % len;
                break;
            }
            ++stops;
        }
        if (stops == len) {
            return idx_start;
        }
    }
    return -1;
}
// NOTICE: this solution utilizes the information "If a solution exists, it is guaranteed to be unique"
//
// As long as the total gas is positive, there must be a solution! So we just need one loop!
int gas_station_2(const std::vector<int>& refills, const std::vector<int>& costs)
{
    const int len          = refills.size();
    int       idx_start    = 0;
    int       tot_left_gas = 0;
    int       cur_left_gas = 0;
    for (int i = 0; i < len; ++i) {
        tot_left_gas = tot_left_gas + refills[i] - costs[i];
        cur_left_gas = cur_left_gas + refills[i] - costs[i];
        if (cur_left_gas < 0) {
            idx_start    = i + 1;
            cur_left_gas = 0;
        }
    }
    return tot_left_gas >= 0 ? idx_start : -1;
}
void run_gas_station()
{
    using use_case_t = std::tuple<std::vector<int>, std::vector<int>, int>;
    for (const auto& [refills, costs, exp_v] : {
             use_case_t({4, 5, 7, 4}, {6, 6, 3, 5}, 2),
             use_case_t({1, 2, 3, 4, 5}, {3, 4, 5, 1, 2}, 3),
             use_case_t{{3, 9}, {7, 6}, -1},
         }) {
        const auto v1 = gas_station(refills, costs);
        const auto v2 = gas_station_2(refills, costs);
        std::cout << "gas_station=" << v1 << ", " << (v1 == exp_v ? "SUCCESS" : "FAILED") << std::endl;
        std::cout << "gas_station_2=" << v2 << ", " << (v2 == exp_v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

// leetcode 24
list_node_int_t* switch_neighbors_list(list_node_int_t* head)
{
    list_node_int_t dummy{
        .next = head,
    };
    auto new_head  = head;
    auto node_prev = &dummy;
    while (node_prev->next) {
        auto node_curr = node_prev->next;
        auto node_next = node_curr->next;
        if (node_next) {
            node_curr->next = node_next->next;
            node_next->next = node_curr;
            node_prev->next = node_next;
            node_prev       = node_curr;

            if (new_head == head) {
                new_head = node_next;
            }
        } else {
            break;
        }
    }
    return new_head;
}
list_node_int_t* switch_neighbors_list_2(list_node_int_t* head)
{
    list_node_int_t dummy{
        .next = head,
    };
    auto node_prev = &dummy;
    auto node_curr = head;
    while (node_curr && node_curr->next) {
        // update curr->next
        auto node_curr_new    = node_curr->next->next;
        node_curr->next->next = node_curr;

        // update curr
        node_prev->next = node_curr->next;
        node_curr->next = node_curr_new;

        // update prev
        node_prev = node_curr;

        // update curr and move on
        node_curr = node_curr_new;
    }
    return dummy.next;
}
void run_switch_neighbors_list()
{
    using use_case_t = std::pair<list_node_int_t*, std::string>;

    auto create_list = [](int len) {
        list_node_int_t dummy;
        auto            node = &dummy;
        while (len--) {
            node->next = new list_node_int_t{
                .data = len,
            };
            node = node->next;
        }
        return dummy.next;
    };
    auto dump_list = [](list_node_int_t* node) {
        std::ostringstream oss;
        oss << "[";
        while (node) {
            oss << node->data;
            node = node->next;
            if (node) {
                oss << "->";
            }
        }
        oss << "]";
        return oss.str();
    };
    for (auto [head, exp_v] : {
             use_case_t(create_list(1), "[0]"),
             use_case_t(create_list(2), "[0->1]"),
             use_case_t(create_list(3), "[1->2->0]"),
             use_case_t(create_list(4), "[2->3->0->1]"),
             use_case_t(create_list(9), "[7->8->5->6->3->4->1->2->0]"),
             use_case_t(create_list(10), "[8->9->6->7->4->5->2->3->0->1]"),
         }) {
        std::cout << "input=" << dump_list(head);

        // auto new_head = switch_neighbors_list(head);
        auto new_head = switch_neighbors_list_2(head);

        std::cout << ", switch_neighbors_list=" << dump_list(new_head) << ", "
                  << (dump_list(new_head) == exp_v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

// leetcode 25
list_node_int_t* reverse_every_k_list(list_node_int_t* head, int k)
{
    // generic, reverse a list [left, right] inclusively
    auto reverse_list = [](list_node_int_t* left, list_node_int_t* right) {
        auto prev   = (list_node_int_t*)nullptr;
        auto ending = right->next;
        auto curr   = left;
        while (curr && curr != ending) {
            auto t     = curr->next;
            curr->next = prev;
            prev       = curr;
            curr       = t;
        }
        return curr;
    };

    list_node_int_t dummy{
        .next = head,
    };

    int  count = 0;
    auto prev  = &dummy;
    auto curr  = head;
    while (curr) {
        if (++count < k) {
            curr = curr->next;
            continue;
        }

        auto next = curr->next;
        auto left = prev->next;

        // reverse nodes between [left, curr]
        reverse_list(left, curr);

        // link the starting of sub-list to the left side list
        prev->next = curr;
        prev       = left;

        // link the ending of sub-list to the right side list
        left->next = next;

        count = 0;
        curr  = next;
    }
    return dummy.next;
}
void run_reverse_every_k_list()
{
    using use_case_t = std::tuple<list_node_int_t*, int, std::string>;

    auto create_list = [](int len) {
        list_node_int_t dummy;
        auto            node = &dummy;
        while (len--) {
            node->next = new list_node_int_t{
                .data = len,
            };
            node = node->next;
        }
        return dummy.next;
    };
    auto dump_list = [](list_node_int_t* node) {
        std::ostringstream oss;
        oss << "[";
        while (node) {
            oss << node->data;
            node = node->next;
            if (node) {
                oss << "->";
            }
        }
        oss << "]";
        return oss.str();
    };
    for (auto [head, k, exp_v] : {
             use_case_t(create_list(1), 1, "[0]"),
             use_case_t(create_list(2), 1, "[1->0]"),
             use_case_t(create_list(2), 2, "[0->1]"),
             use_case_t(create_list(3), 2, "[1->2->0]"),
             use_case_t(create_list(3), 3, "[0->1->2]"),
             use_case_t(create_list(4), 2, "[2->3->0->1]"),
             use_case_t(create_list(4), 3, "[1->2->3->0]"),
             use_case_t(create_list(4), 4, "[0->1->2->3]"),
             use_case_t(create_list(9), 3, "[6->7->8->3->4->5->0->1->2]"),
             use_case_t(create_list(9), 5, "[4->5->6->7->8->3->2->1->0]"),
             use_case_t(create_list(9), 10, "[8->7->6->5->4->3->2->1->0]"),
         }) {
        std::cout << "input=" << dump_list(head) << ", k=" << k;

        auto new_head = reverse_every_k_list(head, k);

        std::cout << ", reverse_every_k_list=" << dump_list(new_head) << ", "
                  << (dump_list(new_head) == exp_v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

// leetcode 32, the longest valid parentheses
int longest_valid_parentheses(std::string str)
{
    int                             result = 0;
    std::stack<std::pair<int, int>> st;
    for (int i = 0; i < (int)str.size(); ++i) {
        if (str[i] == '(') {
            st.push({str[i], i});
        } else if (!st.empty() && st.top().first == '(') {
            st.pop();
            // NOTICE: if st.empty(), use -1
            result = std::max(result, i - (st.empty() ? -1 : st.top().second));
        }
    }
    return result;
}
int longest_valid_parentheses_2(std::string str)
{
    int             result_start = 0;
    int             result_len   = 0;
    int             left         = INT_MAX;
    int             right        = -1;
    std::stack<int> st;
    for (int i = 0; i < (int)str.size(); ++i) {
        if (!st.empty() && str[st.top()] == '(' && str[i] == ')') {
            left  = std::min(left, st.top());
            right = i;
            st.pop();
            continue;
        }

        if (str[i] == ')') {
            if (right - left + 1 > result_len) {
                result_start = left;
                result_len   = right - left + 1;
            }
            left  = INT_MAX;
            right = -1;
        }
        st.push(i);
    }
    if (right - left + 1 > result_len) {
        result_start = left;
        result_len   = right - left + 1;
    }
    return result_len;
}
void run_longest_valid_parentheses()
{
    using use_case_t = std::pair<std::string, int>;
    for (auto [str, exp_v] : {
             use_case_t("(()", 2),
             use_case_t(")()())", 4),
             use_case_t("", 0),
             use_case_t("((((", 0),
             use_case_t("))))", 0),
             use_case_t("((()())))", 8),
             use_case_t("((((())()))))", 12),
         }) {
        /*
       const auto v1 = longest_valid_parentheses(str);
       std::cout << "Input=\"" << str << "\", longest_valid_parentheses=" << v1 << ", "
                 << (exp_v == v1 ? "SUCCESS" : "FAILED") << std::endl;
                 */
        const auto v2 = longest_valid_parentheses_2(str);
        std::cout << "Input=\"" << str << "\", longest_valid_parentheses_2=" << v2 << ", "
                  << (exp_v == v2 ? "SUCCESS" : "FAILED") << std::endl;
    }
}

// leetcode 38
std::string count_and_say(int n)
{
    std::function<std::string(int)> impl = [&impl](int n) -> std::string {
        if (n < 1) {
            return "";
        }
        if (n == 1) {
            return "1";
        }
        std::string s;
        const auto  s_n_1 = impl(n - 1);
        int         count = 1;
        for (int i = 1; i < (int)s_n_1.size(); ++i) {
            if (s_n_1[i] == s_n_1[i - 1]) {
                ++count;
            } else {
                s += std::to_string(count) + s_n_1[i - 1];
                count = 1;
            }
        }
        s += std::to_string(count) + s_n_1.back();
        return s;
    };
    return impl(n);
}
void run_count_and_say()
{
    using use_case_t = std::pair<int, std::string>;
    for (auto [n, exp_v] : {
             use_case_t(1, "1"),
             use_case_t(2, "11"),
             use_case_t(3, "21"),
             use_case_t(4, "1211"),
             use_case_t(5, "111221"),
             use_case_t(6, "312211"),
             use_case_t(7, "13112221"),
             use_case_t(8, "1113213211"),
             use_case_t(9, "31131211131221"),
             use_case_t(10, "13211311123113112211"),
         }) {
        const auto v = count_and_say(n);
        std::cout << "n=" << n << ", count_and_say=" << v << ", " << (exp_v == v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

// leetcode 41
int find_first_missing_integer(std::vector<int> arr)
{
    // NOTICE: the key point is that the missing non-zero integer must between [1, n+1], so loop the arr and put
    // elements 'x' between [1, n] in arr[x-1] = x; then loop arr again and find the missing number

    const auto len = (int)arr.size();
    int        idx = 0;
    while (idx < len) {
        // NOTICE: dont move forward until arr[idx] != idx + 1, since each swap brings a new value to arr[idx],
        // otherwise information is lost
        if (arr[idx] > 0 && arr[idx] <= len && arr[idx] != idx + 1) {
            std::swap(arr[idx], arr[arr[idx] - 1]);
        } else {
            ++idx;
        }
    }

    for (int i = 0; i < len; ++i) {
        if (arr[i] != i + 1) {
            return i + 1;
        }
    }
    return len + 1;
}
void run_find_first_missing_integer()
{
    using use_case_t = std::pair<std::vector<int>, int>;
    for (auto [arr, exp_v] : {
             use_case_t({1, 2, 0}, 3),
             use_case_t({3, 4, -1, 1}, 2),
             use_case_t({7, 8, 9, 11, 12}, 1),

         }) {
        const auto v = find_first_missing_integer(arr);
        std::cout << "find_first_missing_integer=" << v << ", " << (exp_v == v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

// leetcode 50
// O(log(x)) by using binary exponentiation
double pow_x(double x, int exp)
{
    std::function<double(double, int)> impl = [](double x, int exp) {
        double result = 1;
        // NOTICE: idea is x ^ exp = (x ^ (exp/2)) ^ 2
        while (exp / 2) {
            if (exp % 2) {
                result *= x;
            }
            x *= x;
            exp /= 2;
        }
        result *= x;
        return result;
    };
    return exp > 0 ? impl(x, exp) : (1 / impl(x, -exp));
}
void run_pow_x()
{
    using use_case_t = std::tuple<double, int, double>;
    for (auto [x, exp, exp_v] : {
             use_case_t(1, 1, 1),
             use_case_t(2, 2, 4),
             use_case_t(2, 5, 32),
             use_case_t(2, 10, 1024),
             use_case_t(2.1, 3, 9.1610),
             use_case_t(2, -2, 0.2500),
             use_case_t(2, -3, 0.12500),
         }) {
        std::cout << "x=" << x << ", exponent=" << exp << ", pow_x=" << pow_x(x, exp) << ", exp_v=" << exp_v
                  << std::endl;
    }
}

// leetcode 59
std::vector<std::vector<int>> generate_spiral_matrix(int n)
{
    std::vector<std::vector<int>> matrix(n, std::vector<int>(n, 0));
    int                           v      = 0;
    int                           rounds = 0;
    while (v < n * n) {
        ++rounds;

        int r = rounds - 1;
        int c = rounds - 1;

        // TODO: how to merge this corner case in below loops???
        if (v + 1 == n * n) {
            matrix[r][c] = ++v;
            break;
        }

        for (; c < n - rounds; ++c) {
            matrix[r][c] = ++v;
        }

        for (; r < n - rounds; ++r) {
            matrix[r][c] = ++v;
        }

        for (; c >= rounds; --c) {
            matrix[r][c] = ++v;
        }

        for (; r >= rounds; --r) {
            matrix[r][c] = ++v;
        }

        /*
        // below solution also works, but like above version better
        using pair_t = std::pair<int, int>;
        for (auto [offset_r, offset_c] : {
                 pair_t(0, 1),
                 pair_t(1, 0),
                 pair_t(0, -1),
                 pair_t(-1, 0),
             }) {
            int count = 0;
            while (count++ < n - rounds * 2 + 1) {
                matrix[r][c] = ++v;
                r += offset_r;
                c += offset_c;
            }
        }
        */
    }
    return matrix;
}
void run_generate_spiral_matrix()
{
    using use_case_t = std::tuple<int, std::string>;
    auto dump_arr    = [](const auto& vs) {
        std::ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < vs.size(); ++i) {
            oss << "[";
            for (size_t j = 0; j < vs[i].size(); ++j) {
                oss << vs[i][j];
                if (j != vs[i].size() - 1) {
                    oss << ",";
                }
            }
            oss << "]";
            if (i != vs.size() - 1) {
                oss << ",";
            }
        }
        oss << "]";
        return oss.str();
    };
    for (auto [n, exp_v] : {
             use_case_t(1, "[[1]]"),
             use_case_t(2, "[[1,2],[4,3]]"),
             use_case_t(3, "[[1,2,3],[8,9,4],[7,6,5]]"),
             use_case_t(4, "[[1,2,3,4],[12,13,14,5],[11,16,15,6],[10,9,8,7]]"),
         }) {
        const auto v = dump_arr(generate_spiral_matrix(n));
        std::cout << "n=" << n << ", generate_spiral_matrix=" << v << ", " << (exp_v == v ? "SUCCESS" : "FAILED")
                  << std::endl;
    }
}

// leetcode 80
int remove_more_than_2_dup(std::vector<int> arr)
{
    constexpr int MAX_DUPS = 2;
    int           k        = std::min((int)arr.size(), 2);
    int           idx      = 2;
    while (idx < arr.size()) {
        if (arr[idx] != arr[k - MAX_DUPS]) {
            arr[k] = arr[idx];
            ++k;
        }
        ++idx;
    }
    return k;
}
void run_remove_more_than_2_dup()
{
    using use_case_t = std::tuple<std::vector<int>, int>;
    for (auto [arr, exp_v] : {
             use_case_t({1}, 1),                               // [1]
             use_case_t({1, 2, 3}, 3),                         // [1,2,3]
             use_case_t({1, 1, 1, 2, 2, 3}, 5),                // [1,1,2,2,3]
             use_case_t({1, 1, 1, 2, 2, 2, 3, 3, 3, 3}, 6),    // [1,1,2,2,3,3]
             use_case_t({0, 0, 1, 1, 1, 1, 2, 3, 3}, 7),       // [0,0,1,1,2,3,3]
         }) {
        const auto v = remove_more_than_2_dup(arr);
        std::cout << "remove_more_than_2_dup=" << v << ", " << (v == exp_v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

// leetcode 84
// https://www.geeksforgeeks.org/dsa/largest-rectangular-area-in-a-histogram-using-stack/
int largest_area_histogram(const std::vector<int>& heights)
{
    auto dump_arr = [](const auto& vs) {
        std::ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < vs.size(); ++i) {
            oss << vs[i];
            if (i != vs.size() - 1) {
                oss << ",";
            }
        }
        oss << "]";
        return oss.str();
    };

    const int len = heights.size();

    // find the closest idx where heights[idx] is less than current heights[i] from left side
    std::vector<int> smaller_idx_left_arr(len, -1);
    {
        // NOTICE: stack holds the index rather than height
        std::stack<int> st;
        for (int i = 0; i < len; ++i) {
            while (!st.empty() && heights[st.top()] >= heights[i]) {
                st.pop();
            }
            if (!st.empty()) {
                smaller_idx_left_arr[i] = st.top();
            }
            st.push(i);
        }
    }

    // find the closest idx where heights[idx] is less than current heights[i] from right side
    std::vector<int> smaller_idx_right_arr(len, len);
    {
        // NOTICE: stack holds the index rather than height
        std::stack<int> st;
        for (int j = len - 1; j >= 0; --j) {
            while (!st.empty() && heights[st.top()] >= heights[j]) {
                st.pop();
            }
            if (!st.empty()) {
                smaller_idx_right_arr[j] = st.top();
            }
            st.push(j);
        }
    }

    // std::cout << "\nheights=" << dump_arr(heights) << ", smaller_idx_left_arr=" << dump_arr(smaller_idx_left_arr)
    //           << ", smaller_idx_right_arr=" << dump_arr(smaller_idx_right_arr) << std::endl;

    int largest_area = 0;
    for (int i = 0; i < len; ++i) {
        // at index i, the max rectangle with heights[i]
        const auto area_i = heights[i] * (smaller_idx_right_arr[i] - smaller_idx_left_arr[i] - 1);
        largest_area      = std::max(largest_area, area_i);
    }
    return largest_area;
}
void run_largest_area_histogram()
{
    using use_case_t = std::tuple<std::vector<int>, int>;
    for (auto& [heights, exp_v] : {
             use_case_t({2, 4}, 4),
             use_case_t({2, 1, 5, 6, 2, 3}, 10),
             use_case_t({3, 5, 1, 7, 5, 9}, 15),
             use_case_t({60, 20, 50, 40, 10, 50, 60}, 100),
         }) {
        const auto v = largest_area_histogram(heights);
        std::cout << "largest_area_histogram=" << v << ", " << (v == exp_v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

// leetcode 93
std::vector<std::string> find_all_possible_ips(const std::string& s)
{
    const int len = (int)s.size();
    if (len < 4 || len > 12) {
        return {};
    }
    std::vector<std::string>      ip;
    std::vector<std::string>      results;
    std::function<void(int, int)> impl = [&](int idx_start, int tokens) {
        // std::cout << "idx_start=" << idx_start << ", tokens=" << tokens << ", ip=" << ip.size() << std::endl;

        // make sure the rest length of string meets the required range
        if (tokens < 0 || len - idx_start < tokens || len - idx_start > tokens * 3) {
            return;
        }

        if (tokens == 0 && ip.size() == 4) {
            results.emplace_back(ip[0] + "." + ip[1] + "." + ip[2] + "." + ip[3]);
            return;
        }

        auto valid_token = [](const auto& token) {
            return token.size() > 0 && token.size() < 4 && std::atoi(token.c_str()) <= 255 &&
                   (token.size() == 1 || token.front() != '0');
        };

        // current token length may be 1, 2, or 3
        for (auto token_len : {1, 2, 3}) {
            if (idx_start + token_len > len) {
                // just break since we loop from 1 to 3 incrementally
                break;
            }

            const auto token = s.substr(idx_start, token_len);
            // TODO: is it possible to reduce iteration by using return value??
            // since we loop incrementally, don't have to try longer token as long as current one is invalid
            if (!valid_token(token)) {
                break;
            }

            ip.emplace_back(token);

            // TODO: is it possible to reduce iteration by using return value??
            impl(idx_start + token_len, tokens - 1);

            ip.pop_back();
        }
    };
    impl(0, 4);
    return results;
}
void run_find_all_possible_ips()
{
    auto comp_arr = [](auto& a, auto& b) {
        if (a.size() != b.size()) {
            return false;
        }
        for (auto& v : a) {
            if (auto iter = std::find_if(
                    b.begin(), b.end(),
                    [&v](auto& u) {
                        return u == v;
                    });
                iter == b.end()) {
                return false;
            }
        }
        return true;
    };
    auto dump_arr = [](const auto& vs) {
        std::ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < vs.size(); ++i) {
            oss << vs[i];
            if (i != vs.size() - 1) {
                oss << ",";
            }
        }
        oss << "]";
        return oss.str();
    };

    using use_case_t = std::tuple<std::string, std::vector<std::string>>;
    for (auto& [s, exp_v] : {
             use_case_t("0000", {"0.0.0.0"}),
             use_case_t("101023", {"1.0.10.23", "1.0.102.3", "10.1.0.23", "10.10.2.3", "101.0.2.3"}),
             use_case_t("25525511135", {"255.255.11.135", "255.255.111.35"}),
         }) {
        const auto ss = find_all_possible_ips(s);
        std::cout << "s=" << s << ", find_all_possible_ips=" << dump_arr(ss) << ", "
                  << (comp_arr(exp_v, ss) ? "SUCCESS" : "FAILED") << std::endl;
    }
}

// leetcode 98
bool validate_binary_search_tree(tree_node_int_t* root)
{
    // node: current node to be checked, check current node, then iterate left & right children
    // node_lower_boundary: the nearest ancestor on left side, so node->data > node_lower_boundary->data
    // node_upper_boundary: the nearest ancestor on right side, so node->data < node_upper_boundary->data
    std::function<bool(tree_node_int_t*, tree_node_int_t*, tree_node_int_t*)> impl =
        [&](auto* node, auto* node_lower_boundary, auto* node_upper_boundary) {
            if (!node) {
                return true;
            }

            if (node_lower_boundary && node_lower_boundary->data >= node->data) {
                return false;
            }

            if (node_upper_boundary && node_upper_boundary->data <= node->data) {
                return false;
            }

            return impl(node->left, node_lower_boundary, node) && impl(node->right, node, node_upper_boundary);
        };
    return impl(root, nullptr, nullptr);
}
bool validate_binary_search_tree_2(tree_node_int_t* root)
{
    // traverse tree by order left > root > right, so the data is in increasing order which is how the 'prev' gets used
    tree_node_int_t*                      prev = nullptr;
    std::function<bool(tree_node_int_t*)> impl = [&](auto* node) {
        if (!node) {
            return true;
        }

        if (!impl(node->left)) {
            return false;
        }

        if (prev && prev->data >= node->data) {
            return false;
        }
        prev = node;
        return impl(node->right);
    };
    return impl(root);
};
void run_validate_binary_search_tree()
{
    using use_case_t = std::tuple<tree_node_int_t*, bool>;

    auto gen_r1 = []() {    // true
        auto n1     = new tree_node_int_t{.data = 1};
        auto n2     = new tree_node_int_t{.data = 2};
        auto n3     = new tree_node_int_t{.data = 3};
        auto root   = n2;
        root->left  = n1;
        root->right = n3;
        return root;
    }();
    auto gen_r2 = []() {    // false
        auto n1     = new tree_node_int_t{.data = 1};
        auto n2     = new tree_node_int_t{.data = 2};
        auto n3     = new tree_node_int_t{.data = 3};
        auto n4     = new tree_node_int_t{.data = 4};
        auto n5     = new tree_node_int_t{.data = 5};
        auto n6     = new tree_node_int_t{.data = 6};
        auto root   = n5;
        root->left  = n1;
        root->right = n4;
        n4->left    = n3;
        n4->right   = n6;
        return root;
    }();
    auto gen_r3 = []() {    // true
        auto n1     = new tree_node_int_t{.data = 1};
        auto n2     = new tree_node_int_t{.data = 2};
        auto n3     = new tree_node_int_t{.data = 3};
        auto n5     = new tree_node_int_t{.data = 5};
        auto root   = n2;
        root->left  = n1;
        root->right = n3;
        n3->right   = n5;
        return root;
    }();
    auto gen_r4 = []() {    // false
        auto n1     = new tree_node_int_t{.data = 1};
        auto n2     = new tree_node_int_t{.data = 2};
        auto n6     = new tree_node_int_t{.data = 6};
        auto n7     = new tree_node_int_t{.data = 7};
        auto n9     = new tree_node_int_t{.data = 9};
        auto root   = n2;
        root->left  = n1;
        root->right = n7;
        n7->right   = n6;
        n6->right   = n9;
        return root;
    }();
    auto gen_r5 = []() {    // false
        auto n10    = new tree_node_int_t{.data = 10};
        auto n5     = new tree_node_int_t{.data = 5};
        auto n20    = new tree_node_int_t{.data = 20};
        auto n9     = new tree_node_int_t{.data = 9};
        auto n25    = new tree_node_int_t{.data = 25};
        auto root   = n10;
        root->left  = n5;
        root->right = n20;
        n20->left   = n9;
        n20->right  = n25;
        return root;
    }();
    auto gen_r6 = []() {    // true
        auto n5     = new tree_node_int_t{.data = 5};
        auto n3     = new tree_node_int_t{.data = 3};
        auto n8     = new tree_node_int_t{.data = 8};
        auto n2     = new tree_node_int_t{.data = 2};
        auto n4     = new tree_node_int_t{.data = 4};
        auto n9     = new tree_node_int_t{.data = 9};
        auto root   = n5;
        root->left  = n3;
        root->right = n8;
        n3->left    = n2;
        n3->right   = n4;
        n8->right   = n9;
        return root;
    }();
    auto gen_r7 = []() {    // false
        auto n5     = new tree_node_int_t{.data = 5};
        auto n3     = new tree_node_int_t{.data = 3};
        auto n8     = new tree_node_int_t{.data = 8};
        auto n2     = new tree_node_int_t{.data = 2};
        auto n6     = new tree_node_int_t{.data = 6};
        auto n9     = new tree_node_int_t{.data = 9};
        auto root   = n5;
        root->left  = n3;
        root->right = n8;
        n3->left    = n2;
        n3->right   = n6;
        n8->right   = n9;
        return root;
    }();

    for (auto [root, exp_v] : {
             use_case_t(gen_r1, true),
             use_case_t(gen_r2, false),
             use_case_t(gen_r3, true),
             use_case_t(gen_r4, false),
             use_case_t(gen_r5, false),
             use_case_t(gen_r6, true),
             use_case_t(gen_r7, false),
         }) {
        const auto v1 = validate_binary_search_tree(root);
        std::cout << std::boolalpha << "root=" << root->data << ", validate_binary_search_tree=" << v1 << ", "
                  << (v1 == exp_v ? "SUCCESS" : "FAILED") << std::endl;
        const auto v2 = validate_binary_search_tree(root);
        std::cout << std::boolalpha << "root=" << root->data << ", validate_binary_search_tree_2=" << v2 << ", "
                  << (v2 == exp_v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

int binary_tree_burn_down_time(tree_node_int_t* root, int target)
{
    if (!root) {
        return 0;
    }

    std::function<std::pair<int, int>(tree_node_int_t*, int)> impl = [&](tree_node_int_t* node, int height) {
        int height_total  = height;
        int height_target = -1;
        if (node) {
            auto [h_l, h_t_l] = impl(node->left, height + 1);
            auto [h_r, h_t_r] = impl(node->right, height + 1);
            height_total      = std::max(h_l, h_r);
            height_target     = (node->data == target ? (height + 1) : (h_t_l < 0 ? h_t_r : h_t_l));
        }
        return std::make_pair(height_total, height_target);
    };

    const auto [height_left, height_target_left]   = impl(root->left, 1);
    const auto [height_right, height_target_right] = impl(root->right, 1);

    /*
    std::cout << "height_left=" << height_left << ", height_right=" << height_right
              << ", height_target_left=" << height_target_left << ", height_target_right=" << height_target_right
              << std::endl;
    */

    if (root->data == target) {
        return std::max(height_left, height_right);
    } else if (height_target_left > 0) {
        return std::max(height_right + height_target_left - 1, height_left - height_target_left + 1);
    } else if (height_target_right > 0) {
        return std::max(height_left + height_target_right - 1, height_right - height_target_right + 1);
    }
    return 0;
}
void run_binary_tree_burn_down_time()
{
    using use_case_t = std::tuple<tree_node_int_t*, int, int>;

    auto gen_r1 = []() {
        /*
                2
               / \
              1   3
        */
        auto n1     = new tree_node_int_t{1};
        auto n2     = new tree_node_int_t{2};
        auto n3     = new tree_node_int_t{3};
        auto root   = n2;
        root->left  = n1;
        root->right = n3;
        return root;
    }();
    auto gen_r2 = []() {    // false
        /*
                5
               / \
              1   4
                 / \
                3   6
        */
        auto n1     = new tree_node_int_t{1};
        auto n2     = new tree_node_int_t{2};
        auto n3     = new tree_node_int_t{3};
        auto n4     = new tree_node_int_t{4};
        auto n5     = new tree_node_int_t{5};
        auto n6     = new tree_node_int_t{6};
        auto root   = n5;
        root->left  = n1;
        root->right = n4;
        n4->left    = n3;
        n4->right   = n6;
        return root;
    }();
    auto gen_r3 = []() {    // true
        /*
                2
               / \
              1   3
                   \
                    5
        */
        auto n1     = new tree_node_int_t{1};
        auto n2     = new tree_node_int_t{2};
        auto n3     = new tree_node_int_t{3};
        auto n5     = new tree_node_int_t{5};
        auto root   = n2;
        root->left  = n1;
        root->right = n3;
        n3->right   = n5;
        return root;
    }();
    auto gen_r4 = []() {    // false
        /*
                2
               / \
              1   7
                   \
                    6
                     \
                      9
        */
        auto n1     = new tree_node_int_t{1};
        auto n2     = new tree_node_int_t{2};
        auto n6     = new tree_node_int_t{6};
        auto n7     = new tree_node_int_t{7};
        auto n9     = new tree_node_int_t{9};
        auto root   = n2;
        root->left  = n1;
        root->right = n7;
        n7->right   = n6;
        n6->right   = n9;
        return root;
    }();
    auto gen_r5 = []() {    // false
        /*
                10
               / \
              5  20
                 / \
                9  25
        */
        auto n10    = new tree_node_int_t{10};
        auto n5     = new tree_node_int_t{5};
        auto n20    = new tree_node_int_t{20};
        auto n9     = new tree_node_int_t{9};
        auto n25    = new tree_node_int_t{25};
        auto root   = n10;
        root->left  = n5;
        root->right = n20;
        n20->left   = n9;
        n20->right  = n25;
        return root;
    }();
    auto gen_r6 = []() {    // true
        /*
                5
               / \
              3   8
             / \   \
            2   4   9
        */
        auto n5     = new tree_node_int_t{5};
        auto n3     = new tree_node_int_t{3};
        auto n8     = new tree_node_int_t{8};
        auto n2     = new tree_node_int_t{2};
        auto n4     = new tree_node_int_t{4};
        auto n9     = new tree_node_int_t{9};
        auto root   = n5;
        root->left  = n3;
        root->right = n8;
        n3->left    = n2;
        n3->right   = n4;
        n8->right   = n9;
        return root;
    }();
    auto gen_r7 = []() {    // false
        /*
                5
               / \
              3   8
             / \   \
            2   6   9
        */
        auto n5     = new tree_node_int_t{5};
        auto n3     = new tree_node_int_t{3};
        auto n8     = new tree_node_int_t{8};
        auto n2     = new tree_node_int_t{2};
        auto n6     = new tree_node_int_t{6};
        auto n9     = new tree_node_int_t{9};
        auto root   = n5;
        root->left  = n3;
        root->right = n8;
        n3->left    = n2;
        n3->right   = n6;
        n8->right   = n9;
        return root;
    }();

    for (auto [root, target, exp_v] : {
             // clang-format off
             use_case_t(gen_r1, 2, 2),
             use_case_t(gen_r1, 1, 3),
             use_case_t(gen_r1, 3, 3),
             use_case_t(gen_r2, 5, 3),
             use_case_t(gen_r2, 1, 4),
             use_case_t(gen_r2, 4, 3),
             use_case_t(gen_r2, 6, 4),
             use_case_t(gen_r3, 2, 3),
             use_case_t(gen_r3, 1, 4),
             use_case_t(gen_r3, 3, 3),
             use_case_t(gen_r3, 5, 4),
             use_case_t(gen_r4, 2, 4),
             use_case_t(gen_r4, 1, 5),
             use_case_t(gen_r4, 7, 3),
             use_case_t(gen_r4, 6, 4),
             use_case_t(gen_r4, 9, 5),
             use_case_t(gen_r5, 5, 4),
             use_case_t(gen_r6, 2, 5),
             use_case_t(gen_r7, 6, 5),
             // clang-format on
         }) {
        const auto v = binary_tree_burn_down_time(root, target);
        std::cout << "root=" << root->data << ", target=" << target << ", binary_tree_burn_down_time=" << v << ", "
                  << (exp_v == v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

bool is_palindrome(const std::string& s)
{
    const int len = s.size();
    if (len < 2) {
        return true;
    }

    int mid          = len / 2;
    int left_ending  = len / 2;
    int right_ending = (len % 2 == 0 ? (len - 1) / 2 : len / 2);
    int not_matched  = 0;
    int left         = 0;
    int right        = len - 1;
    while (left < left_ending && right > right_ending) {
        if (s[left] == s[right]) {
            ++left;
            --right;
            continue;
        }

        const auto sub_len = left_ending - left;

        // remove the not-matched character from right section, cut the sub string, reverse and compare
        auto s_left  = s.substr(left, sub_len);
        auto s_right = s.substr(right_ending, sub_len);
        std::reverse(s_right.begin(), s_right.end());
        if (s_left == s_right) {
            break;
        }

        // remove the not-matched character from left section, cut the sub string, reverse and compare
        s_left  = s.substr(left + 1, sub_len);
        s_right = s.substr(right_ending + 1, sub_len);
        std::reverse(s_right.begin(), s_right.end());
        if (s_left != s_right) {
            return false;
        }
        break;
    }
    return true;
}
void run_is_palindrome()
{
    using use_case_t = std::tuple<std::string, bool>;
    for (auto [s, exp_v] : {
             use_case_t("tacocats", true),
             use_case_t("racercar", true),
             use_case_t("kbayak", true),
             use_case_t("acbccba", true),
             use_case_t("abccbca", true),
             use_case_t("abcd", false),
             use_case_t("btnnure", false),
             use_case_t("baaba", true),
         }) {
        const auto v = is_palindrome(s);
        std::cout << "s=" << s << ", is_palindrome=" << v << ", " << (exp_v == v ? "SUCCESS" : "FAILED") << std::endl;
    }
}


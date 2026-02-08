#include <iostream>
#include <map>
#include <set>
#include <string>
#include "../../core/util.h"

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
            auto tokens = util::split(' ', commit);
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
            auto tokens = util::split(' ', query);
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
            auto tokens = util::split(' ', commit);
            std::cout << commit << std::endl;
            if (!recovery.push_commit(tokens)) {
                std::cout << "Failed to push commit: " << commit << std::endl;
            }
        }

        for (const auto& query : {
                 "0 1000000 bar.py 0d82b9",
                 "0 1000000 qux.h d139af",
             }) {
            auto tokens = util::split(' ', query);
            recovery.query(tokens);
        }
    }
}


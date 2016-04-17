#ifndef SEARCHWORKER_H
#define SEARCHWORKER_H

#include "./include.h"
#include "./IFetchWords.h"
#include "./ISearch.h"
#include "./FileFetcher.h"
#include "./WithinRangeSearch.h"

class SearchWorker {
public:
    SearchWorker() {
        _searcher = NULL;
        _wordsFetcher = NULL;
    }
    ~SearchWorker() {
        if (_searcher != NULL) {
            delete _searcher;
            _searcher = NULL;
        }
        if (_wordsFetcher != NULL) {
            delete _wordsFetcher;
            _wordsFetcher = NULL;
        }
    }
    int RunSearch(string file, string w1, string w2, int range) {
        if (file.empty() || w1.empty() || w2.empty() || range <= 0) return 0;

        createSearcher(1);
        createFetcher(1, const_cast<char*>(file.c_str()));
        int count = 0;
        auto words = _wordsFetcher->Fetch(range);
        while (1) {
            if (words.empty()) break;

            count += _searcher->Search(words, w1, w2);

            words.erase(words.begin());
            auto ws = _wordsFetcher->Fetch(1);
            copy(begin(ws), end(ws), back_inserter(words));
        }
        return count;
    }
    
private:
    // design pattern: bridge
    ISearch* _searcher;
    IFetchWords* _wordsFetcher;

    // design pattern: factory
    const ISearch& createSearcher(int searchType) {
        if (_searcher != NULL) {
            delete _searcher;
            _searcher = NULL;
        }
        switch (searchType) {
        //case 1: break;
        default:
            _searcher = new WithinRangeSearch();
            break;
        }
        return *_searcher;
    }
    const IFetchWords& createFetcher(int fetcherType, void* para) {
        if (_wordsFetcher != NULL) {
            delete _wordsFetcher;
            _wordsFetcher = NULL;
        }
        switch (fetcherType) {
        //case 1: break;
        default:
            _wordsFetcher = new FileFetcher();
            _wordsFetcher->SetSource(para);
            break;
        }
        return *_wordsFetcher;
    }
};


#endif



#ifndef GENERATE_BLINK_SIGNALS_H
#define GENERATE_BLINK_SIGNALS_H

#include "../main/header.h"

// 
/*
 * This is Bloomberg interview question.
 * Given array pos_ons that tells positions where the lights should be always on,
 * array pos_offs that tells positions where the lights should be always off,
 * output string vector that contains all possible signal.
 * 
 * Examples :
 * 
	Input: len_all = 3,
           pos_ons = {1},
           pos_offs = {},
	Output: 4
            010
            011
            110
            111
	Input: len_all = 5,
           pos_ons = {0,1},
           pos_offs = {3},
	Output: 4
            00011
            00111
            10011
            10111
 * 
 */

unsigned int generate_blink_signal(unsigned int len_all,
                                   const vector<unsigned int>& pos_ons,
                                   const vector<unsigned int>& pos_offs,
                                   vector<string>& blink_signals)
{
    unsigned int count_possible = pow(2, len_all - pos_ons.size() - pos_offs.size());
    if (count_possible < 1) return -1;

    // convert pos_ons into vector with index as position
    unordered_set<unsigned int> pos_on_hash;
    for_each(pos_ons.begin(),
             pos_ons.end(),
             [&pos_on_hash](unsigned int v)
             {
                pos_on_hash.insert(v);
             });
    // convert pos_offs into vector with index as position
    unordered_set<unsigned int> pos_off_hash;
    for_each(pos_offs.begin(),
             pos_offs.end(),
             [&pos_off_hash](unsigned int v)
             {
                pos_off_hash.insert(v);
             });

    string signal(len_all, '0');
    // keep those always on positions '1'
    for_each(pos_ons.begin(),
             pos_ons.end(),
             [&signal](unsigned int pos)
             {
                signal[signal.size()-pos-1] = '1';
             });

    unsigned int count = 0;
    while (count++ < count_possible)
    {
        unsigned int j = len_all;
        while (j--)
        {
            // current bit's position which is consistent with pos_ons/pos_offs
            auto pos = len_all-j-1;
            // bypass those always on or off positions
            if (pos_on_hash.end()  != pos_on_hash.find(pos) || 
                pos_off_hash.end() != pos_off_hash.find(pos))
            {
                continue;
            }

            if (signal[j] == '0')
            {
                // change bit and this is a new signal!
                signal[j] = '1';
                break;
            }
            else
            {
                // need to carry to upper bit
                signal[j] = '0';
            }
        }
        // add latest generated signal to container
        blink_signals.push_back(signal);
    }
    
    assert(blink_signals.size() == pow(2, len_all - pos_ons.size() - pos_offs.size()));
    return blink_signals.size();
}

void run_generate_blink_signals()
{
    unsigned int len_all = 1;
    vector<unsigned int> pos_ons = {};
    vector<unsigned int> pos_offs = {};
    
    auto lam_blink = [&]() {
        cout << "---------------------------------" << endl;

        cout << "len_all: " << len_all << endl;

        vector<string> blink_signals;
        cout << "pos_ons: ";
        for_each(pos_ons.begin(), pos_ons.end(), [](unsigned int v) {
            cout << v << ",";
        });
        cout << "\npos_offs: ";
        for_each(pos_offs.begin(), pos_offs.end(), [](unsigned int v) {
            cout << v << ",";
        });
        cout << "\n# of signals: "
             << generate_blink_signal(len_all,
                                      pos_ons,
                                      pos_offs,
                                      blink_signals)
             << endl;
        for_each(blink_signals.begin(), blink_signals.end(), [](const string& s) {
            cout << '\t' << s << '\n';
        });
    };
    lam_blink();

    len_all = 1;
    pos_ons = {0};
    pos_offs = {};
    lam_blink();

    len_all = 3;
    pos_ons = {0};
    pos_offs = {};
    lam_blink();
    
    len_all = 4;
    pos_ons = {1};
    pos_offs = {3};
    lam_blink();

    len_all = 5;
    pos_ons = {0};
    pos_offs = {1,3};
    lam_blink();
 
    len_all = 6;
    pos_ons = {2};
    pos_offs = {1,4};
    lam_blink();

    len_all = 10;
    pos_ons = {2,7};
    pos_offs = {1,4,8};
    lam_blink();
}

#endif


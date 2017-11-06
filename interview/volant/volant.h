#ifndef VOLANT_H
#define VOLANT_H

#include "../../main/header.h"

// 
// Given a string which consists of number, space, and dash, normalize it as a phone number
// string s1 = "   0  0-44  4  8 5555 8361      ";
// string s2 = " 0 - 22 1985--32   4";
// string s3 = "5310   ";
string normalize_phone_number(string& S)
{
    char arr[S.size()] = {'\0'};
    unsigned int len_total = 0;
    unsigned int len_digits = 0;
    for (unsigned int i = 0; i < S.size(); ++i)
    {
        char ch = S.at(i);
        if (ch != ' ' && ch != '-')
        {
            if (len_digits != 0 && len_digits % 3 == 0)
            {
                arr[len_total++] = '-';
            }
            arr[len_total++] = ch;
            ++len_digits;
        }
    }
    if (len_digits % 3 == 1)
    {
        char digit = arr[len_total-3];
        arr[len_total-3] = arr[len_total-2];
        arr[len_total-2] = digit;
    }
    return string(arr);
}

// 
// Given a series of commands {number, DUP, POP, +, -}, process the command and return the value finally
// string s1 = "13 DUP 4 POP 5 DUP + DUP + -";
// string s2 = "5 6 + -";
// string s3 = "3 DUP 5 - -";
int process_commands(string& S)
{
    static int MAX_NUM = pow(2, 20) - 1;
    static int MIN_NUM = 0;

    stack<int> st;
    size_t pos_s = 0;
    size_t pos_e = 0;
    while (1)
    {
        pos_e = S.find(' ', pos_s);
        if (pos_e == string::npos)
        {
            if (pos_s < S.size())
                pos_e = S.size();
            else
                break;
        }

        string op = S.substr(pos_s, pos_e - pos_s);
        pos_s = pos_e + 1;

        if (op == "POP")
        {
            if (!st.size()) return -1;
            st.pop();
        }
        else if (op == "DUP")
        {
            if (!st.size()) return -1;
            st.push(st.top());
        }
        else if (op == "+")
        {
            if (st.size() < 2) return -1;
            int v1 = st.top(); st.pop();
            int v2 = st.top(); st.pop();
            int v = v1 + v2;
            if (v > MAX_NUM) return -1;
            st.push(v);
        }
        else if (op == "-")
        {
            if (st.size() < 2) return -1;
            int v1 = st.top(); st.pop();
            int v2 = st.top(); st.pop();
            int v = v1 - v2;
            if (v < MIN_NUM) return -1;
            st.push(v);
        }
        else
        {
            int num = stoi(op);
            st.push(num);
        }
    }
    return st.size() > 0 ? st.top() : -1;
}

#endif


#ifndef HRT_H
#define HRT_H

#include "handler.h"

void
print_usage()
{
    string usage = 
    "Usage:\n"
    "./main_exe [--help] --file='/your/file/path' [--cols=col1,col2,col3] [--operations='col1,col2,+|col3,col4,x'] [--stats=col1,col2] [--join='/your/file/path,col1,col2']"
    "\n\n"
    "Arguments:\n"
    "  --help         Show this help message.\n"
    "  --file         Input csv file's full path.\n"
    "  --cols         Target output columns' zero based index, seprated by comma.\n"
    "  --operations   Operations that are applied to two columns +/-/x//, separated by '|'.\n"
    "  --stats        Run statistics max/min/average/median on the columns, separated by comma.\n"
    "  --join         Target join csv file's full path, and primary column indexes.\n\n"
    "  PLEASE go to README for more details.\n\n";

    std::cout << usage;
}

void run_hrt(int argc, char **argv)
{
    // 
    // > ./main_exe --file='0.csv' --cols=1,2,4 --operations='2,4,+|2,5,x' --stats=2,4 --join='1.csv,1,2'
    // 

    if (argc == 1)
    {
        print_usage();
        exit(0);
    }

    string file;
    set<int> fixed_output_cols;
    vector<tuple<int, int, operator_2_cols>> operations;
    vector<int> stats_cols;
    string join_file;
    pair<int, int> join_cols;

    for (int i = 1; i < argc; ++i)
    {
        string s = argv[i];
        int pos = s.find('=');
        string name = s.substr(0, pos);
        string value = s.substr(pos+1, s.size()-pos);
        if (name == "--help")
        {
            print_usage();
            exit(0);
        }
        else if (name == "--file")
        {
            file = value;
        }
        else if (name == "--cols")
        {
            int p0 = 0;
            int p1 = value.find(',', p0);
            while (p0 < (int)value.size())
            {
                p1 = p1 < 0 ? value.size() : p1;
                fixed_output_cols.insert(stoi(value.substr(p0, p1-p0)));
                p0 = p1 + 1;
                p1 = value.find(',', p0);
            }
        }
        else if (name == "--operations")
        {
            int p0 = 0;
            int p1 = value.find('|', p0);
            while (p0 < (int)value.size())
            {
                p1 = p1 < 0 ? value.size() : p1;

                string opera = value.substr(p0, p1-p0);
                int pp1 = opera.find(',');
                int col1 = stoi(opera.substr(0, pp1));
                int pp2 = opera.find(',', pp1+1);
                int col2 = stoi(opera.substr(pp1+1, pp2-pp1-1));
                char op = opera.at(opera.size()-1);
                operator_2_cols oper;
                switch (op)
                {
                    case '+':
                        oper = operator_2_cols::ADD;
                        break;
                    case '-':
                        oper = operator_2_cols::SUBTRACTION;
                        break;
                    case 'x':
                        oper = operator_2_cols::MULTIPLICATION;
                        break;
                    case '/':
                        oper = operator_2_cols::DIVISION;
                        break;
                    default:
                        throw invalid_argument("Invalid operator");
                }
                operations.emplace_back(std::make_tuple(col1, col2, oper));
                p0 = p1 + 1;
                p1 = value.find('|', p0);
            }
        }
        else if (name == "--stats")
        {
            int p0 = 0;
            int p1 = value.find(',', p0);
            while (p0 < (int)value.size())
            {
                p1 = p1 < 0 ? value.size() : p1;

                stats_cols.emplace_back(stoi(value.substr(p0, p1-p0)));
                p0 = p1 + 1;
                p1 = value.find(',', p0);
            }
        }
        else if (name == "--join")
        {
            int p0 = 0;
            int p1 = value.find(',', p0);
            join_file = value.substr(p0, p1-p0);
            p0 = p1 + 1;
            p1 = value.find(',', p0);
            join_cols.first = stoi(value.substr(p0, p1-p0));
            p0 = p1 + 1;
            join_cols.second = stoi(value.substr(p0, value.size()-p0));
        }
    }

    if (!fixed_output_cols.empty())
    {
        auto results = handle_fixed_output_cols(file,
                                                fixed_output_cols);
        print_results("cols.csv", results);
    }
    if (!operations.empty())
    {
        auto results = handle_operations(file,
                                         operations);
        print_results("operations.csv", results);
    }
    if (!stats_cols.empty())
    {
        auto results = handle_stats(file, stats_cols);
        print_results_stats("stats.csv", results);
    }
    if (!join_file.empty())
    {
        string output_inner = "inner.csv";
        string output_outer = "outer.csv";
        handle_join(file, join_cols.first,
                    join_file, join_cols.second,
                    output_inner,
                    output_outer);
    }
    exit(0);
}

#endif


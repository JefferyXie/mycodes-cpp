#ifndef HANDLER_H
#define HANDLER_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <utility>
#include <tuple>
#include <limits>
#include <map>
#include <unordered_map>

#include "csv_file_reader.h"

using namespace std;

enum class operator_2_cols : char
{
    ADD,
    SUBTRACTION,
    DIVISION,
    MULTIPLICATION,
};

// meet requirement 2): output specified cols
vector<vector<string>>
handle_fixed_output_cols(const string& file,
                         const set<int>& fixed_output_cols);

// meet requirement 3): operation on two cols
vector<vector<string>>
handle_operations(const string& file,
                  const vector<tuple<int, int, operator_2_cols>>& operations);

// meet requirement 4): statistics max/min/avg/median on specified cols
map<int, vector<double>>
handle_stats(const string& file,
             const vector<int>& stats_cols);

void
handle_median(map<int, vector<double>>& stats,
              const map<int, vector<double>>& cached_cols_for_median);

// meet requirement 5): inner & outer join the two data sets
void
handle_join(const string& file_1,
            int col_1,
            const string& file_2,
            int col_2,
            const string& output_innner_file,
            const string& output_outer_file);

// parse csv file, calculate & cache necessary data fields for use
vector<vector<string>>
handle_csv_file(const string& file,
                const set<int>& fixed_output_cols,
                const vector<tuple<int, int, operator_2_cols>>& operations,
                const vector<int>& stats_cols,
                map<int, vector<double>>& results_stats,
                pair<int, unordered_map<string, string>>& join_arg,
                int& total_cols);

// algorithm, find median
double
find_median(vector<double>& source, int pos_s, int pos_e);

// dump data fields into csv file
void
print_results(const string& file_name,
              const std::vector<std::vector<std::string>>& results);

// dump stats data into csv file
void
print_results_stats(const string& file_name,
                    const std::map<int, std::vector<double>>& results);

// utility function, convert string into double
double str_to_double(const string& s)
{
    double db = 0.0;
    try {
        db = stod(s);
    } catch (...) {}
    return db;
}

vector<vector<string>>
handle_fixed_output_cols(const string& file,
                         const set<int>& fixed_output_cols)
{
    // dummy variables
    vector<tuple<int, int, operator_2_cols>> dummy_operations;
    vector<int> dummy_stats_cols;
    pair<int, unordered_map<string, string>> dummy_join_arg;
    dummy_join_arg.first = -1;
    int dummy_total_cols;
    map<int, vector<double>> dummy_results_stats;
    
    auto results = handle_csv_file(file,
                                   fixed_output_cols,
                                   dummy_operations,
                                   dummy_stats_cols,
                                   dummy_results_stats,
                                   dummy_join_arg,
                                   dummy_total_cols);
    return results;
}

vector<vector<string>>
handle_operations(const string& file,
                  const vector<tuple<int, int, operator_2_cols>>& operations)
{
    // dummy variables
    set<int> dummy_fixed_output_cols;
    vector<int> dummy_stats_cols;
    pair<int, unordered_map<string, string>> dummy_join_arg;
    dummy_join_arg.first = -1;
    int dummy_total_cols;
    map<int, vector<double>> dummy_results_stats;
    
    auto results = handle_csv_file(file,
                                   dummy_fixed_output_cols,
                                   operations,
                                   dummy_stats_cols,
                                   dummy_results_stats,
                                   dummy_join_arg,
                                   dummy_total_cols);
    return results;
}

map<int, vector<double>>
handle_stats(const string& file,
             const vector<int>& stats_cols)
{
    // dummy variables
    set<int> dummy_fixed_output_cols;
    vector<tuple<int, int, operator_2_cols>> dummy_operations;
    pair<int, unordered_map<string, string>> dummy_join_arg;
    dummy_join_arg.first = -1;
    int dummy_total_cols;
    map<int, vector<double>> results_stats;

    handle_csv_file(file,
                    dummy_fixed_output_cols,
                    dummy_operations,
                    stats_cols,
                    results_stats,
                    dummy_join_arg,
                    dummy_total_cols);
    return results_stats;
}

void
handle_median(map<int, vector<double>>& stats,
              const map<int, vector<double>>& cached_cols_for_median)
{
    for (auto& cached_entry : cached_cols_for_median)
    {
        auto& cached_cols = cached_entry.second;
        auto stat_median = find_median(const_cast<vector<double>&>(cached_cols),
                                       0,
                                       (int)cached_cols.size());

        int i = cached_entry.first;
        stats[i].emplace_back(stat_median);
    }
}

void
handle_join(const string& file_1,
            int col_1,
            const string& file_2,
            int col_2,
            const string& output_innner_file,
            const string& output_outer_file)
{
    // dummy variables
    set<int> dummy_fixed_output_cols;
    vector<tuple<int, int, operator_2_cols>> dummy_operations;
    vector<int> dummy_stats_cols;
    map<int, vector<double>> dummy_resuluts_stats;

    // join data sets
    pair<int, unordered_map<string, string>> join_arg_1;
    join_arg_1.first = col_1;
    int total_cols_1;
    handle_csv_file(file_1,
                    dummy_fixed_output_cols,
                    dummy_operations,
                    dummy_stats_cols,
                    dummy_resuluts_stats,
                    join_arg_1,
                    total_cols_1);
    pair<int, unordered_map<string, string>> join_arg_2;
    join_arg_2.first = col_2;
    int total_cols_2;
    handle_csv_file(file_2,
                    dummy_fixed_output_cols,
                    dummy_operations,
                    dummy_stats_cols,
                    dummy_resuluts_stats,
                    join_arg_2,
                    total_cols_2);

    // inner join
    std::ofstream ofs_inner;
    ofs_inner.open (output_innner_file, std::ofstream::out | std::ofstream::trunc);
    bool first_record = true;
    for (auto& entry : join_arg_1.second)
    {
        auto it_entry_2 = join_arg_2.second.find(entry.first);
        if (it_entry_2 != join_arg_2.second.end())
        {
            if (!first_record)
                ofs_inner << '\n';
            ofs_inner << entry.first + ',' + entry.second + ',' + it_entry_2->second;
            first_record = false;
        }
    }
    ofs_inner.close();

    // outer join
    std::ofstream ofs_outer;
    ofs_outer.open (output_outer_file, std::ofstream::out | std::ofstream::trunc);
    first_record = true;
    for (auto& entry : join_arg_1.second)
    {
        if (!first_record)
            ofs_outer << '\n';

        ofs_outer << entry.first + ',' + entry.second;

        auto it_entry_2 = join_arg_2.second.find(entry.first);
        if (it_entry_2 != join_arg_2.second.end())
        {
            ofs_outer << ',' + it_entry_2->second;

            join_arg_2.second.erase(entry.first);
        }
        else
        {
            std::string commas(total_cols_2, ',');
            ofs_outer << commas;
        }
        first_record = false;
    }
    for (auto& entry : join_arg_2.second)
    {
        std::string commas(total_cols_1, ',');
        ofs_outer << "\n" + entry.first + commas + entry.second;
    }
    ofs_outer.close();
}

vector<vector<string>>
handle_csv_file(const string& file,
                const set<int>& fixed_output_cols,
                const vector<tuple<int, int, operator_2_cols>>& operations,
                const vector<int>& stats_cols,
                map<int, vector<double>>& results_stats,
                pair<int, unordered_map<string, string>>& join_arg,
                int& total_cols)
{
    // load csv file(s)
    csv_file_reader csv_1(file);
    
    int results_total_cols = fixed_output_cols.size() + operations.size();
    vector<vector<string>> results(results_total_cols);

    // container to cache fields that each operation needs
    vector<pair<double, double>> operations_cached_fields{operations.size()};
    // cache raw data to calculate median at end
    map<int, vector<double>> cached_cols_for_median;
    for (auto col : stats_cols)
    {
        // cache intermediate max/min/avg
        results_stats.emplace(col,
                              vector<double>{
                                std::numeric_limits<double>::lowest(),// for stat max
                                std::numeric_limits<double>::max(),   // for stat min
                                0.0                                   // for stat avg
                                });
    }
    int col = 0;
    string key_field;
    string all_fields_exclude_key = "";
    string field;
    while (csv_1.read(field))
    {
        // to meet requirement 2): output desired column(s)
        auto it_fixed = fixed_output_cols.find(col);
        if (it_fixed != fixed_output_cols.end())
        {
            // get the col's container
            int index = std::distance(fixed_output_cols.begin(), it_fixed);
            results[index].emplace_back(field);
        }

        // to meet requirement 3): operation on two columns
        int col1, col2;
        operator_2_cols opera;
        for (int i = 0; i < (int)operations.size(); ++i)
        {
            tie(col1, col2, opera) = operations.at(i);
            // cache the field value and execute operation at the end of record
            if (col1 == col)
                operations_cached_fields[i].first = str_to_double(field);
            else if (col2 == col)
                operations_cached_fields[i].second = str_to_double(field);
        }

        // to meet requirement 4): statistics min/max/median/avg
        auto it_stats = results_stats.find(col);
        if (it_stats != results_stats.end())
        {
            // assume the field value type is double
            auto value = str_to_double(field);
            auto stat_max = it_stats->second[0];
            auto stat_min = it_stats->second[1];
            auto stat_avg = it_stats->second[2];

            it_stats->second[0] = stat_max > value ? stat_max : value;
            it_stats->second[1] = stat_min < value ? stat_min : value;
            auto& cached_cols = cached_cols_for_median[col];
            it_stats->second[2] = (stat_avg*cached_cols.size() + value) / (cached_cols.size()+1);
            cached_cols.emplace_back(value);
        }

        // to meet requirement 5): join data sets
        if (join_arg.first >= 0)
        {
            // keep all fields except key since key will be moved as first field
            if (col != join_arg.first)
            {
                if (col == 0)
                    all_fields_exclude_key += field;
                else
                    all_fields_exclude_key += ',' + field;
            }
            else
            {
                key_field = field;
            }
        }

        ++col;
        // current record ends
        if (csv_1.is_record_end())
        {
            // to meet requirement 3): execute the operation with cached fields
            for (int i = 0; i < (int)operations.size(); ++i)
            {
                // column container from the results
                auto& results_col = results.at(fixed_output_cols.size()+i);

                double result_field = 0.0;
                const auto& cached_fields = operations_cached_fields[i];
                operator_2_cols opera = std::get<2>(operations.at(i));
                switch (opera)
                {
                    case operator_2_cols::ADD:
                        result_field = cached_fields.first + cached_fields.second;
                        break;
                    case operator_2_cols::SUBTRACTION:
                        result_field = cached_fields.first - cached_fields.second;
                        break;
                    case operator_2_cols::DIVISION:
                        result_field = cached_fields.first / cached_fields.second;
                        break;
                    case operator_2_cols::MULTIPLICATION:
                        result_field = cached_fields.first * cached_fields.second;
                        break;
                    default:
                        throw invalid_argument("Unknown operator");
                }
                char buf[64] = {'\0'};
                std::sprintf(buf, "%.2f", result_field);
                results_col.emplace_back(buf);
            }

            // to meet requirement 5): join data sets
            if (join_arg.first >= 0)
            {
                auto& csv_entries = join_arg.second;
                csv_entries.emplace(key_field, all_fields_exclude_key);
            }
 
            total_cols = col;
            col = 0;
            all_fields_exclude_key = "";
        }
    }
    handle_median(results_stats, cached_cols_for_median);
    return results;
}

double
find_median(vector<double>& source, int pos_s, int pos_e)
{
    // max value on the left array (smaller than the flag_value)
    double max_on_left_array = std::numeric_limits<double>::min();
    int j = pos_s;
    while (j < pos_e)
    {
        auto flag_value = source[j];
        for (int k = j+1; k < pos_e; ++k)
        {
            auto value = source[k];
            if (value < flag_value)
            {
                source[j] = value;
                source[k] = flag_value;
                if (k > j+1)
                {
                    source[k] = source[j+1];
                    source[j+1] = flag_value;
                }
                j = j+1;

                max_on_left_array = max_on_left_array > value ?
                                    max_on_left_array : value;
            }
        }
        int length_total = (int)source.size();
        // check if current flag_value's pos is the middle point
        if (j == length_total/2)
        {
            auto med = source[j];
            // if total length is even, find the max value on left array
            // and average with med; if total is odd, med is the answer
            if (length_total % 2 == 0)
            {
                med = (med + max_on_left_array)/2;
            }
            return med;
        }
        else if (j > length_total/2)
        {
            // sort the array on the left side: smaller values than flag_value
            return find_median(source, pos_s, j);
        }
        else
        {
            // sort the array on the right side: bigger values than flag_value
            return find_median(source, j+1, pos_e);
        }
    }
    return source[j];
}

void
print_results(const string& file_name,
              const std::vector<std::vector<std::string>>& results)
{
    std::ofstream ofs;
    ofs.open (file_name, std::ofstream::out | std::ofstream::trunc);

    int cols = (int)results.size();
    int rows = cols > 0 ? results[0].size() : 0;
    int row = 0;
    while (row < rows)
    {
        int col = 0;
        while (col < cols)
        {
            if ((int)results[col].size() > row)
            {
                if (col)
                    ofs << ',';
                ofs  << results[col][row];
            }
            ++col;
        }
        ofs << '\n';
        ++row;
    }
    ofs.close();
}

void
print_results_stats(const string& file_name,
                    const std::map<int, std::vector<double>>& results)
{
    std::ofstream ofs;
    ofs.open (file_name, std::ofstream::out | std::ofstream::trunc);

    string tag[4] = {"max", "min", "avg", "median"};
    int rows = 4;
    int row = 0;
    while (row < rows)
    {
        bool first_field = true;
        for (auto& entry : results)
        {
            std::cout.width(10);
            int col = entry.first;
            if (first_field)
            {
                ofs << tag[row];
            }
            ofs << ',';
            ofs << std::fixed << results.at(col)[row];

            first_field = false;
            ++col;
        }
        ofs << '\n';
        ++row;
    }
    ofs.close();
}

#endif


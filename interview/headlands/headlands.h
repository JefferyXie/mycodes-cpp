#ifndef HEADLANDS_H
#define HEADLANDS_H

#include <cstring>
#include <cstdint>
#include <istream>
#include <iterator>
#include <ostream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <utility>

class csv_reader final
{
public:
    csv_reader(std::string_view file)
    {
        if (ifs_.open(file.data()); !ifs_.is_open()) {
            throw std::invalid_argument("Failed to open file: " + std::string{file});
        }
    }
    ~csv_reader() { ifs_.close(); }
    size_t read(char* buf, size_t len)
    {
        std::memset(buf, '\0', len);
        ifs_.read(buf, len);
        return ifs_.gcount();
    }
    bool read_line(std::string& line)
    {
        if (!ifs_.good())
            return false;

        line.clear();
        std::getline(ifs_, line);
        return !ifs_.fail();
    }
    bool read_line_fields(std::vector<std::string>& fields, char delimiter)
    {
        fields.clear();
        if (std::string line; read_line(line)) {
            std::istringstream ss{line};
            for (std::string token; std::getline(ss, token, delimiter);) {
                fields.emplace_back(std::move(token));
            }
            return true;
        }
        return false;
    }

private:
    std::ifstream ifs_;
};

class date final
{
public:
    // date(std::string_view yyyymmdd, char delimiter = '\0')
    date(std::string_view yyyymmdd, char delimiter = '-')
    {
        if (delimiter == '\0') {
            if (yyyymmdd.size() != 8u) {
                throw std::invalid_argument("Invalid date string: " + std::string{yyyymmdd});
            }
            year_  = static_cast<uint32_t>(std::atoi(std::string{yyyymmdd.substr(0, 4)}.c_str()));
            month_ = static_cast<uint8_t>(std::atoi(std::string{yyyymmdd.substr(4, 2)}.c_str()));
            day_   = static_cast<uint8_t>(std::atoi(std::string{yyyymmdd.substr(6, 2)}.c_str()));
        } else {
            if (yyyymmdd.size() != 10u) {
                throw std::invalid_argument("Invalid date string: " + std::string{yyyymmdd});
            }
            year_  = static_cast<uint32_t>(std::atoi(std::string{yyyymmdd.substr(0, 4)}.c_str()));
            month_ = static_cast<uint8_t>(std::atoi(std::string{yyyymmdd.substr(5, 2)}.c_str()));
            day_   = static_cast<uint8_t>(std::atoi(std::string{yyyymmdd.substr(8, 2)}.c_str()));
        }
        if (year_ == 0 || month_ == 0 || day_ == 0 || month_ > 12 || day_ > 31) {
            throw std::invalid_argument("Invalid date string: " + std::string{yyyymmdd});
        }
    }
    bool operator==(const date& other) const
    {
        return year_ == other.year_ && month_ == other.month_ && day_ == other.day_;
    }
    bool operator>(const date& other) const
    {
        return year_ > other.year_ || (year_ == other.year_ && month_ > other.month_) ||
               (year_ == other.year_ && month_ == other.month_ && day_ > other.day_);
    }
    bool operator<(const date& other) const
    {
        return year_ < other.year_ || (year_ == other.year_ && month_ < other.month_) ||
               (year_ == other.year_ && month_ == other.month_ && day_ < other.day_);
    }

    friend std::ostream& operator<<(std::ostream& os, const date& dt);

private:
    uint32_t year_{0};
    uint8_t  month_{0};
    uint8_t  day_{0};
};
std::ostream& operator<<(std::ostream& os, const date& dt)
{
    // clang-format off
    os << std::setfill('0') << std::setw(4) << dt.year_
       << "-"
       << std::setfill('0') << std::setw(2) << static_cast<uint32_t>(dt.month_)
       << "-"
       << std::setfill('0') << std::setw(2) << static_cast<uint32_t>(dt.day_);
    // clang-format on
    return os;
}

void run_date()
{
    date dt1{"2010-01-01", '-'};
    date dt2{"1999-12-31", '-'};
    date dt3{"2020-11-01", '-'};
    date dt4{"20100101"};
    date dt5{"19991231"};
    date dt6{"20201101"};
    date dt7{"2010/01/01", '/'};
    date dt8{"1999/12/31", '/'};
    date dt9{"2020/11/01", '/'};

    std::cout << dt1 << " > " << dt2 << ": " << std::boolalpha << (dt1 > dt2) << std::endl;
    std::cout << dt1 << " > " << dt3 << ": " << std::boolalpha << (dt1 > dt3) << std::endl;
    std::cout << dt1 << " > " << dt4 << ": " << std::boolalpha << (dt1 > dt4) << std::endl;
    std::cout << dt1 << " > " << dt5 << ": " << std::boolalpha << (dt1 > dt5) << std::endl;
    std::cout << dt1 << " > " << dt6 << ": " << std::boolalpha << (dt1 > dt6) << std::endl;
    std::cout << dt1 << " > " << dt7 << ": " << std::boolalpha << (dt1 > dt7) << std::endl;
    std::cout << dt1 << " > " << dt8 << ": " << std::boolalpha << (dt1 > dt8) << std::endl;
    std::cout << dt1 << " > " << dt9 << ": " << std::boolalpha << (dt1 > dt9) << std::endl;

    std::cout << dt1 << " < " << dt2 << ": " << std::boolalpha << (dt1 < dt2) << std::endl;
    std::cout << dt1 << " < " << dt3 << ": " << std::boolalpha << (dt1 < dt3) << std::endl;
    std::cout << dt1 << " < " << dt4 << ": " << std::boolalpha << (dt1 < dt4) << std::endl;
    std::cout << dt1 << " < " << dt5 << ": " << std::boolalpha << (dt1 < dt5) << std::endl;
    std::cout << dt1 << " < " << dt6 << ": " << std::boolalpha << (dt1 < dt6) << std::endl;
    std::cout << dt1 << " < " << dt7 << ": " << std::boolalpha << (dt1 < dt7) << std::endl;
    std::cout << dt1 << " < " << dt8 << ": " << std::boolalpha << (dt1 < dt8) << std::endl;
    std::cout << dt1 << " < " << dt9 << ": " << std::boolalpha << (dt1 < dt9) << std::endl;

    std::cout << dt1 << " == " << dt2 << ": " << std::boolalpha << (dt1 == dt2) << std::endl;
    std::cout << dt1 << " == " << dt3 << ": " << std::boolalpha << (dt1 == dt3) << std::endl;
    std::cout << dt1 << " == " << dt4 << ": " << std::boolalpha << (dt1 == dt4) << std::endl;
    std::cout << dt1 << " == " << dt5 << ": " << std::boolalpha << (dt1 == dt5) << std::endl;
    std::cout << dt1 << " == " << dt6 << ": " << std::boolalpha << (dt1 == dt6) << std::endl;
    std::cout << dt1 << " == " << dt7 << ": " << std::boolalpha << (dt1 == dt7) << std::endl;
    std::cout << dt1 << " == " << dt8 << ": " << std::boolalpha << (dt1 == dt8) << std::endl;
    std::cout << dt1 << " == " << dt9 << ": " << std::boolalpha << (dt1 == dt9) << std::endl;
}

void run_csv_reader()
{
    {
        std::cout << "---csv_reade::read(char*, size_t)---" << std::endl;

        csv_reader reader{"./interview/headlands/test.csv"};
        int        times = 0;
        char       buf[5];
        while (true) {
            const auto count = reader.read(buf, sizeof(buf) / sizeof(buf[0]));
            if (count == 0)
                break;

            std::cout << "[" << times++ << "] read '" << count << "': " << buf << std::endl;
        }
    }

    {
        std::cout << "---csv_reade::read_line(std::string&)---" << std::endl;

        csv_reader  reader{"./interview/headlands/test.csv"};
        int         times = 0;
        std::string line;
        while (reader.read_line(line)) {
            std::cout << "[" << times++ << "]: " << line << std::endl;
        }
    }

    {
        std::cout << "---csv_reade::read_line_fields(.., ',')---" << std::endl;

        csv_reader               reader{"./interview/headlands/test.csv"};
        int                      times = 0;
        std::vector<std::string> fields;
        while (reader.read_line_fields(fields, ',')) {
            std::cout << "[" << times++ << "]: size=" << fields.size() << ", ";
            // print_array(fields);
            std::cout << std::endl;
        }
    }

    {
        std::cout << "---csv_reade::read_line_fields(.., '|')---" << std::endl;

        csv_reader               reader{"./interview/headlands/test.csv"};
        int                      times = 0;
        std::vector<std::string> fields;
        while (reader.read_line_fields(fields, '|')) {
            std::cout << "[" << times++ << "]: size=" << fields.size() << ", ";
            // print_array(fields);
            std::cout << std::endl;
        }
    }
}

struct my_job {
    std::string date;
    std::string name;
    std::string next_job;

    bool valid() { return !date.empty() && !name.empty(); }

    friend std::istream& operator>>(std::istream& is, my_job& job)
    {
        /* read from std::cin
        my_job j;
        while (std::cin >> j) {
            jobs.emplace_back(std::move(j));
        }

        // with std::cin, the std::getline(ss, job.date, ',') keep reading even new line break, this is expected but
        // causes last job line is not read well; to solve this, we std::getline from std::cin first and covert it into
        // different stream
        std::string line;
        std::getline(is, line);
        std::istringstream ss{line};

        std::getline(ss, job.name, ',');
        std::getline(ss, job.date, ',');
        std::getline(ss, job.next_job);
        if (!job.date.empty() && job.date.back() == '\n') {
            job.date.erase(job.date.size() - 1);
        }

        // alternatively, just split the 'line' directly
        auto split = [](std::string_view str, char delimiter) {
            size_t                   last = 0;
            size_t                   next = 0;
            std::vector<std::string> tokens;
            while ((next = str.find(delimiter, last)) != string::npos) {
                tokens.emplace_back(str.substr(last, next - last));
                last = next + 1;
            }
            tokens.emplace_back(str.substr(last));
            return tokens;
        };
        */

        std::string line;
        std::getline(is, line);
        size_t start = 0;
        for (auto field : {&job.name, &job.date, &job.next_job}) {
            size_t pos = line.find(',', start);
            if (pos == std::string::npos) {
                *field = line.substr(start);
                break;
            }

            *field = line.substr(start, pos - start);
            start  = pos + 1;
        }

        /*
        std::ifstream ifs;
        ifs.open("./interview/headlands/jobs.csv");

        two ways to load data from csv file:
        1) explict while loop of the file stream
        while (ifs.good()) {
            jobs.emplace_back();
            ifs >> jobs.back();
        }

        2) utilize std::istream_iterator
        std::istream_iterator<my_job> bg(ifs);
        std::istream_iterator<my_job> ed;
        std::copy(bg, ed, std::back_inserter(jobs));
        */
        /*
        std::getline(is, job.name, ',');
        std::getline(is, job.date, ',');
        std::getline(is, job.next_job);
        if (!job.date.empty() && job.date.back() == '\n') {
            job.date.erase(job.date.size() - 1);
        }
        */

        /*
        if (!is.good()) {
            is.setstate(std::ios::failbit);
        }
        */
        return is;
    }
    friend std::ostream& operator<<(std::ostream& os, my_job& job)
    {
        os << job.name << ", " << job.date << ", " << job.next_job;
        return os;
    }
};

void run_stream_test()
{
    std::ifstream ifs;
    ifs.open("./interview/headlands/jobs.csv");
    std::vector<my_job> jobs;
    my_job              job;
    while (ifs >> job) {
        if (job.valid()) {
            jobs.emplace_back(std::move(job));
        } else {
            std::cout << "Malformed input" << std::endl;
        }
    }
    /*
    std::istream_iterator<my_job> bg(ifs);
    std::istream_iterator<my_job> ed;
    std::copy(bg, ed, std::back_inserter(jobs));
    */
    std::cout << "size=" << jobs.size() << std::endl;
    for (auto& j : jobs) {
        std::cout << j << std::endl;
    }
}

void run_verify_job_chain()
{
    /*
    start_job,date,next_job
    intern01,2010-01-01,intern02
    intern02,2010-04-01,intern03
    intern03,2011-06-15,intern04
    intern04,2011-05-01,intern05
    intern05,2012-01-01
    */
    struct job {
        date        start_date;
        std::string name;
        std::string next_job;
    };

    std::map<date, job> job_map;

    csv_reader  reader{"./interview/headlands/jobs.csv"};
    std::string header;
    reader.read_line(header);

    std::vector<std::string> fields;
    while (reader.read_line_fields(fields, ',')) {
        if (fields.size() < 2) {
            break;
        }
        date dt{fields[1], '-'};
        job_map.emplace(
            dt, job{
                    .start_date = dt,
                    .name       = fields[0],
                    .next_job   = (fields.size() > 2 ? fields[2] : ""),
                });
    }
    const job* prev_job = nullptr;
    for (const auto& [dt, jb] : job_map) {
        if (prev_job) {
            if (prev_job->next_job != jb.name) {
                std::cout << "Error: job chain is broken, expected job=" << prev_job->next_job
                          << ", but got job=" << jb.name << std::endl;
            } else {
                std::cout << "Job changed: {" << prev_job->name << ", " << prev_job->start_date << "} ---> {" << jb.name
                          << ", " << jb.start_date << "}" << std::endl;
            }
        } else {
            std::cout << " Detected first job {" << jb.name << ", " << jb.start_date << "}" << std::endl;
        }
        prev_job = &jb;
    }
}

#endif

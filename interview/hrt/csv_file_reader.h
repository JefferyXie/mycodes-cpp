#ifndef CSV_FILE_READER_H
#define CSV_FILE_READER_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <cstdio>

using namespace std;

class csv_file_reader {
public:
    csv_file_reader(const string& file)
    {
        _fs.open(file);
        if (!_fs.is_open())
            throw std::invalid_argument("fails to open file " + file);
        _is_record_end = false;
    }
    ~csv_file_reader() {
        if (_fs.is_open()) _fs.close();
    }
    bool is_record_end()
    {
        return _is_record_end;
    }
    bool read(string& out_field)
    {
        _is_record_end = false;
        out_field.clear();

        char buf;
        int num_quotes = 0;
        // we may simplify this parse step if the data type is only numeric!
        while (read(buf))
        {
            if (buf == '"' && ++num_quotes % 2)
            {
                continue;
            }
            if (buf == ',' && !(num_quotes % 2))
            {
                return true;
            }
            if (buf == '\n' && !(num_quotes % 2))
            {
                _is_record_end = true;
                if (*out_field.rbegin() == '\r')
                {
                    out_field.erase(out_field.end()-1);
                }
                return true;
            }
            out_field += buf;
        }
        return false;
    }
private:
    bool read(char& out_buf) {
        if (_fs.is_open()) {
            _fs.read(&out_buf, 1);
            return _fs.good();
        }
        return false;
    }
    ifstream _fs;
    bool _is_record_end;
};

#endif


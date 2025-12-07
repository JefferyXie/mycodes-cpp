#pragma once

#include "../core/header.h"

bool is_leap_year(int year)
{
    return (year % 4 == 0 ? (year % 100 == 0 ? year % 400 == 0 : true) : false);
}

int num_days_in_month(int year, int month)
{
    static int MonthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 1) {
        return MonthDays[0];
    }
    if (is_leap_year(year) && month == 2) {
        return 29;
    }
    return MonthDays[month - 1];
}

int num_days_between(int s_year, int s_month, int s_day, int e_year, int e_month, int e_day)
{
    int numDays = -s_day + e_day;
    int m       = s_month;
    for (int tYear = s_year; tYear <= e_year; ++tYear) {
        int n = (tYear == e_year ? (e_month - 1) : 12);
        while (m <= n) {
            numDays += num_days_in_month(tYear, m);
            ++m;
        }
        m = 1;
    }
    return numDays;
}


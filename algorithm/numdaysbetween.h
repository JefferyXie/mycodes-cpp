#ifndef NUMDAYSBETWEEN_H
#define NUMDAYSBETWEEN_H

#include "../header.h"

bool IsLeapYear(int year) {
    return (year%4 == 0 ? (year%100 == 0 ? year%400 == 0 : true) : false);
}

int NumDaysInMonth(int year, int month) {
    static int MonthDays[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    if (month == 1) {
        return MonthDays[0];
    }
    if (IsLeapYear(year) && month == 2) {
        return 29;
    }
    return MonthDays[month-1];
}

int NumDaysBetween(int s_year, int s_month, int s_day, int e_year, int e_month, int e_day) {
    int numDays = 0;
    for (int tYear = s_year; tYear <= e_year; ++tYear) {
        if (tYear == s_year) {
            numDays += NumDaysInMonth(tYear, s_month) - s_day;
            int m = s_month;
            while (++m <= 12) {
                numDays += NumDaysInMonth(tYear, m);
            }
            continue;
        }
        if (tYear == e_year) {
            int m = 0;
            while (++m < e_month) {
                numDays += NumDaysInMonth(tYear, m);
            }
            numDays += e_day;
            continue;
        }
        numDays += IsLeapYear(tYear) ? 366 : 365;
    }
    return numDays;
}

#endif


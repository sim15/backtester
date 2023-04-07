#ifndef DATE_H
#define DATE_H

#include <iostream>

class Date {
public:
  Date(int year, int month, int day) {
    year_ = year;
    month_ = month;
    day_ = day;
  }
  Date(){};

  int year() const { return year_; }
  int month() const { return month_; }
  int day() const { return day_; }

  bool operator<(const Date &other) const {
    if (year_ < other.year_) {
      return true;
    } else if (year_ > other.year_) {
      return false;
    } else {
      if (month_ < other.month_) {
        return true;
      } else if (month_ > other.month_) {
        return false;
      } else {
        return day_ < other.day_;
      }
    }
  }

  bool operator==(const Date &other) const {
    bool yr = year_ == other.year_;
    bool mth = month_ == other.month_;
    bool day = day_ == other.day_;

    return yr && mth && day;
  }

private:
  int year_;
  int month_;
  int day_;
};

#endif
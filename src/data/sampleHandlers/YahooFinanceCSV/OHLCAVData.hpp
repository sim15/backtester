#ifndef YAHOODATA_H
#define YAHOODATA_H
#include <data/Date.hpp>
#include <iostream>

// Date,Open,High,Low,Close,Adj Close,Volume
struct OHLCAVData {

public:
  OHLCAVData(Date d, int open, int high, int low, int close, int adjClose,
             int vol)
      : date(d), open(open), high(high), low(low), close(close),
        adjClose(adjClose), volume(vol){};
  OHLCAVData(){};

  Date date;
  int open;
  int high;
  int low;
  int close;
  int adjClose;
  int volume;
};

#endif
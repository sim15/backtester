#ifndef YAHOODATA_H
#define YAHOODATA_H
// Date,Open,High,Low,Close,Adj Close,Volume
// #include <data/Data.hpp>
#include <data/Date.hpp>
#include <iostream>

// a single price point data
// custom subclasses can be defined with more data parameters (e.g., OHLCVI
// data) by default, only price is stored
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
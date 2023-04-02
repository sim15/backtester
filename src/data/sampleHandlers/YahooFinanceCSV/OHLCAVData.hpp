#ifndef YAHOODATA_H
#define YAHOODATA_H
// Date,Open,High,Low,Close,Adj Close,Volume
// #include <data/Data.hpp>
#include <data/Date.hpp>
#include <iostream>

// a single price point data
// custom subclasses can be defined with more data parameters (e.g., OHLCVI
// data) by default, only price is stored
class OHLCAVData {

public:
  OHLCAVData(Date d, int open, int high, int low, int close, int adjClose,
             int vol)
      : date(d), open(open), high(high), low(low), close(close),
        adjClose(adjClose), volume(vol){};
  OHLCAVData(){};

  Date getDate() const { return date; };
  int getOpen() const { return open; };
  int getHigh() const { return high; };
  int getLow() const { return low; };
  int getClose() const { return close; };
  int getAdjClose() const { return adjClose; };
  int getVolume() const { return volume; };
  // std::string stringify() override;

private:
  Date date;
  int open;
  int high;
  int low;
  int close;
  int adjClose;
  int volume;
  // TODO: base class has wasted 'price' value. remove or rename to 'close' or
  // something like 'open'?
};

#endif
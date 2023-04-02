// base Data class

#ifndef DATA_H
#define DATA_H

#include <iostream>

// a single price point data
// custom subclasses can be defined with more data parameters (e.g., OHLCVI
// data) by default, only price is stored
class Data {

public:
  virtual ~Data() = default;
  // virtual std::string stringify() { return "---NO DATA---"; };
};

#endif
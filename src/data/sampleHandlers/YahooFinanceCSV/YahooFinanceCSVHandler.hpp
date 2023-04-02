#ifndef YAHOOFINANCECSVHANDLER_H
#define YAHOOFINANCECSVHANDLER_H

// #include <data/Data.hpp>
#include <data/DataHandler.hpp>
#include <data/Date.hpp>
#include <event/Event.hpp>
#include <iostream>

Date parseDateString(std::string date);

class YahooFinanceCSVHandler : public DataHandler<OHLCAVData> {
public:
  YahooFinanceCSVHandler(int numSymbols, std::string *symbolFiles,
                         std::string *symbolLabels,
                         std::queue<Event> *eventQueue);
  // YahooFinanceCSVHandler();
  void update_bars() override;
  void empty_read();
  // OHLCAVData get_latest_bar() override;
  // ~YahooFinanceCSVHandler();

private:
  std::map<std::string, std::ifstream> dataFiles;
  int linesRead;
};

#endif
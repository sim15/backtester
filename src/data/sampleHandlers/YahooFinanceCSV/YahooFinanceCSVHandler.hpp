#ifndef YAHOOFINANCECSVHANDLER_H
#define YAHOOFINANCECSVHANDLER_H

#include <data/DataHandler.hpp>
#include <data/Date.hpp>
#include <data/sampleHandlers/YahooFinanceCSV/OHLCAVData.hpp>
#include <event/Event.hpp>
#include <iostream>
#include <memory>

Date parseDateString(std::string date);

class YahooFinanceCSVHandler : public DataHandler<OHLCAVData, Date> {
public:
  YahooFinanceCSVHandler(int numSymbols, std::string *symbolFiles,
                         std::string *symbolLabels,
                         std::queue<std::shared_ptr<Event>> *eventQueue);
  // YahooFinanceCSVHandler();

  bool update_bars() override;
  void empty_read();
  // ~YahooFinanceCSVHandler();

private:
  std::map<std::string, std::ifstream> dataFiles;
};

#endif
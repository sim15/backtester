#include <ctime>
#include <data/CSVRow.hpp>
#include <data/Date.hpp>
#include <data/sampleHandlers/YahooFinanceCSV/OHLCAVData.hpp>
#include <data/sampleHandlers/YahooFinanceCSV/YahooFinanceCSVHandler.hpp>
#include <event/Event.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

// YahooFinanceCSVHandler::~YahooFinanceCSVHandler() {
//   delete dataFiles;
//   delete symbols;
// }

Date parseDateString(std::string date) {
  // TODO: add format checking
  std::string date_time_format = "%Y/%m/%d";
  std::istringstream ss{date};
  tm dt;

  ss >> std::get_time(&dt, date_time_format.c_str());

  time_t final_time;

  final_time = mktime(&dt);
  tm *ltm = localtime(&final_time);

  return Date(1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);
}

YahooFinanceCSVHandler::YahooFinanceCSVHandler(int numSymbols,
                                               std::string *symbolFiles,
                                               std::string *symbolLabels,
                                               std::queue<Event> *eventQueue)
    : DataHandler<OHLCAVData>(eventQueue) {

  for (int i = 0; i < numSymbols; i++) {
    dataFiles.emplace(symbolLabels[i], symbolFiles[i]);
    historical[symbolLabels[i]].clear();
  }

  linesRead = 0;
}

void YahooFinanceCSVHandler::empty_read() {

  for (auto const &[symbol, path] : dataFiles) {

    if (!dataFiles[symbol])
      throw std::invalid_argument("symbol file path invalid; file not found");

    CSVRow row;

    if (dataFiles[symbol] >> row) {
      linesRead++;
    } else
      throw std::out_of_range("no new data read for at least one data stream");
  }
}

void YahooFinanceCSVHandler::update_bars() {

  for (auto const &[symbol, path] : dataFiles) {
    if (!dataFiles[symbol])
      throw std::invalid_argument("symbol file path invalid; file not found");

    CSVRow row;

    if (dataFiles[symbol] >> row) {
      // TODO: potentially unnecessary allocation
      // converting from string view to int

      std::string date(row[0]);
      std::string open(row[1]);
      std::string high(row[2]);
      std::string low(row[3]);
      std::string close(row[4]);
      std::string adjClose(row[5]);
      std::string volume(row[6]);

      OHLCAVData ticker(parseDateString(date), stoi(open), stoi(high),
                        stoi(low), stoi(close), stoi(adjClose), stoi(volume));

      historical[symbol].push_back(ticker);
      linesRead++;
    } else
      throw std::out_of_range("no new data read for at least one data stream");
  }
}
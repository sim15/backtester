#include <ctime>
#include <data/CSVRow.hpp>
#include <data/Date.hpp>
#include <data/sampleHandlers/YahooFinanceCSV/YahooFinanceCSVHandler.hpp>
#include <event/Event.hpp>
#include <event/MarketEvent.hpp>
#include <iomanip>
#include <iostream>
#include <memory>
#include <queue>
#include <sstream>
#include <string>

/**
 * @brief parse a date (e.g., string "1980-12-12") and convert to a Date object
 *
 * @param date
 * @return Date
 */
Date parseDateString(std::string date) {
  // std::string date_time_format = "%Y-%m-%d";
  // std::istringstream ss{date};
  // tm dt;

  // ss >> std::get_time(&dt, date_time_format.c_str());

  // time_t final_time;

  // final_time = mktime(&dt);
  // tm *ltm = localtime(&final_time);

  // int a = 1900 + ltm->tm_year;
  // int b = 1 + ltm->tm_mon;
  // int c = ltm->tm_mday;
  //! TODO: add format checking

  int year;
  int month;
  int day;
  int result = sscanf(date.c_str(), "%d-%d-%d", &year, &month, &day);

  return Date(year, month, day);
}

/**
 * @brief Construct a new Yahoo Finance C S V Handler:: Yahoo Finance CSV
 * Handler object
 *
 * @param numSymbols number of symbols/files to be read
 * @param symbolFiles global path to symbol data file (csv) (e.g.,
 * 'C:/.../AAPL.csv')
 * @param symbolLabels label for each symbol (e.g., AAPL)
 * @param eventQueue pointer to main event queue
 */
YahooFinanceCSVHandler::YahooFinanceCSVHandler(
    int numSymbols, std::string *symbolFiles, std::string *symbolLabels,
    std::queue<std::shared_ptr<Event>> *eventQueue)
    : DataHandler<OHLCAVData>(eventQueue) {

  for (int i = 0; i < numSymbols; i++) {
    dataFiles.emplace(symbolLabels[i], symbolFiles[i]);
    historical[symbolLabels[i]].clear();
  }

  numDataPoints = 0;
}

/**
 * @brief read a line from each symbol file (do nothing; move to next line)
 *
 */
void YahooFinanceCSVHandler::empty_read() {

  for (auto const &[symbol, path] : dataFiles) {

    if (!dataFiles[symbol])
      throw std::invalid_argument("symbol file path invalid; file not found");

    CSVRow row;

    if (dataFiles[symbol] >> row) {
      numDataPoints++;
    } else
      throw std::out_of_range("no new data read for at least one data stream");
  }
}

/**
 * @brief read one line from all symbol data files. store read ticker data and
 * add a market event if new data was read
 *
 */
void YahooFinanceCSVHandler::update_bars() {
  // TODO: check that dates in data are the same

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
      numDataPoints++;

      (*events).push(std::make_shared<MarketEvent>());
    } else
      throw std::out_of_range("no new data read for at least one data stream");
  }
}
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
  //! TODO: add format checking
  int year;
  int month;
  int day;
  int result = sscanf(date.c_str(), "%d-%d-%d", &year, &month, &day);

  return Date(year, month, day);
}

/**
 * @brief Construct a new Yahoo FinanceCSV Handler:: Yahoo Finance CSV
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
    : DataHandler<OHLCAVData, Date>(eventQueue) {

  for (int i = 0; i < numSymbols; i++)
    dataFiles.emplace(symbolLabels[i], symbolFiles[i]);
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

    if (!(dataFiles[symbol] >> row))
      throw std::out_of_range("no new data read for at least one data stream");
  }
}

/**
 * @brief read one line from all symbol data files. store read ticker data and
 * add a market event to queue if new data was read;
 * simulates NEW price data (no retroactive updates; each ticker must be for
 * same day) allows missing data points but not misaligned dates on ticker
 */
// TODO: does not allow for missing data yet
bool YahooFinanceCSVHandler::update_bars() {
  // use first date in sheet as reference
  bool flag_dateNotRecorded = true;
  Date currentDate;
  std::vector<std::string> toWriteSymbol;
  std::vector<OHLCAVData> toWriteTicker;
  int scalePrice = 100;

  for (auto const &[symbol, path] : dataFiles) {
    if (!dataFiles[symbol])
      throw std::invalid_argument("symbol file path invalid; file not found");

    CSVRow row;
    // try to read the csv row
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

      OHLCAVData ticker(parseDateString(date), stoi(open) * scalePrice,
                        stoi(high) * scalePrice, stoi(low) * scalePrice,
                        stoi(close) * scalePrice, stoi(adjClose) * scalePrice,
                        stoi(volume) * scalePrice);

      // keep track of current time index
      if (flag_dateNotRecorded) {
        currentDate = ticker.date;

        const Date *ok = get_latest_time();
        // check date is fresh
        if (!((get_latest_time() == NULL) ||
              (*get_latest_time() < currentDate)))
          throw std::runtime_error("Attempting to read stale data");

        flag_dateNotRecorded = false;

      }
      // make sure all data is from the same point in time
      else if (currentDate != ticker.date) {
        throw std::runtime_error("Data rows not aligned; dates do not match");
      }

      // don't write data until all rows validates
      toWriteSymbol.push_back(symbol);
      toWriteTicker.push_back(ticker);
    }
  }

  // no rows read
  if (flag_dateNotRecorded)
    throw std::runtime_error("no new data read");

  // data validated; write to handler
  for (int i = 0; i < toWriteSymbol.size(); i++)
    writeDataPoint(toWriteSymbol[i], toWriteTicker[i].date, toWriteTicker[i]);

  (*events).push(std::make_shared<MarketEvent>());

  return true;
}
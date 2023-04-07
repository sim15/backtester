#include <iostream>

#include <A/A.h>
#include <data/CSVRow.hpp>
#include <data/DataHandler.hpp>
#include <data/Date.hpp>
#include <data/sampleHandlers/YahooFinanceCSV/OHLCAVData.hpp>
#include <data/sampleHandlers/YahooFinanceCSV/YahooFinanceCSVHandler.hpp>
#include <event/Event.hpp>
#include <memory>
#include <queue>
#include <string>

int main() {
  std::queue<std::shared_ptr<Event>> eventQueue;

  std::string *files = new std::string[2]{
      "C:/Users/simor/Programming "
      "Files/backtester/build/example_datafeed_daily/AAPL.csv",
      "C:/Users/simor/Programming "
      "Files/backtester/build/example_datafeed_daily/BABA.csv"};

  std::string *symbols = new std::string[2]{"AAPL", "BABA"};

  // set up data
  YahooFinanceCSVHandler yahooRead(2, files, symbols, &eventQueue);
  // ignore header row
  yahooRead.empty_read();

  // a single heart beat of the system

  // fetch bars (data) -> iterate over event queue until system fully up-to-date
  yahooRead.update_bars();

  while (!eventQueue.empty()) {
    std::shared_ptr<Event> curEvent = eventQueue.front();
    eventQueue.pop();

    if (curEvent->type == "Market") {
      // strategy.calculate_signals(event)
      // portfolio.update_timeindex(event)
    } else if (curEvent->type == "Signal") {
      // portfolio.update_signal(event)
    } else if (curEvent->type == "Order") {
      // broker.execute_order(event)
    } else if (curEvent->type == "Fill") {
      // portfolio.update_fill(event)
    }
  }

  return 0;
}
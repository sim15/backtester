#include <iostream>

#include <A/A.h>
#include <data/CSVRow.hpp>
#include <data/DataHandler.hpp>
#include <data/Date.hpp>
#include <data/sampleHandlers/YahooFinanceCSV/OHLCAVData.hpp>
#include <data/sampleHandlers/YahooFinanceCSV/YahooFinanceCSVHandler.hpp>
#include <demo_setup/BasicOHLCAVPortfolio.hpp>
#include <demo_setup/NaiveStrategy.hpp>
#include <event/Event.hpp>
#include <event/MarketEvent.hpp>
#include <memory>
#include <queue>
#include <string>

#include "spdlog/spdlog.h"
#include <spdlog/fmt/bundled/format.h>

int main() {
  int numSymbols = 2, initCapital = 1000;

  std::queue<std::shared_ptr<Event>> eventQueue;

  std::string *files = new std::string[numSymbols]{
      "C:/Users/simor/Programming "
      "Files/backtester/src/example_datafeed_daily/AAPL.csv",
      "C:/Users/simor/Programming "
      "Files/backtester/src/example_datafeed_daily/BABA.csv"};

  std::string *symbols = new std::string[numSymbols]{"AAPL", "BABA"};

  // set up data
  YahooFinanceCSVHandler yahooRead(numSymbols, files, symbols, &eventQueue);
  // ignore header row
  yahooRead.empty_read();
  spdlog::info("Initialized YahooFinance Handler on {} symbols ", numSymbols);

  // set up portfolio
  BasicOHLCAVPortfolio portfolio(numSymbols, &yahooRead, &eventQueue, symbols,
                                 initCapital);
  spdlog::info(
      "Initialized BasicOHLCAVPortfolio on {} with ${:03.2f} initial capital",
      numSymbols, (double)initCapital / 100);

  // while (true) {
  for (int i = 0; i < 2; i++) {
    // a single heart beat of the system
    // TODO: add exit backtest condition in datahandler

    // !log will fail if no new data fetched
    yahooRead.update_bars();
    spdlog::info("Fetched new data for date {}", *yahooRead.getLatestTime());

    // iterate over event queue until system fully
    // up-to-date
    while (!eventQueue.empty()) {
      std::shared_ptr<Event> curEvent = eventQueue.front();
      eventQueue.pop();

      if (curEvent->type == "Market") {
        MarketEvent *newMarketData =
            static_cast<MarketEvent *>(curEvent.get()); //?is this ok
        // strategy.calculate_signals(*newMarketData)
        portfolio.updateTimeIndex();
      } else if (curEvent->type == "Signal") {
        SignalEvent *newSignal =
            static_cast<SignalEvent *>(curEvent.get()); //?is this ok
        portfolio.updateSignal(*newSignal);
      } else if (curEvent->type == "Order") {
        // broker.execute_order(event)
      } else if (curEvent->type == "Fill") {
        // portfolio.update_fill(event)
      }
    }
  }

  return 0;
}
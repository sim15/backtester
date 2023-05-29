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

int main() {
  std::queue<std::shared_ptr<Event>> eventQueue;

  std::string *files = new std::string[2]{
      "C:/Users/simor/Programming "
      "Files/backtester/src/example_datafeed_daily/AAPL.csv",
      "C:/Users/simor/Programming "
      "Files/backtester/src/example_datafeed_daily/BABA.csv"};

  std::string *symbols = new std::string[2]{"AAPL", "BABA"};

  // set up data
  YahooFinanceCSVHandler yahooRead(2, files, symbols, &eventQueue);
  // ignore header row
  yahooRead.empty_read();
  BasicOHLCAVPortfolio portfolio(2, &yahooRead, &eventQueue, symbols, 0, 1000);

  // a single heart beat of the system

  // while (true) {
  // TODO: add exit backtest condition in datahandler

  // fetch bars (data) -> iterate over event queue until system fully
  // up-to-date
  yahooRead.update_bars();
  // yahooRead.update_bars();

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

  // }

  return 0;
}
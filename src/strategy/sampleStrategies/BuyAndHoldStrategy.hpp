#ifndef BuyAndHoldStrategy_H
#define BuyAndHoldStrategy_H

#include <data/sampleHandlers/YahooFinanceCSV/OHLCAVData.hpp>
#include <portfolio/Portfolio.hpp>
#include <strategy/Strategy.hpp>

template <class Portfolio>
class BuyAndHoldStrategy : public Strategy<OHLCAVData, Date> {
  BuyAndHoldStrategy(std::queue<std::shared_ptr<Event>> *eventQueue,
                     DataHandler<OHLCAVData, Date> *data,
                     Portfolio<OHLCAVData, Date> port)
      : Strategy(eventQueue, data, "Buy and Hold") {
    portfolio = port;
    calculateInitialBought();
  };

  void calculateInitialBought() {
    symbols = portfolio.getSymbols();
    numSymbols = portfolio.getNumSymbols();
    for (int i = 0; i < numSymbols; i++)
      bought[symbols[i]] = false;
  };

  void calculateSignals() {
    for (int i = 0; i < numSymbols; i++) {
      OHLCAVData data = data->get_latest_bar(symbols[i]);
      if
    }
  };

private:
  Portfolio<OHLCAVData> portfolio;
  std::map<std::string, bool> bought;
  int numSymbols;
  std::string symbols;
};

#endif
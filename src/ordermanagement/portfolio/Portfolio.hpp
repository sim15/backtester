// base portfolio

#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <data/DataHandler.hpp>
#include <event/Event.hpp>
#include <event/FillEvent.hpp>
#include <event/OrderEvent.hpp>
#include <event/SignalEvent.hpp>
#include <map>
#include <vector>

template <class Data> class Portfolio {

public:
  Portfolio(int numSymbols, DataHandler<Data> *bars,
            std::queue<std::shared_ptr<Event>> *events, std::string *symbols,
            int startDate, int initialCapital)
      : numSymbols(numSymbols), bars(bars), events(events),
        startDate(startDate), symbolList(symbols),
        initialCapital(initialCapital) {

    currentHoldings = constructCurrentHoldings();
    currentPositions = constructCurrentPositions();
  };

  // std::vector<std::map<std::string, int>>  ();
  // std::vector<std::map<std::string, int>> constructAllHoldings();
  std::map<std::string, int> constructCurrentHoldings();
  std::map<std::string, int> constructCurrentPositions();

  void updateTimeIndex();
  //   Adds a new record to the positions matrix for the current
  // market data bar. This reflects the PREVIOUS bar, i.e. all
  // current market data at this stage is known (OHLCV).
  // Makes use of a MarketEvent from the events queue.

  void updatePositionsFromFill(FillEvent event);
  void updateHoldingsFromFill(FillEvent event);
  void updateFill(FillEvent event);
  void updateSignal(SignalEvent event);

  virtual int compute_market_value(int position, Data priceData) = 0;
  virtual OrderEvent generate_order(SignalEvent event) = 0;

private:
  DataHandler<Data> *bars; // The DataHandler object with current market data.
  std::queue<std::shared_ptr<Event>> *events; // The Event Queue object.
  std::string *symbolList;
  int startDate;      // The start date (bar) of the portfolio
  int initialCapital; // The starting capital in USD.
  int numSymbols;

  std::vector<std::map<std::string, int>> allPositions;
  std::vector<std::map<std::string, int>> allHoldings;

  std::map<std::string, int> currentPositions;
  std::map<std::string, int> currentHoldings;
};

// ==========================================================

// initial instantaneous holdings
// TODO: convert to a portfolio state struct
template <class Data>
std::map<std::string, int> Portfolio<Data>::constructCurrentHoldings() {
  std::map<std::string, int> holdings;
  for (int i = 0; i < numSymbols; i++)
    holdings[symbolList[i]] = 0;

  holdings["cash"] = initialCapital;
  holdings["commission"] = 0;
  holdings["total"] = initialCapital;
  return holdings;
}

template <class Data>
std::map<std::string, int> Portfolio<Data>::constructCurrentPositions() {
  std::map<std::string, int> positions;
  for (int i = 0; i < numSymbols; i++)
    positions[symbolList[i]] = 0;

  return positions;
}

// update time indices and add both positions and holdings to history
template <class Data> void Portfolio<Data>::updateTimeIndex() {
  // fetch all data
  std::map<std::string, Data> data;
  for (int i = 0; i < numSymbols; i++) {
    std::string symbol = symbolList[i];
    data[symbol] = bars->get_latest_bar(symbol);
  }
  // TODO: setup time indices in data handler
  int curTimeIndex = bars->getTimeIndex();

  // positions
  std::map<std::string, int> positions(currentPositions);
  positions["timeIndex"] = curTimeIndex;
  allPositions.push_back(currentPositions);

  // holdings
  std::map<std::string, int> holdings(currentHoldings);
  holdings["timeIndex"] = curTimeIndex;
  holdings["total"] = holdings["cash"];

  // update each holding based on market price
  for (int i = 0; i < numSymbols; i++) {
    int marketValue = compute_market_value(currentPositions[symbolList[i]],
                                           data[symbolList[i]]);
    holdings[symbolList[i]] = marketValue;
    holdings["total"] += marketValue;
  }
  allHoldings.push_back(holdings);
}

template <class Data>
void Portfolio<Data>::updatePositionsFromFill(FillEvent fill) {
  int fillDir = 0;
  if (fill.direction == "Buy")
    fillDir = 1;
  if (fill.direction == "Sell")
    fillDir = -1;

  currentPositions[fill.symbol] += fillDir * fill.quantity;
}

// The corresponding update_holdings_from_fill is similar to the above method
// but updates the holdings values instead. In order to simulate the cost of a
// fill, the following method does not use the cost associated from the
// FillEvent. Why is this? Simply put, in a backtesting environment the fill
// cost is actually unknown (the market impact and the depth of book are
// unknown) and thus is must be estimated.
// Thus the fill cost is set to the the "current market price", which is the
// closing price of the last bar. The holdings for a particular symbol are then
// set to be equal to the fill cost multiplied by the transacted quantity. For
// most lower frequency trading strategies in liquid markets this is a
// reasonable approximation, but at high frequency these issues will need to be
// considered in a production backtest and live trading engine.

template <class Data>
void Portfolio<Data>::updateHoldingsFromFill(FillEvent fill) {
  int fillDir = 0;
  if (fill.direction == "Buy")
    fillDir = 1;
  if (fill.direction == "Sell")
    fillDir = -1;

  Data curSymbolData = bars->get_latest_bar(fill.symbol);

  int marketValue = compute_market_value(fill.quantity, curSymbolData);
  int cost = fillDir * marketValue;
  currentHoldings[fill.symbol] += cost;
  currentHoldings["commission"] += fill.commission;
  currentHoldings["cash"] -= (cost + fill.commission);
  currentHoldings["total"] -= (cost + fill.commission);
}

template <class Data> void Portfolio<Data>::updateFill(FillEvent fill) {
  updatePositionsFromFill(fill);
  updateHoldingsFromFill(fill);
}

template <class Data> void Portfolio<Data>::updateSignal(SignalEvent event) {
  OrderEvent order = generate_order(event);
  (*events).push(std::make_shared<Event>(order));
}

#endif
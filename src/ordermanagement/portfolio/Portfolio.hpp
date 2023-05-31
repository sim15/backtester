// base portfolio

#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <data/DataHandler.hpp>
#include <event/Event.hpp>
#include <event/FillEvent.hpp>
#include <event/OrderEvent.hpp>
#include <event/SignalEvent.hpp>
#include <map>
#include <ordermanagement/portfolio/PortfolioState.hpp>
#include <vector>

#include "spdlog/spdlog.h"
#include <spdlog/fmt/bundled/format.h>

template <class Data, class IndexTypeComparable> class Portfolio {

public:
  Portfolio(int numSymbols, DataHandler<Data, IndexTypeComparable> *bars,
            std::queue<std::shared_ptr<Event>> *events, std::string *symbols,
            int initialCapital)
      : numSymbols(numSymbols), bars(bars), events(events), symbolList(symbols),
        initialCapital(initialCapital) {

    startIndex = bars->get_time_index();
    curState =
        PortfolioState(constructEmptyHoldings(), constructEmptyPositions(),
                       initialCapital, 0, initialCapital, startIndex);
  };

  std::map<std::string, int> constructEmptyHoldings();
  std::map<std::string, int> constructEmptyPositions();

  void updateTimeIndex();

  void updatePositionsFromFill(FillEvent event);
  void updateHoldingsFromFill(FillEvent event);
  void updateFill(FillEvent event);
  void updateSignal(SignalEvent event);

  virtual int compute_market_value(std::string symbol, int quantity) = 0;
  virtual std::vector<OrderEvent> generate_orders(SignalEvent event) = 0;

  std::string *getSymbols() const { return symbolList; };
  int getNumSymbols() const { return numSymbols; };

protected:
  DataHandler<Data, IndexTypeComparable>
      *bars; // The DataHandler object with current market data.
  std::queue<std::shared_ptr<Event>> *events; // The Event Queue object.

private:
  std::string *symbolList;
  int startIndex;     // The start date (bar) of the portfolio
  int initialCapital; // The starting capital in USD.
  int numSymbols;

  PortfolioState curState; // current portfolio state
  std::vector<PortfolioState> historicalStates;
};

// ==========================================================

/**
 * @brief return default (all 0) empty holdings
 *
 * @tparam Data
 * @return std::map<std::string, int>
 */
template <class Data, class IndexTypeComparable>
std::map<std::string, int>
Portfolio<Data, IndexTypeComparable>::constructEmptyHoldings() {
  std::map<std::string, int> holdings;
  for (int i = 0; i < numSymbols; i++)
    holdings[symbolList[i]] = 0;

  return holdings;
}

/**
 * @brief return default (all 0) empty positions
 *
 * @tparam Data
 * @tparam IndexTypeComparable
 * @return std::map<std::string, int>
 */
template <class Data, class IndexTypeComparable>
std::map<std::string, int>
Portfolio<Data, IndexTypeComparable>::constructEmptyPositions() {
  std::map<std::string, int> positions;
  for (int i = 0; i < numSymbols; i++)
    positions[symbolList[i]] = 0;

  return positions;
}

/**
 * @brief stores current portfolio state in matrix and update time index.
 * Reflects PREVIOUS bar, i.e. all current market data at this stage is known.
 * does not actually modify current portfolio state, just recomputes value of
 * holdings.
 *
 * @tparam Data
 * @tparam IndexTypeComparable
 */
template <class Data, class IndexTypeComparable>
void Portfolio<Data, IndexTypeComparable>::updateTimeIndex() {
  // fetch all latest data
  std::map<std::string, const Data *> data;
  for (int i = 0; i < numSymbols; i++) {
    std::string symbol = symbolList[i];
    data[symbol] = bars->get_latest_bar(symbol);
  }
  // get index of data (number of datapoints)
  int curTimeIndex = bars->get_time_index();

  // holdings
  std::map<std::string, int> holdings(curState.holdings); // copy holdings
  int newTotal = curState.cash;                           // TODO: verify

  // recalculate/update each holding based on market price
  for (int i = 0; i < numSymbols; i++) {
    int marketValue =
        compute_market_value(symbolList[i], curState.positions[symbolList[i]]);
    holdings[symbolList[i]] = marketValue;
    newTotal += marketValue;
  }

  // TODO?: change curTimeINdex to be Date and not int
  PortfolioState valueState =
      PortfolioState(holdings, curState.positions, curState.cash,
                     curState.commission, newTotal, curTimeIndex);

  historicalStates.push_back(valueState);

  spdlog::info("Portfolio state saved to history for {}th time index ({}\t| "
               "Cash: ${}, Total: ${}, Commission: ${})",
               curTimeIndex, *bars->get_nth_time(curTimeIndex), valueState.cash,
               valueState.total, valueState.commission);
}

/**
 * @brief update positions from an executed market (fill) order
 *
 * @tparam Data
 * @param fill
 */
template <class Data, class IndexTypeComparable>
void Portfolio<Data, IndexTypeComparable>::updatePositionsFromFill(
    FillEvent fill) {
  int fillDir = 0;
  if (fill.direction == "Buy")
    fillDir = 1;
  if (fill.direction == "Sell")
    fillDir = -1;

  curState.positions[fill.symbol] += fillDir * fill.quantity;
}

// The corresponding update_holdings_from_fill is similar to the above method
// but updates the holdings values instead.

/**
 * @brief updates current holdings from a fill event.
 *
 * In order to simulate the cost of a
 * fill, the following method does not use the cost associated from the
 * FillEvent. Why is this? Simply put, in a backtesting environment the fill
 * cost is actually unknown (the market impact and the depth of book are
 * unknown) and thus is must be estimated.
 * Thus the fill cost is set to the the "current market price", which is the
 * closing price of the last bar. The holdings for a particular symbol are then
 * set to be equal to the fill cost multiplied by the transacted quantity. For
 * most lower frequency trading strategies in liquid markets this is a
 * reasonable approximation, but at high frequency these issues will need to be
 * considered in a production backtest and live trading engine.
 * @tparam Data
 * @param fill
 */
template <class Data, class IndexTypeComparable>
void Portfolio<Data, IndexTypeComparable>::updateHoldingsFromFill(
    FillEvent fill) {
  int fillDir = 0;
  if (fill.direction == "Buy")
    fillDir = 1;
  if (fill.direction == "Sell")
    fillDir = -1;

  // current price data
  Data *curSymbolData = bars->get_latest_bar(fill.symbol);

  // estimate cost value
  int marketValue = compute_market_value(fill.symbol, fill.quantity);
  int cost = fillDir * marketValue;
  // int cost = fill.fillCost; // ? let's estimate cost when adding fillevent

  curState.holdings[fill.symbol] += cost;
  curState.commission += fill.commission;
  curState.cash -= (cost + fill.commission);
  curState.total -= (cost + fill.commission);
}

template <class Data, class IndexTypeComparable>
void Portfolio<Data, IndexTypeComparable>::updateFill(FillEvent fill) {
  updatePositionsFromFill(fill);
  updateHoldingsFromFill(fill);
}

template <class Data, class IndexTypeComparable>
void Portfolio<Data, IndexTypeComparable>::updateSignal(SignalEvent event) {
  std::vector<OrderEvent> orders = generate_orders(event);
  for (auto &order : orders)
    (*events).push(std::make_shared<Event>(order));
  spdlog::debug("Processed signal and added {} generated order events to queue",
                orders.size());
}

#endif
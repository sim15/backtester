// base data handler class

#ifndef DATAHANDLER_H
#define DATAHANDLER_H

// #include <data/Data.hpp>
#include <event/Event.hpp>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

template <class Data> class DataHandler {

public:
  DataHandler(std::queue<Event> *eventQueue);
  // DataHandler<Data>();
  //   ~DataHandler();

  // fetch N most recent data point for a symbol
  Data get_nth_latest_bar(std::string symbol, int n);
  // fetch most recent data point for a symbol
  Data get_latest_bar(std::string symbol);
  //   Data get_latest_bar_time(string symbol); // ? not necessary for now?

  // updates internal market data points and adds market event to event queue
  // (called during a single heartbeat)
  virtual void update_bars();

protected:
  std::queue<Event> *events;
  std::map<std::string, std::vector<Data>> historical;
};

template <class Data>
DataHandler<Data>::DataHandler(std::queue<Event> *eventQueue) {
  events = eventQueue;
}

/**
 * @brief returns nth day data point for a ticker symbol
 *
 * @param symbol
 * @param n
 * @return Data
 */
template <class Data>
Data DataHandler<Data>::get_nth_latest_bar(std::string symbol, int n) {
  // if symbol does not exist, throw error
  if (historical.find(symbol) == historical.end()) {
    throw std::invalid_argument("given ticker symbol does not exist");
  }

  // n must be non-negative
  if (n < 0) {
    throw std::invalid_argument("time index n must be non-negative");
  }

  // too far back in time (no data)
  if (n > historical[symbol].size())
    return Data();

  // nth day data
  return historical[symbol].at(n);
}

/**
 * @brief returns current (today) data point for a ticker symbol
 *
 * @param symbol ticker symbol
 * @return Data
 */
template <class Data>
Data DataHandler<Data>::get_latest_bar(std::string symbol) {
  return get_nth_latest_bar(symbol, 0);
}

/**
 * @brief fetches new data points and adds them to full data history; adds a
 * market event to the event queue if at least one non-null data point is
 * processed
 */
template <class Data> void DataHandler<Data>::update_bars() { return; }

#endif
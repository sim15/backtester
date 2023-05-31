// base data handler class

#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include "spdlog/spdlog.h"
#include <algorithm>
#include <data/TimeIndex.hpp>
#include <event/Event.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <vector>

template <class Data, class IndexTypeComparable> class DataHandler {

public:
  DataHandler(std::queue<std::shared_ptr<Event>> *eventQueue)
      : events(eventQueue), numDataPoints(0){};
  // DataHandler<Data>();

  // fetch N most recent data point for a symbol
  const Data *get_nth_latest_bar(std::string symbol, int n);
  // fetch N most recent data point for a symbol
  std::vector<const Data *> get_n_latest_bars(std::string symbol, int n);
  // fetch most recent data point for a symbol
  const Data *get_latest_bar(std::string symbol);
  const Data *get_latest_valid_bar(std::string symbol);

  /**
   * @brief fetches new data points and adds them to full data history; adds a
   * market event to the event queue if at least one non-null data point is
   * processed (called during a single heartbeat; returns true if market event
   * added). *MUST BE IMPLEMENTED BY ALL DATA HANDLERS
   *
   */
  virtual bool update_bars() = 0;

  // write data to symbol
  void writeDataPoint(std::string symbol, IndexTypeComparable timeIndex,
                      Data dataPoint);

  // TODO: do we want a getter for symbol at timeindex (not nth)? seems to-
  // TODO: -contradict flexible access for strategies at any time step

  // get latest time index
  const IndexTypeComparable *getLatestTimeIndex() const {
    if (numDataPoints > 0)
      return &dateTimeSequence[numDataPoints - 1];
    return nullptr;
  }
  int getTimeIndex() const { return numDataPoints; };

protected:
  std::queue<std::shared_ptr<Event>> *events;

private:
  std::map<std::string, std::map<IndexTypeComparable, Data>>
      historical; // symbol -> (time -> data)
  std::vector<IndexTypeComparable>
      dateTimeSequence; // ordered list of date indices (never nullptr)
  int numDataPoints;    // number of rows read
};

/**
 * @brief returns an immutable nth day data pointer (or nullptr) for a ticker
 * symbol
 *
 * @param symbol
 * @param n
 * @return const Data*
 */
template <class Data, class IndexTypeComparable>
const Data *
DataHandler<Data, IndexTypeComparable>::get_nth_latest_bar(std::string symbol,
                                                           int n) {
  // if symbol does not exist, throw error
  if (historical.count(symbol) != 1) {
    spdlog::error(
        "Datahandler couldn't access symbol {}; symbol doesn't exist.", symbol);
    throw std::invalid_argument("given ticker symbol does not exist");
  }

  // n must be non-negative
  if (n < 0) {
    spdlog::error(
        "Unsuccessfully attempted to fetch symbol {} at negative index {}.",
        symbol, n);
    throw std::invalid_argument("time index n must be non-negative");
  }
  // too far back in time (no data)
  if (n >= numDataPoints) {
    spdlog::debug(
        "Fetched null ptr for ticker {} at index {} ({} total data points)",
        symbol, n, numDataPoints);
    return nullptr;
  }

  IndexTypeComparable nthIndexTypeComparable =
      dateTimeSequence.at(numDataPoints - n - 1);

  auto entry = historical[symbol].find(nthIndexTypeComparable);

  // no data for this symbol at nth index
  if (entry == historical[symbol].end()) {
    spdlog::debug(
        "Fetched null ptr for ticker {} at index {} (no data entry found)",
        symbol, n);
    return nullptr;
  }

  return &(entry->second);
}

// get the latest n bars in a vector (may be nullptr)
/**
 * @brief returns a vector of the n latest bars (some may be null ptr) for
 * symbol
 *
 * @param symbol
 * @param n
 * @return vector<const Data *>
 */
template <class Data, class IndexTypeComparable>
std::vector<const Data *>
DataHandler<Data, IndexTypeComparable>::get_n_latest_bars(std::string symbol,
                                                          int n) {
  std::vector<Data *> res;
  for (int i = 0; i < n; i++)
    res.push_back(get_nth_latest_bar(i));
  return res;
}

/**
 * @brief returns latest non-null bar (does not return time index); if null ptr,
 * no data found for ticker
 *
 * @tparam Data
 * @tparam IndexTypeComparable
 * @param symbol
 * @return const Data*
 */
template <class Data, class IndexTypeComparable>
const Data *DataHandler<Data, IndexTypeComparable>::get_latest_valid_bar(
    std::string symbol) {
  for (int i = 0; i < numDataPoints; i++) {
    const Data *bar = get_nth_latest_bar(symbol, i);
    if (bar != nullptr)
      return bar;
  }
  spdlog::debug("Failed to find latest valid bar for ticker {} (checked "
                "{} data points)",
                symbol, numDataPoints);
  return nullptr;
}

/**
 * @brief returns current (today) data point for a ticker symbol
 *
 * @tparam Data
 * @tparam IndexTypeComparable
 * @param symbol
 * @return const Data*
 */
template <class Data, class IndexTypeComparable>
const Data *
DataHandler<Data, IndexTypeComparable>::get_latest_bar(std::string symbol) {
  return get_nth_latest_bar(symbol, 0);
}

template <class Data, class IndexTypeComparable>
void DataHandler<Data, IndexTypeComparable>::writeDataPoint(
    std::string symbol, IndexTypeComparable timeIndex, Data dataPoint) {
  // initialize if empty
  historical[symbol];

  historical[symbol][timeIndex] = dataPoint;

  // check if timeindex does not yet exist
  if (std::find(dateTimeSequence.begin(), dateTimeSequence.end(), timeIndex) ==
      dateTimeSequence.end()) {

    // make sure new index is fresh (not in past)
    if ((numDataPoints != 0) &&
        (dateTimeSequence.at(numDataPoints - 1) > timeIndex))
      throw std::invalid_argument("attempting to add data retroactively. "
                                  "Ensure data streams are aligned");

    // add new time index
    dateTimeSequence.push_back(timeIndex);
    numDataPoints++;
  }
}

#endif
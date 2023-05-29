// base data handler class

#ifndef DATAHANDLER_H
#define DATAHANDLER_H

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
  std::vector<Data *> get_n_latest_bars(std::string symbol, int n);
  // fetch most recent data point for a symbol
  const Data *get_latest_bar(std::string symbol);
  const Data *get_latest_valid_bar(std::string symbol);

  // updates internal market data points and adds market event to event
  // queue (called during a single heartbeat)
  virtual void update_bars();

  // write data to symbol
  void writeDataPoint(std::string symbol, IndexTypeComparable timeIndex,
                      Data dataPoint);

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
  std::map<std::string, std::map<IndexTypeComparable, Data>> historical;
  std::vector<IndexTypeComparable>
      dateTimeSequence; // ordered list of date indices (never nullptr)
  int numDataPoints;    // number of rows read
};

/**
 * @brief returns nth day data pointer (or nullptr) for a ticker symbol
 * (duplicates the pointer)
 *
 * @param symbol
 * @param n
 * @return Data
 */
template <class Data, class IndexTypeComparable>
const Data *
DataHandler<Data, IndexTypeComparable>::get_nth_latest_bar(std::string symbol,
                                                           int n) {
  // if symbol does not exist, throw error
  if (historical.count(symbol) != 1)
    throw std::invalid_argument("given ticker symbol does not exist");

  // n must be non-negative
  if (n < 0)
    throw std::invalid_argument("time index n must be non-negative");

  // too far back in time (no data)
  if (n >= numDataPoints)
    return nullptr;

  IndexTypeComparable nthIndexTypeComparable =
      dateTimeSequence.at(numDataPoints - n - 1);
  // // // if no data was read by any symbol at nth index
  // // if (nthIndexTypeComparable == nullptr)
  // //  return nullptr;

  auto entry = historical[symbol].find(nthIndexTypeComparable);

  // no data for this symbol at nth index
  if (entry == historical[symbol].end())
    return nullptr;

  return &(entry->second);

  // if (historical[symbol].count(nthIndexTypeComparable) != 1)
  //   return nullptr;

  // Data *dataPoint;
  // //! make sure to copy data so unmutable
  // dataPoint = historical[symbol][nthIndexTypeComparable];
  // // nth day data (not nullptr)
  // return dataPoint;
}

// get the latest n bars in a vector (may be nullptr)
template <class Data, class IndexTypeComparable>
std::vector<Data *>
DataHandler<Data, IndexTypeComparable>::get_n_latest_bars(std::string symbol,
                                                          int n) {
  std::vector<Data *> res;
  for (int i = 0; i < n; i++)
    res.push_back(get_nth_latest_bar(i));
  return res;
}

// return latest non-null bar
// if nullptr, means no data for that symbol (usually an error)
template <class Data, class IndexTypeComparable>
const Data *DataHandler<Data, IndexTypeComparable>::get_latest_valid_bar(
    std::string symbol) {
  for (int i = 0; i < numDataPoints; i++) {
    const Data *bar = get_nth_latest_bar(symbol, i);
    if (bar != nullptr)
      return bar;
  }
  return nullptr;
}

/**
 * @brief returns current (today) data point for a ticker symbol
 *
 * @param symbol ticker symbol
 * @return Data
 */
template <class Data, class IndexTypeComparable>
const Data *
DataHandler<Data, IndexTypeComparable>::get_latest_bar(std::string symbol) {
  return get_nth_latest_bar(symbol, 0);
}

/**
 * @brief fetches new data points and adds them to full data history; adds a
 * market event to the event queue if at least one non-null data point is
 * processed
 */
template <class Data, class IndexTypeComparable>
void DataHandler<Data, IndexTypeComparable>::update_bars() {
  return;
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
// base strategy class

#ifndef STRATEGY_H
#define STRATEGY_H

#include <data/DataHandler.hpp>
#include <event/Event.hpp>
#include <iostream>
#include <queue>

template <class Data, class IndexTypeComparable> class Strategy {

public:
  Strategy(std::queue<std::shared_ptr<Event>> *eventQueue,
           DataHandler<Data, IndexTypeComparable> *data, std::string name)
      : events(eventQueue), bars(data), name(name){};

  virtual void calculate_signals() = 0;

private:
  std::queue<std::shared_ptr<Event>> *events;
  DataHandler<Data, IndexTypeComparable> *bars;
  std::string name;
};

#endif

// base strategy class

#ifndef STRATEGY_H
#define STRATEGY_H

#include <data/DataHandler.hpp>
#include <event/Event.hpp>
#include <iostream>
#include <queue>

template <class Data> class Strategy {

public:
  Strategy(std::queue<std::shared_ptr<Event>> *eventQueue,
           DataHandler<Data> *data)
      : events(eventQueue), bars(data){};

  virtual void calculate_signals();

private:
  std::queue<std::shared_ptr<Event>> *events;
  DataHandler<Data> *bars;
};

#endif

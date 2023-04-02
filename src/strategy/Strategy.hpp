// base strategy class

#ifndef STRATEGY_H
#define STRATEGY_H

#include <data/DataHandler.hpp>
#include <event/Event.hpp>
#include <iostream>
#include <queue>

class Strategy {

public:
  Strategy(queue<Event> *eventQueue, DataHandler data);

  void calculate_signals();

private:
  queue<Event> *events;
  DataHandler data;
};

#endif

#ifndef MARKETEVENT_H
#define MARKETEVENT_H

#include <event/Event.hpp>

struct MarketEvent : Event {

  MarketEvent() : Event("Market"){};
  // void printEvent() override{};
};

#endif
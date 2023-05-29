// base Event class

#ifndef EVENT_H
#define EVENT_H

#include <iostream>
#include <queue>

struct Event {
  Event(std::string eventType) : type(eventType){};
  // virtual void printEvent() = 0;
  std::string type;
};

#endif
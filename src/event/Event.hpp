// base Event class

#ifndef EVENT_H
#define EVENT_H

#include <iostream>
#include <queue>

class Event {

public:
  Event(std::string eventType);

  std::string getType() const;

private:
  std::string type;
};

#endif
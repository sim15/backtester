

#include <event/Event.hpp>
#include <iostream>

Event::Event(std::string eventType) { type = eventType; }

std::string Event::getType() const { return type; }

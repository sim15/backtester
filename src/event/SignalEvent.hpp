#ifndef SIGNALEVENT_H
#define SIGNALEVENT_H

#include <event/Event.hpp>

struct SignalEvent : Event {

  SignalEvent(int id, std::string symb, int timestamp, std::string type,
              double strengthFactor)
      : Event("Signal"), strategyId(id), symbol(symb), datetime(timestamp),
        signalType(type), strength(strengthFactor){};
  void printEvent() override{};

  int strategyId; // The unique identifier for the strategy that generated the
                  // signal.
  std::string symbol;     // The ticker symbol, e.g. "GOOG".
  int datetime;           // The timestamp at which the signal was generated.
  std::string signalType; // "LONG" or "SHORT".
  double strength; // An adjustment factor "suggestion" used to scale quantity
                   // at the portfolio level. Useful for pairs strategies.
};

#endif
#ifndef FILLEVENT_H
#define FILLEVENT_H

#include <event/Event.hpp>

struct FillEvent : Event {

  FillEvent(int timeindex, std::string symbol, std::string exchange,
            float quantity, std::string direction, int fillCost, int commission)
      : Event("Fill"), timeindex(timeindex), symbol(symbol), exchange(exchange),
        quantity(quantity), direction(direction), fillCost(fillCost),
        commission(commission){};

  // void printEvent() override{};

  int timeindex;         // The bar-resolution when the order was filled
  std::string symbol;    // The instrument which was filled
  std::string exchange;  // The exchange where the order was filled
  float quantity;        // The filled quantity
  std::string direction; // The direction of fill (’BUY’ or ’SELL’)
  int fillCost;          // The holdings value in dollars.
                         //! (not known during backtesting)
  int commission;        //! An optional commission sent from IB.
};

#endif
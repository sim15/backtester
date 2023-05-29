#ifndef ORDEREVENT_H
#define ORDEREVENT_H

#include <event/Event.hpp>

struct OrderEvent : Event {

  OrderEvent(std::string symbol, std::string orderType, int quantity,
             std::string direction)
      : Event("Order"), symbol(symbol), orderType(orderType),
        quantity(quantity), direction(direction){};
  // void printEvent() override{};

  std::string symbol;    // The instrument to trade.
  std::string orderType; // "MKT" or "LMT" for Market or Limit.
  int quantity;          // Non-negative integer for quantity.
  std::string direction; // "BUY" or "SELL" for long or short.
};

#endif
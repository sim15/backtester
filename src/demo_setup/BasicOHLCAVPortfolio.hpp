#ifndef BASIC_PORTFOLIO
#define BASIC_PORTFOLIO

#include <data/sampleHandlers/YahooFinanceCSV/OHLCAVData.hpp>
#include <event/OrderEvent.hpp>
#include <ordermanagement/portfolio/Portfolio.hpp>

class BasicOHLCAVPortfolio : public Portfolio<OHLCAVData> {
public:
  using Portfolio::Portfolio;

  int compute_market_value(int position, OHLCAVData priceData) override {
    return priceData.adjClose * position;
  };

  OrderEvent generate_order(SignalEvent event) override {
    std::string symbol = event.symbol;
    std::string orderType = "MKT";
    int quantity = 1; // keep at 1 for now
    std::string direction;
    if (event.signalType == "LONG")
      direction = "BUY";
    if (event.signalType == "SHORT")
      direction = "SELL";

    // TODO: add EXIT signals to sell/buy off all positions

    return OrderEvent(symbol, orderType, quantity, direction);
  };
};

#endif
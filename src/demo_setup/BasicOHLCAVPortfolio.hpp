// DEMO PORTFOLIO FOR YAHOO FINANCE DATA
#ifndef BASIC_PORTFOLIO
#define BASIC_PORTFOLIO

#include <algorithm>
#include <data/sampleHandlers/YahooFinanceCSV/OHLCAVData.hpp>
#include <event/OrderEvent.hpp>
#include <ordermanagement/portfolio/Portfolio.hpp>

class BasicOHLCAVPortfolio : public Portfolio<OHLCAVData, Date> {
public:
  using Portfolio::Portfolio;

  int compute_market_value(std::string symbol, float position) const override {
    const OHLCAVData *latestData = bars->get_latest_valid_bar(symbol);
    if (latestData == nullptr)
      throw std::runtime_error(
          "Attempting to fetch market price of symbol with no data");
    // TODO: change everything to float
    float cost = (float)latestData->adjClose * (float)position;
    if (cost > (float)INT_MAX)
      return INT_MAX;
    return (int)cost;
  };

  std::vector<OrderEvent> generate_orders(SignalEvent event) override {
    std::string symbol = event.symbol;
    std::string orderType = "MKT";           // for testing
    float quantity = std::max(event.qty, 1); // keep min at 1 for now
    // this sample portfolio does not use the strength factor

    std::string direction;
    if (event.signalType == "LONG")
      direction = "BUY";
    if (event.signalType == "SHORT")
      direction = "SELL";

    // TODO: add EXIT signals to sell/buy off all positions
    std::vector<OrderEvent> result;
    float curQuantity = getCurrentPositions(symbol);

    if ((event.signalType == "EXIT") && (curQuantity > 0)) {
      // sell off long position
      result.push_back(OrderEvent(symbol, orderType, abs(curQuantity), "SELL"));
      return result;
    } else if ((event.signalType == "EXIT") && (curQuantity < 0)) {
      // sell off short position
      result.push_back(OrderEvent(symbol, orderType, abs(curQuantity), "BUY"));
      return result;
    }

    result.push_back(OrderEvent(symbol, orderType, quantity, direction));
    return result;
  };
};

#endif
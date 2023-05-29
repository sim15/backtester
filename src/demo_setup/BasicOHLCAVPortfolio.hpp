#ifndef BASIC_PORTFOLIO
#define BASIC_PORTFOLIO

#include <data/sampleHandlers/YahooFinanceCSV/OHLCAVData.hpp>
#include <event/OrderEvent.hpp>
#include <ordermanagement/portfolio/Portfolio.hpp>
class BasicOHLCAVPortfolio : public Portfolio<OHLCAVData, Date> {
public:
  using Portfolio::Portfolio;

  int compute_market_value(std::string symbol, int position) override {
    const OHLCAVData *latestData = bars->get_latest_valid_bar(symbol);
    if (latestData == nullptr)
      throw std::runtime_error(
          "Attempting to fetch market price of symbol with no data");

    return latestData->adjClose * position;
  };

  std::vector<OrderEvent> generate_orders(SignalEvent event) override {
    std::string symbol = event.symbol;
    std::string orderType = "MKT";
    int quantity = 1; // keep at 1 for now
    std::string direction;
    if (event.signalType == "LONG")
      direction = "BUY";
    if (event.signalType == "SHORT")
      direction = "SELL";

    // TODO: add EXIT signals to sell/buy off all positions

    std::vector<OrderEvent> result;
    result.push_back(OrderEvent(symbol, orderType, quantity, direction));
    return result;
  };
};

#endif
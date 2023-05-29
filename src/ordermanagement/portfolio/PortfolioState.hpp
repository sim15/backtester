#ifndef PORTFOLIO_STATE
#define PORTFOLIO_STATE

#include <map>
#include <string>

struct PortfolioState {
  PortfolioState(std::map<std::string, int> hold,
                 std::map<std::string, int> pos, int cash, int commission,
                 int total, int timeIndex)
      : holdings(hold), positions(pos), cash(cash), commission(commission),
        total(total), timeIndex(timeIndex){};

  PortfolioState(){};

  std::map<std::string, int> holdings;
  std::map<std::string, int> positions;
  int cash;
  int commission;
  int total;
  int timeIndex;
};

#endif
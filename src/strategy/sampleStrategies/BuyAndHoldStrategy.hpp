#ifndef BuyAndHoldStrategy_H
#define BuyAndHoldStrategy_H

#include <ordermanagement/portfolio/Portfolio.hpp>
#include <strategy/Strategy.hpp>

template <class Data, class IndexTypeComparable>
class BuyAndHoldStrategy : public Strategy<Data, IndexTypeComparable> {
public:
  BuyAndHoldStrategy(std::queue<std::shared_ptr<Event>> *eventQueue,
                     DataHandler<Data, IndexTypeComparable> *bars,
                     const Portfolio<Data, IndexTypeComparable> *port)
      : Strategy<Data, IndexTypeComparable>(eventQueue, bars, "Buy and Hold"),
        portfolio(port) {
    calculate_initial_bought();
  };

  // initially, no symbols are bought
  void calculate_initial_bought() {
    symbols = portfolio->getSymbols();
    numSymbols = portfolio->getNumSymbols();
    for (int i = 0; i < numSymbols; i++)
      bought[symbols[i]] = false;
  };

  // void calculate_signals() override { return; }
  void calculate_signals() override {
    for (int i = 0; i < numSymbols; i++) {
      const Data *symbData = this->bars->get_latest_bar(symbols[i]);
      if ((symbData != nullptr) && (!bought[symbols[i]])) {
        // buy an (approximately) equal ratio of every symbol with all
        // available
        // cash
        float qty = portfolio->compute_qty_from_cash(
            symbols[i], (portfolio->getCash() / numSymbols));

        // SignalEvent event = SignalEvent(
        //     this->name, symbols[i], 123, "LONG",
        //     qty); // TODO: timestamp is temporarily '123' for testing
        // (*this->events).push(std::make_shared<Event>(event));
        bought[symbols[i]] = true;
      }
    }
  };

private:
  const Portfolio<Data, IndexTypeComparable> *portfolio;
  std::map<std::string, bool> bought;
  int numSymbols;
  const std::string *symbols;
  // DataHandler<Data, IndexTypeComparable> *bars =
  //     this->bars; // hacky/slightly redundant; hides original bars from base
  // class for some improved readibility
  // https://isocpp.org/wiki/faq/templates#nondependent-name-lookup-members
};

#endif
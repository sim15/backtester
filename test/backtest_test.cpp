#include <Backtester.h> // ! TODO: https://github.com/search?q=repo%3AJamagas%2FHMM%20HMM_LIBRARY_NAME&type=code

// #include <A/A.h>
// #include <data/CSVRow.hpp>
// #include <data/DataHandler.hpp>
// #include <data/Date.hpp>
// #include <data/sampleHandlers/YahooFinanceCSV/OHLCAVData.hpp>
// #include <data/sampleHandlers/YahooFinanceCSV/YahooFinanceCSVHandler.hpp>
// #include <demo_setup/BasicOHLCAVPortfolio.hpp>
// #include <demo_setup/NaiveStrategy.hpp>
// #include <event/Event.hpp>
// #include <event/MarketEvent.hpp>
#include <gtest/gtest.h>
// #include <memory>
// #include <queue>
// #include <strategy/Strategy.hpp>
// #include <strategy/sampleStrategies/BuyAndHoldStrategy.hpp>
// #include <string>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

TEST(DataLoadTest, LoadCSV) {
  // int numSymbols = 2, initCapital = 1000;

  // std::queue<std::shared_ptr<Event>> eventQueue;

  // std::string *files = new std::string[numSymbols]{
  //     "C:/Users/simor/Programming "
  //     "Files/backtester/src/example_datafeed_daily/AAPL.csv",
  //     "C:/Users/simor/Programming "
  //     "Files/backtester/src/example_datafeed_daily/BABA.csv"};

  // std::string *symbols = new std::string[numSymbols]{"AAPL", "BABA"};

  // // set up data
  // YahooFinanceCSVHandler yahooRead(numSymbols, files, symbols, &eventQueue);
  // // ignore header row
  // yahooRead.empty_read();
  // spdlog::info("Initialized YahooFinance Handler on {} symbols ",
  // numSymbols);

  // // set up portfolio
  // BasicOHLCAVPortfolio portfolio(numSymbols, &yahooRead, &eventQueue,
  // symbols,
  //                                initCapital);
  // spdlog::info(
  //     "Initialized BasicOHLCAVPortfolio on {} with ${:03.2f} initial
  //     capital", numSymbols, (double)portfolio.getCash() / 100.00);

  // // TODO: see if maybe pass by refernce is better?
  // // https://stackoverflow.com/a/32959891
  // BuyAndHoldStrategy<OHLCAVData, Date> strategy(&eventQueue, &yahooRead,
  //                                               &portfolio);
}

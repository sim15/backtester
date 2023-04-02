#include <iostream>

#include <A/A.h>
#include <data/CSVRow.hpp>
#include <data/DataHandler.hpp>
#include <data/Date.hpp>
#include <data/sampleHandlers/YahooFinanceCSV/OHLCAVData.hpp>
#include <data/sampleHandlers/YahooFinanceCSV/YahooFinanceCSVHandler.hpp>
#include <event/Event.hpp>
#include <queue>
#include <string>

int main() {
  std::queue<Event> eQ;

  std::string a = "yes";
  Event e(a);

  // DataHandler<int> d = DataHandler(&eQ);

  std::cout << "Foo main" << std::endl;

  std::ifstream file("c");

  Date ass(2000, 5, 5);

  std::string *files = new std::string[2]{
      "C:/Users/simor/Programming "
      "Files/backtester/build/example_datafeed_daily/AAPL.csv",
      "C:/Users/simor/Programming "
      "Files/backtester/build/example_datafeed_daily/BABA.csv"};

  std::string *symbols = new std::string[2]{"AAPL", "BABA"};

  YahooFinanceCSVHandler yahooRead(2, files, symbols, &eQ);

  yahooRead.empty_read();

  for (int i = 0; i < 5; i++)
    yahooRead.update_bars();

  OHLCAVData lalala = yahooRead.get_latest_bar("AAPL");

  return 0;
}
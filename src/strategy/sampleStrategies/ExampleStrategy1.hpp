#ifndef EXAMPLESTRATEGY1_H
#define EXAMPLESTRATEGY1_H

#include <data/sampleHandlers/YahooFinanceCSV/OHLCAVData.hpp>
#include <strategy/Strategy.hpp>

class ExampleStrategy1 : public Strategy<OHLCAVData> {};

#endif
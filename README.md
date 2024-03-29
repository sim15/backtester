# Event-Driven Backtesting Library (C++)
Event-driven backtesting system built in C++ for performant strategy prototyping and testing. 
Designed to be used in combination with custom data streams (e.g., Yahoo Finance data) and custom portfolio management.   

Inspired by the structure outlined in Michael Halls-Moore's [*Successful Algorithmic Trading*](https://www.quantstart.com/successful-algorithmic-trading-ebook/).

# Running the backtester

<b>Windows:</b>
```
cd .\build\
cmake ..
cmake --build .\
```

# Testing
```
cd .\build\
.\backtest_test.exe
```

# Roadmap
**This project is currently under development.** 
Prior to the first stable release, there may be unexpected changes. 

A live roadmap (up to first stable release) is outlined. It is subject to change:

- Add integrated logger to portfolio, data handler, event queue, and allow for custom interfacing with future system areas
- Write demo portfolio and demo strategy as base for future testing
- Test data handling and reading with Yahoo Finance CSVs as data stream. Use loggers where needed 
- Begin testing on basic event queue and signal generation. Fill any gaps/bugs

Begin Python interfacing (once signals and event handling is tested):
- Python wrappers for portfolio, events, data handling, strategies, execution
- Integration with plotter for performance metrics
  - Portfolio performance metrics computed in C++, plotted in Python 
  - ensure flexibility for future risk management integration
- At this stage, be able to run basic functionality through Python wrapper of original C++ implementation:
  - Load Yahoo Finance Data (from on-disk CSV for now)
    - Loose structure for future broker API integration
  - Select symbols to be considered
  - Define strateg(ies)
    - if in Python, may need extensive C++ helpers to optimize performance
    - otherwise, leave strategies in C++ 
  - Define portfolio (generate orders, compute market price)
    - if in Python, need C++ helpers to optimize order generation from signals
    - otherwise, leave signals and orders to C++

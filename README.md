# Stock Price & Portfolio Analyzer (C++)

A comprehensive C++ application for financial stock analysis, trend indicators, volatility estimation, and portfolio management.

## 🌟 Key Features
- **Object-Oriented Architecture:** Abstract base classes (`Analysis`) supporting polymorphic reporting engines.
- **Custom CSV Engine:** Custom string tokenizers and parsers handling quoted strings and escaping.
- **Portfolio Simulation:** Average cost basis calculation, unrealized P/L calculation, and transaction history logging.
- **Robust Input Streams:** Type-safe console input readers preventing input stream crashes.

## 🚀 How to Run

### Build & Execute
```bash
g++ ayush.cpp -o stock_analyzer
./stock_analyzer
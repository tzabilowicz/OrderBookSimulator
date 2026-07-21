# Order Book Simulator

---

### Overview

This project simulates an order book for an arbitrary security. An order book is a list of open buy (bid) and sell (ask) orders showing a security's liquidity and facilitates transactions between buyers and sellers. Buy and sell orders are matched by price, quantity, and order time. This order book simulator offers comprehensive features for various use cases such as HFT (high-frequency trading) algorithm development, machine learning experiments, and financial market microstructure research.

For a technical details, see [Specification Document (SPEC.md)](https://github.com/tzabcoder/OrderBookSimulator/blob/main/SPEC.md "SPEC.md").

### Features

* Order simulation
* Support for all order types
* Historical trade and order logging
* Order matching using common exchange rules (price-time priority)
* Fully customizable network configurations (client and server)
* Fully automated exchange simulation tool; N simulated traders

### Usage

##### Order Book

`./orderBook -s <symbol_1> <symbol_2> ... <symbol_N> -p <port_number> -l`

-s specifies the symbol list; creates an order book for each symbol; required; no default

-p specifies the port for the order book manager to listen on; optional; default=8080

-l is a flag for logging to console; optional; default=false

Ex: `./orderBook -s TEMP1 TEMP2 -p 5555 -l`

##### Agent

// Global Includes
#include <iostream>
#include <string>
#include <vector>

// Project Includes
#include <OrderBookManager.hpp>

/**
 * @brief Extracts all exchange symbols from the command line arg list.
 *
 * @param args - command line args
 * @return std::vector<std::string> - vector of exchange symbols
 */
std::vector<std::string> extract_exchange_symbols(char* args[])
{
    std::vector<std::string> symbols;

    // Find the -s
    // Process all symbols until either -* character or end
    // return symbols

    return symbols;
}

/**
 * @brief Extracts the ports associated with the order book
 * book manager.
 *
 * @param args - command line args
 * @return std::vector<uint32_t> - vector of order book ports
 */
std::vector<uint32_t> extract_port(char* args[])
{
    std::vector<uint32_t> ports;

    return ports;
}

bool extract_console_log(char* args[])
{

}

/**
 * @brief Runs the order book simulation engine
 *
 * @param argc - number of command line arguements
 * @param argv - command line arguements
 *
 * Command line arguements
 * -p (port) : port for the order book simulator
 * -s (symbols) : symbols for the order books (will
 * create one for each symbol)
 * -l : for order book manager console logging
 *
 * @return int - engine status code
 */
int main(int argc, char* argv[]) {
    // Process the command line arguements
    std::vector<std::string> symbols = extract_exchange_symbols(argv);
    std::vector<std::uint32_t> ports = extract_port(argv);
    bool log = extract_console_log(argv);

    // Validate the command line args

    // Create the new order book manager
    OrderBookManager obManager = OrderBookManager(
        ports,
        symbols,
        log
    );

    // Run the order book manager
    obManager.startListener();

    return 0;
}
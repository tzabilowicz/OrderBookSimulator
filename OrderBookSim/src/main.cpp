// Global Includes
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

// Project Includes
#include <OrderBookManager.hpp>

/**
 * @brief Display usage text for initializing the order book manager.
 */
void usage() {
    std::cout << "./orderBook -s <SYMBOL_1> ... <SYMBOL_N> -p <PORT> (-l)" << std::endl;
    std::cout << "-p | port for the order book manager (required)" << std::endl;
    std::cout << "-s | exchange symbols for each order book (required)" << std::endl;
    std::cout << "-l | console log flag (optional)" << std::endl;
}

/**
 * @brief Extracts all exchange symbols from the command line arg list.
 *
 * @param argc - number of command line args
 * @param args - command line args
 * @return std::vector<std::string> - vector of exchange symbols
 */
std::vector<std::string> extract_exchange_symbols(int argc, char* args[]) {
    std::vector<std::string> symbols;

    // Find the position of the symbols
    int s_pos = -1;
    for (int i = 0; i < argc; i++) {
        if (args[i] != nullptr && std::strcmp(args[i], "-s") == 0) {
            s_pos = i;
            break;
        }
    }

    // No symbols position found
    if (s_pos == -1) {
        return symbols;
    }

    // Add all specified symbols symbols
    for (int i = (s_pos + 1); i < argc; i++) {
        if (args[i] == nullptr || args[i][0] == '-') {
            break;
        }
        symbols.push_back(std::string(args[i]));
    }

    return symbols;
}

/**
 * @brief Extracts the ports associated with the order book
 * book manager.
 *
 * @param argc - number of command line args
 * @param args - command line args
 * @return int32_t - port for the order book manager
 */
int32_t extract_port(int argc, char* args[]) {
    int32_t port = -1;

    // Find the position of the port
    int p_pos = -1;
    for (int i = 0; i < argc; i++) {
        if (args[i] != nullptr && std::strcmp(args[i], "-p") == 0) {
            p_pos = i;
            break;
        }
    }

    // No port specified; No numerical value specified
    if (p_pos == -1 || p_pos == (argc - 1)) {
        return port;
    }

    port = std::stoi(args[p_pos+1]);
    return port;
}

/**
 * @brief Extract the console logging flag.
 *
 * @param argc - number of command line args
 * @param args - command line args
 * @return true if -l present, otherwise false
 */
bool extract_console_log(int argc, char* args[]) {
    bool logging = false;

    for (int i = 0; i < argc; i++) {
        if (args[i] != nullptr && std::strcmp(args[i], "-l") == 0) {
            logging = true;
            break;
        }
    }

    return logging;
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
    std::vector<std::string> symbols = extract_exchange_symbols(argc, argv);
    int32_t port = extract_port(argc, argv);
    bool log = extract_console_log(argc, argv);

    // Validate port number
    if (port <= -1) {
        std::cout << "Must provide a port." << std::endl;
        usage();
        return 1;
    }

    // Validate the exchange symbols
    if (symbols.size() == 0) {
        std::cout << "No valid exchange symbols provided." << std::endl;
        usage();
        return 1;
    }

    // Create the new order book manager
    OrderBookManager obManager = OrderBookManager(port, symbols, log);

    // Start listening for order requests
    obManager.startListener();

    return 0;
}
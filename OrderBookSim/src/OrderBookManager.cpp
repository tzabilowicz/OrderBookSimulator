// Project Includes
#include <OrderBookManager.hpp>

//#########################################################################
OrderBookManager::OrderBookManager (
    int port,
    std::vector<std::string> symbols,
    bool logging
) : logging(logging),
    obmPort(port),
    BUFFER_SIZE(1024),
    obmSocket(INVALID_SOCKET) {

    // Create the Order Book Map
    for (std::string symbol : symbols) {
        logMessage(
            LogLevel::INFO,
            "OrderBookManager(): Creating order book (symbol=" + symbol + ")",
            logging
        );

        // Add the order book
        orderBookMap.emplace(symbol, OrderBook(symbol));
    }
}
OrderBookManager::~OrderBookManager() {
    cleanupSocket();
}

//#########################################################################
OrderResponse OrderBookManager::handleMessage(std::string& buffer) {
    logMessage(
        LogLevel::INFO,
        buffer,
        true
    );

    return {};
}

//#########################################################################
void OrderBookManager::startListener() {
    logMessage(
        LogLevel::INFO,
        "startListener(): Starting OBM listener socket...",
        logging
    );

    createSocket();

    while (true) {
        // Accept client connection
        SOCKET clientSocket = accept(obmSocket, nullptr, nullptr);

        if (clientSocket == INVALID_SOCKET) {
            logMessage(
                LogLevel::ERR,
                "startListener(): failed to accept client connection...",
                logging
            );
        }
        // Client socket connected
        else {
            logMessage(
                LogLevel::INFO,
                "startListener(): Client socket connected.",
                logging
            );

            // Read string from the socket
            char rcvBuffer[BUFFER_SIZE];
            int bytesRcv = recv(clientSocket, rcvBuffer, sizeof(rcvBuffer), 0);

            if (bytesRcv > 0) {
                std::string buffer(rcvBuffer, bytesRcv);

                // Handle the order request message
                OrderResponse response = handleMessage(buffer);

                // Write the response to the socket
                std::string responseBuffer = serialize(response);
                send(clientSocket, responseBuffer.data(), static_cast<int>(responseBuffer.size()), 0);

                logMessage(
                    LogLevel::INFO,
                    "startListener(): Sent OrderResponse to client.",
                    logging
                );
            }
            else {
                logMessage(
                    LogLevel::INFO,
                    "startListener(): Failed to read buffer from socket...",
                    logging
                );
            }
        }
    }

    cleanupSocket();
}

//#########################################################################
int OrderBookManager::createSocket() {
    logMessage(
        LogLevel::INFO,
        "createSocket(): Creating OBM lsitener socket...",
        logging
    );

    WSADATA wsaData;
    int wsResult;

    // Initialize winsock
    wsResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsResult != 0) {
        throw std::runtime_error("[ERROR] createSocket(): WSAStartup Failed: " + std::to_string(wsResult));
    }

    // Create the OBM socket
    obmSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (obmSocket == INVALID_SOCKET) {
        WSACleanup();
        throw std::runtime_error("[ERROR] createSocket(): Error creating OBM Socket...");
    }

    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = INADDR_ANY;
    service.sin_port = htons(obmPort);

    // Log socket information (IP address and port)
    std::string ipString = inet_ntoa(service.sin_addr);

    logMessage(
        LogLevel::DEBUG,
        "createSocket(): Binding OBM socket to IP=" + ipString +
        ", Port=" + std::to_string(ntohs(service.sin_port)),
        logging
    );

    // Bind the OBM socket
    if (bind(obmSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        cleanupSocket();
        throw std::runtime_error("[ERROR] createSocket(): OBM socket bind failed...");
    }

    // Start listening on the OBM socket
    if (listen(obmSocket, SOMAXCONN) == SOCKET_ERROR) {
        cleanupSocket();
        throw std::runtime_error("[ERROR] createSocket(): OBM socket listen failed...");
    }

    logMessage(
        LogLevel::INFO,
        "createSocket(): OBM socket created. Listening on port=" + std::to_string(obmPort),
        logging
    );

    return 1; // Socket created
}

//#########################################################################
int OrderBookManager::cleanupSocket() {
    logMessage(
        LogLevel::INFO,
        "cleanupSocket(): Closing OBM lsitener socket...",
        logging
    );

    // Close the socket if there is an existing valid socket
    if (obmSocket != INVALID_SOCKET) {
        closesocket(obmSocket);
        WSACleanup();
        obmSocket = INVALID_SOCKET;

        logMessage(
            LogLevel::INFO,
            "cleanupSocket(): OBM socket closed.",
            logging
        );

        return 1; // Socket closed
    }

    logMessage(
        LogLevel::WARN,
        "cleanupSocket(): Cannon close INVALID socket...",
        logging
    );

    return -1; // No socket to close
}

//#########################################################################
std::string OrderBookManager::serialize(const OrderResponse& response) {
    // Explicitly compute the buffer size
    constexpr size_t buffSize = sizeof(response.orderId) + sizeof(response.errCode);

    // Create the buffer
    std::string buffer(buffSize, '\0');
    size_t offset = 0;

    // Serialize the order response values
    memcpy(&buffer[offset], &response.orderId, sizeof(response.orderId));
    offset += sizeof(response.orderId);

    memcpy(&buffer[offset], &response.errCode, sizeof(response.errCode));
    offset += sizeof(response.errCode);

    return buffer;
}

//#########################################################################
OrderRequest OrderBookManager::deserialize(const std::string& buffer) {
    OrderRequest orderRequest{};
    size_t offset = 0;

    // Calculate the order request size
    size_t requestSize = sizeof(orderRequest.symbol) + sizeof(orderRequest.qty) +
                         sizeof(orderRequest.price) + sizeof(orderRequest.orderSide) +
                         sizeof(orderRequest.orderType);

    // Check the buffer size
    if (buffer.size() < requestSize) {
        throw std::runtime_error("[ERROR] deserialize(): Cannot dezerialize {OrderRequest}...");
    }

    // Deserialize the buffer into an order request
    memcpy(&orderRequest.symbol, &buffer[offset], sizeof(orderRequest.symbol));
    offset += sizeof(orderRequest.symbol);

    memcpy(&orderRequest.qty, &buffer[offset], sizeof(orderRequest.qty));
    offset += sizeof(orderRequest.qty);

    memcpy(&orderRequest.price, &buffer[offset], sizeof(orderRequest.price));
    offset += sizeof(orderRequest.price);

    memcpy(&orderRequest.orderSide, &buffer[offset], sizeof(orderRequest.orderSide));
    offset += sizeof(orderRequest.orderSide);

    memcpy(&orderRequest.orderType, &buffer[offset], sizeof(orderRequest.orderType));
    offset += sizeof(orderRequest.orderType);

    return orderRequest;
}
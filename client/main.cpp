#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

int main() {
    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    // Connect to the server
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);  // Use the same port as the server
    if (inet_pton(AF_INET, "127.0.0.1", &(serverAddress.sin_addr)) <= 0) {
        std::cerr << "Invalid address or address not supported." << std::endl;
        return 1;
    }

    if (connect(clientSocket, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) < 0) {
        std::cerr << "Connection failed." << std::endl;
        return 1;
    }

    // Send a message to the server
    const char* message = "Hello, World!";
    ssize_t bytesSent = send(clientSocket, message, strlen(message), 0);
    if (bytesSent < 0) {
        std::cerr << "Failed to send message." << std::endl;
        return 1;
    }

    // Close the socket
    close(clientSocket);

    return 0;
}

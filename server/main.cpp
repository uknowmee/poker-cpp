#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    // Bind the socket to an address and port
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080);  // You can choose any available port

    if (bind(serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) < 0) {
        std::cerr << "Failed to bind socket to address." << std::endl;
        return 1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 1) < 0) {
        std::cerr << "Failed to listen for connections." << std::endl;
        return 1;
    }

    // Accept a client connection
    sockaddr_in clientAddress{};
    socklen_t clientAddressLength = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, reinterpret_cast<struct sockaddr*>(&clientAddress), &clientAddressLength);
    if (clientSocket < 0) {
        std::cerr << "Failed to accept client connection." << std::endl;
        return 1;
    }

    // Receive and print the message from the client
    char buffer[1024] = {0};
    ssize_t bytesRead = read(clientSocket, buffer, sizeof(buffer));
    if (bytesRead < 0) {
        std::cerr << "Failed to read from socket." << std::endl;
        return 1;
    }

    std::cout << "Received message from client: " << buffer << std::endl;

    // Close the sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}

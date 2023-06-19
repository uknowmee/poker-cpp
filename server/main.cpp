#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <utility>
#include <vector>
#include <algorithm>

class ClientConnection {
public:
    ClientConnection(int socket, sockaddr_in address, std::string name)
            : socket(socket), address(address), name(std::move(name)) {}

    int getSocket() const { return socket; }
    sockaddr_in getAddress() const { return address; }
    std::string getName() const { return name; }

private:
    int socket;
    sockaddr_in address;
    std::string name;
};

class ConnectionManager {
public:
    void addConnection(ClientConnection* connection) {
        pthread_mutex_lock(&mutex);
        connections.push_back(connection);
        pthread_mutex_unlock(&mutex);
    }

    void removeConnection(ClientConnection* connection) {
        pthread_mutex_lock(&mutex);
        auto it = std::find_if(connections.begin(), connections.end(),
                               [connection](const ClientConnection* c) {
                                   return c == connection;
                               });

        if (it != connections.end()) {
            connections.erase(it);
        }
        pthread_mutex_unlock(&mutex);
    }


    void broadcastMessage(const std::string& message, const std::string& senderName) {
        pthread_mutex_lock(&mutex);

        for (const auto& connection : connections) {
            if (senderName != connection->getName()) {
                send(connection->getSocket(), message.c_str(), message.length(), 0);
            }
        }

        pthread_mutex_unlock(&mutex);
    }

private:
    std::vector<ClientConnection*> connections;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
};

class Server {
public:
    Server() {
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket < 0) {
            std::cerr << "Error creating server socket." << std::endl;
            exit(1);
        }

        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        serverAddress.sin_port = htons(8987);

        if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
            std::cerr << "Error binding server socket." << std::endl;
            exit(1);
        }

        if (listen(serverSocket, 5) < 0) {
            std::cerr << "Error listening for connections." << std::endl;
            exit(1);
        }

        std::cout << "Server started. Listening for connections..." << std::endl;
    }

    void start() {
        while (true) {
            socklen_t clientAddressLength = sizeof(clientAddress);
            int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
            if (clientSocket < 0) {
                std::cerr << "Error accepting client connection." << std::endl;
                continue;
            }

            char clientNameBuffer[256];
            memset(clientNameBuffer, 0, sizeof(clientNameBuffer));
            int nameBytesRead = recv(clientSocket, clientNameBuffer, sizeof(clientNameBuffer), 0);
            if (nameBytesRead <= 0) {
                std::cerr << "Error receiving client name." << std::endl;
                close(clientSocket);
                continue;
            }
            std::string clientName = clientNameBuffer;

            ClientConnection* connection = new ClientConnection(clientSocket, clientAddress, clientName);
            connectionManager.addConnection(connection);

            pthread_create(&clientThread, nullptr, handleClient, connection);
            pthread_detach(clientThread);
        }

        close(serverSocket);
    }

private:
    static void* handleClient(void* arg) {
        ClientConnection* connection = static_cast<ClientConnection*>(arg);
        int socket = connection->getSocket();
        std::string clientName = connection->getName();

        char buffer[4096];
        std::string message;

        while (true) {
            memset(buffer, 0, sizeof(buffer));
            int bytesRead = recv(socket, buffer, sizeof(buffer), 0);

            if (bytesRead <= 0) {
                break;
            }

            message = "[" + clientName + "]: " + buffer;
            std::cout << message << std::endl;
            connectionManager.broadcastMessage(message, clientName);
        }

        connectionManager.removeConnection(connection);
        close(socket);
        delete connection;

        return nullptr;
    }

    int serverSocket;
    sockaddr_in serverAddress, clientAddress;
    pthread_t clientThread;
    static ConnectionManager connectionManager;
};

ConnectionManager Server::connectionManager;

int main() {
    Server server;
    server.start();

    return 0;
}

//
// Created by micha on 19/06/2023.
//

#include <iostream>
#include <unistd.h>
#include <cstring>
#include "ConnectionManager.h"

ConnectionManager ConnectionManager::createConnectionManager(int serverPort, int numOfPlayers) {
    ConnectionManager connectionManager = ConnectionManager(serverPort, numOfPlayers);
    connectionManager.prepareServerSocket();

    return connectionManager;
}

ConnectionManager::ConnectionManager() = default;

ConnectionManager::ConnectionManager(int serverPort, int numOfPlayers)
        : serverPort(serverPort), maxNumOfPlayers(numOfPlayers) {
    connections = std::vector<ClientConnection *>();
    mutex = PTHREAD_MUTEX_INITIALIZER;
}

void ConnectionManager::prepareServerSocket() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Error creating server socket." << std::endl;
        exit(1);
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(serverPort);

    if (bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Error binding server socket." << std::endl;
        exit(1);
    }

    if (listen(serverSocket, 5) < 0) {
        std::cerr << "Error listening for connections." << std::endl;
        exit(1);
    }

    std::cout << "Server started. Listening for connections..." << std::endl;
}

ClientConnection *ConnectionManager::acceptConnection(bool isGameStarted) {
    sockaddr_in clientAddress{};

    socklen_t clientAddressLength = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientAddressLength);

    if (clientSocket < 0) {
        std::cerr << "Error accepting client connection." << std::endl;
        throw std::runtime_error("Error accepting client connection.");
    }

    if (isGameStarted) {
        throw std::runtime_error("Game already started. Closing connection.");
    }

    if (connections.size() == maxNumOfPlayers) {
        std::cout << "Maximum number of players reached. Closing connection." << std::endl;
        close(clientSocket);
        throw std::runtime_error("Maximum number of players reached. Closing connection.");
    }

    std::string clientName = findNewClientName();

    auto *connection = new ClientConnection(clientSocket, clientAddress, clientName);
    addConnection(connection);

    return connection;
}

void ConnectionManager::startListening(
        ClientConnection *connection,
        const std::function<void(const std::string &, const std::string &)>& onMessageReceived
) {
    int socket = connection->getSocket();
    std::string clientName = connection->getName();

    char buffer[4096];
    std::string message;

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        long bytesRead = recv(socket, buffer, sizeof(buffer), 0);

        if (bytesRead <= 0) {
            break;
        }

        onMessageReceived(buffer, clientName);
    }

    removeConnection(connection);
    close(socket);
    delete connection;
}

void ConnectionManager::closeServerSocket() const {
    close(serverSocket);
}

void ConnectionManager::addConnection(ClientConnection *connection) {
    pthread_mutex_lock(&mutex);
    connections.push_back(connection);
    pthread_mutex_unlock(&mutex);
}

void ConnectionManager::removeConnection(ClientConnection *connection) {
    pthread_mutex_lock(&mutex);
    connections.erase(std::remove(connections.begin(), connections.end(), connection), connections.end());
    pthread_mutex_unlock(&mutex);
}

void ConnectionManager::broadcastMessageExceptSender(const std::string &message, const std::string &senderName) {
    pthread_mutex_lock(&mutex);
    for (auto &connection: connections) {
        if (connection->getName() != senderName) {
            send(connection->getSocket(), message.c_str(), message.size(), 0);
        }
    }
    pthread_mutex_unlock(&mutex);
}

void ConnectionManager::sendToClientFromSender(
        const std::string &message,
        const std::string &senderName,
        const std::string &receiverName
) {
    pthread_mutex_lock(&mutex);
    for (auto &connection: connections) {
        if (connection->getName() == receiverName) {
            send(connection->getSocket(), message.c_str(), message.size(), 0);
        }
    }
    pthread_mutex_unlock(&mutex);
}

void ConnectionManager::sendToClient(const std::string &message, const std::string &receiverName) {
    pthread_mutex_lock(&mutex);
    for (auto &connection: connections) {
        if (connection->getName() == receiverName) {
            send(connection->getSocket(), message.c_str(), message.size(), 0);
        }
    }
    pthread_mutex_unlock(&mutex);
}

std::string ConnectionManager::findNewClientName() {
    std::string clientName = "Player";
    int i = 1;

    while (true) {
        bool nameExists = false;

        for (auto &connection: connections) {
            if (connection->getName() == clientName + std::to_string(i)) {
                nameExists = true;
                break;
            }
        }

        if (!nameExists) {
            break;
        }

        i++;
    }

    return clientName + std::to_string(i);
}

int ConnectionManager::getNumOfPlayers() const {
    return (int)connections.size();
}

void ConnectionManager::broadcastMessage(const std::string& message) {
    pthread_mutex_lock(&mutex);
    for (auto &connection: connections) {
        send(connection->getSocket(), message.c_str(), message.size(), 0);
    }
    pthread_mutex_unlock(&mutex);
}

void ConnectionManager::disconnectAllClients() {
    pthread_mutex_lock(&mutex);
    for (auto &connection: connections) {
        close(connection->getSocket());
        delete connection;
    }
    connections.clear();
    pthread_mutex_unlock(&mutex);
}

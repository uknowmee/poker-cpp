//
// Created by micha on 19/06/2023.
//

#include "Server.h"

ConnectionManager Server::connectionManager;

Server Server::createServer(int serverPort, int numOfPlayers) {
    return {serverPort, numOfPlayers};
}

Server::Server(int serverPort, int numOfPlayers) {

    // todo logic for creating game

    connectionManager = ConnectionManager::createConnectionManager(serverPort, numOfPlayers);
}

void Server::start() {
    while (true) {
        pthread_t clientThread;
        try {
            ClientConnection* connection = connectionManager.acceptConnection();

            addPlayer(connection->getName());

            HandleClientData data{};
            data.serverInstance = this;  // Assign the server instance
            data.clientConnection = connection;  // Assign the client connection

            pthread_create(&clientThread, nullptr, handleClient, &data);
            pthread_detach(clientThread);
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            continue;
        }
    }
    connectionManager.closeServerSocket();
}

void *Server::handleClient(void* arg) {

    auto* data = static_cast<HandleClientData*>(arg);

    Server* serverInstance = data->serverInstance;
    ClientConnection* clientConnection = data->clientConnection;

    connectionManager.startListening(clientConnection, serverInstance, onMessageReceived);

    serverInstance->removePlayer(clientConnection->getName());

    return nullptr;
}

void Server::onMessageReceived(const Server &serverInstance, const std::string &message, const std::string &senderName) {

    // todo logic for handling message

    std::string fullMessage = "[" + senderName + "]: " + message;
    std::cout << fullMessage << std::endl;
    connectionManager.broadcastMessage(fullMessage, senderName);
}

void Server::addPlayer(const std::string &playerName) {
    // todo logic for adding player to game
    connectionManager.broadcastMessage(playerName + " has joined the game!", playerName);
}

void Server::removePlayer(const std::string &playerName) {
    // todo logic for removing player from game
    connectionManager.broadcastMessage(playerName + " has left the game!", playerName);

}


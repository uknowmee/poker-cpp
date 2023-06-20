//
// Created by micha on 19/06/2023.
//

#include "Server.h"

ConnectionManager Server::connectionManager;

Server Server::createServer(int serverPort, int maxNumOfPlayers) {
    return {serverPort, maxNumOfPlayers};
}

Server::Server(int serverPort, int maxNumOfPlayers) {

    // todo logic for creating game
    this->maxNumOfPlayers = maxNumOfPlayers;
    connectionManager = ConnectionManager::createConnectionManager(serverPort, maxNumOfPlayers);
}

void Server::start() {
    while (true) {
        pthread_t clientThread;
        try {
            ClientConnection* connection = connectionManager.acceptConnection(isGameStarted);

            addPlayer(connection->getName());

            HandleClientData data{};
            data.serverInstance = this;
            data.clientConnection = connection;

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

    auto onMsgReceive = [serverInstance](const std::string& message, const std::string& senderName) {
        serverInstance->onMessageReceived(message, senderName);
    };

    connectionManager.startListening(clientConnection, onMsgReceive);

    serverInstance->removePlayer(clientConnection->getName());

    return nullptr;
}

void Server::onMessageReceived(const std::string &message, const std::string &senderName) {
    // todo logic for handling message

    std::string fullMessage = "[" + senderName + "]: " + message;
    std::cout << fullMessage << std::endl;
    connectionManager.broadcastMessage(fullMessage, senderName);
}

void Server::addPlayer(const std::string &playerName) {
    // todo logic for adding player to game

    std::string message = playerName + " has joined the game!";
    std::cout << message << std::endl;
    connectionManager.broadcastMessage(message, playerName);
    connectionManager.sendToClient("Welcome to the game! You are " + playerName, playerName);
    connectionManager.broadcastMessage("There are " + std::to_string(connectionManager.getNumOfPlayers()) + " players in the game");
    checkStartGame();
}

void Server::removePlayer(const std::string &playerName) {
    // todo logic for removing player from game

    std::string message = playerName + " has left the game!";
    std::cout << message << std::endl;
    connectionManager.broadcastMessage(message, playerName);
    connectionManager.broadcastMessage("There are " + std::to_string(connectionManager.getNumOfPlayers()) + " players in the game");
}

void Server::checkStartGame() {
    if (connectionManager.getNumOfPlayers() == maxNumOfPlayers) {
        if (!isGameStarted) {
            isGameStarted = true;
            std::cout << "The game has started!" << std::endl;
            connectionManager.broadcastMessage("The game has started!");
        }
    }
}


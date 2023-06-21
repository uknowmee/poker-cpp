//
// Created by micha on 19/06/2023.
//

#include "Server.h"

ConnectionManager Server::connectionManager;

Server Server::createServer(int serverPort, int maxNumOfPlayers) {
    return {serverPort, maxNumOfPlayers};
}

Server::Server(int serverPort, int maxNumOfPlayers) {
    this->maxNumOfPlayers = maxNumOfPlayers;
    connectionManager = ConnectionManager::createConnectionManager(serverPort, maxNumOfPlayers);
    messageHandler = MessageHandler::createMessageHandler(this);
    game = Game::createGame();
}

void Server::start() {
    try {
        acceptClientConnections();
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        connectionManager.closeServerSocket();
        return;
    }
}

void Server::acceptClientConnections() {
    while (true) {
        pthread_t clientThread;
        try {
            ClientConnection *connection = connectionManager.acceptConnection(game.isStarted());

            addPlayer(connection->getName());

            HandleClientData data{};
            data.serverInstance = this;
            data.clientConnection = connection;

            pthread_create(&clientThread, nullptr, handleClient, &data);
            pthread_detach(clientThread);
        } catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
            continue;
        }
    }
}

void *Server::handleClient(void *arg) {

    auto *data = static_cast<HandleClientData *>(arg);

    Server *serverInstance = data->serverInstance;
    ClientConnection *clientConnection = data->clientConnection;

    auto onMsgReceive = [serverInstance](const std::string &message, const std::string &senderName) {
        serverInstance->onMessageReceived(message, senderName);
    };

    connectionManager.startListening(clientConnection, onMsgReceive);

    serverInstance->removePlayer(clientConnection->getName());

    return nullptr;
}

void Server::onMessageReceived(const std::string &message, const std::string &senderName) {
    messageHandler.handleMessage(message, senderName);
}

void Server::addPlayer(const std::string &playerName) {
    game.addPlayer(playerName);

    std::string message = MessagePrinter::printAddPlayerMessage(playerName);
    broadcastMessageExceptSender(message, playerName);

    sendToClient(MessagePrinter::welcomeMessage(playerName), playerName);
    broadcastMessage(MessagePrinter::numberOfConnectedPlayersInfoMessage(connectionManager.getNumOfPlayers()));

    checkStartGame();
}

void Server::removePlayer(const std::string &playerName) {
    game.removePlayer(playerName);

    std::string message = MessagePrinter::printRemovePlayerMessage(playerName);
    broadcastMessageExceptSender(message, playerName);
    broadcastMessage(MessagePrinter::numberOfConnectedPlayersInfoMessage(connectionManager.getNumOfPlayers()));
}

void Server::checkStartGame() {
    if (connectionManager.getNumOfPlayers() != maxNumOfPlayers) {
        return;
    }

    if (!game.isStarted()) {
        game.start();
        MessagePrinter::printStartGameMessage();
        broadcastMessage(MessagePrinter::gameStartMessage());
    }
}

void Server::broadcastMessageExceptSender(const std::string &message, const std::string &senderName) {
    connectionManager.broadcastMessageExceptSender(message, senderName);
}

void Server::broadcastMessage(const std::string &message) {
    connectionManager.broadcastMessage(message);
}

void Server::sendToClientFromSender(
        const std::string &message,
        const std::string &senderName,
        const std::string &receiverName
) {
    connectionManager.sendToClientFromSender(message, senderName, receiverName);
}

void Server::sendToClient(const std::string &message, const std::string &receiverName) {
    connectionManager.sendToClient(message, receiverName);
}

void Server::invokeCommand(const std::string &command, const std::string &senderName) {
    // todo implement
    broadcastMessageExceptSender("[" + senderName + "]: " + command, senderName);
}

void Server::invokeCommand(const std::string &command, int value, const std::string &senderName) {
    // todo implement
}

void Server::invokeInvalidCommand(const std::string &messageInfo, const std::string &senderName) {
    sendToClient(messageInfo, senderName);
}

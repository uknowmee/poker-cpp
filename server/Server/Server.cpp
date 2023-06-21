//
// Created by micha on 19/06/2023.
//

#include "Server.h"

ConnectionManager Server::connectionManager;

Server Server::createServer(int serverPort, int maxNumOfPlayers) {
    return {serverPort, maxNumOfPlayers};
}

Server::Server(int serverPort, int maxNumOfPlayers)
        : gameService(GameService::createGameService(this)),
          maxNumOfPlayers(maxNumOfPlayers) {
    connectionManager = ConnectionManager::createConnectionManager(serverPort, maxNumOfPlayers);
    messageIdentifier = MessageIdentifier::createMessageIdentifier(
            dynamic_cast<ServerCommandInvoker *>(this)
    );
}

void Server::start() {
    try {
        startConsoleThread();
        acceptClientConnections();
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        stop();
    }
}

void Server::stop() {
    broadcastMessage(MessagePrinter::serverStopMessage());
    connectionManager.disconnectAllClients();
    connectionManager.closeServerSocket();
    exit(0);
}

void Server::startConsoleThread() {
    std::cout << MessagePrinter::serverStopCommandMessage() << std::endl;

    pthread_t consoleThread;
    pthread_create(&consoleThread, nullptr, handleConsole, this);
    pthread_detach(consoleThread);
}

void *Server::handleConsole(void *arg) {
    Server server = *static_cast<Server *>(arg);

    while (true) {
        std::string command;
        std::cin >> command;

        if (command == "/stop") {
            server.stop();
            return nullptr;
        }
    }
}

void Server::acceptClientConnections() {
    while (true) {
        pthread_t clientThread;
        try {
            ClientConnection *connection = connectionManager.acceptConnection(gameService.isGameStarted());

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
    messageIdentifier.identifyMessage(message, senderName);
}

void Server::addPlayer(const std::string &playerName) {
    gameService.addPlayer(playerName);

    std::string message = MessagePrinter::printAddPlayerMessage(playerName);
    broadcastMessageExceptSender(message, playerName);

    sendToClient(MessagePrinter::welcomeMessage(playerName), playerName);
    usleep(50000);
    broadcastMessage(MessagePrinter::numberOfConnectedPlayersInfoMessage(connectionManager.getNumOfPlayers()));

    checkStartGame();
}

void Server::removePlayer(const std::string &playerName) {
    gameService.removePlayer(playerName);

    std::string message = MessagePrinter::printRemovePlayerMessage(playerName);
    broadcastMessageExceptSender(message, playerName);
    broadcastMessage(MessagePrinter::numberOfConnectedPlayersInfoMessage(connectionManager.getNumOfPlayers()));
}

void Server::checkStartGame() {
    if (connectionManager.getNumOfPlayers() != maxNumOfPlayers) {
        return;
    }

    if (!gameService.isGameStarted()) {
        gameService.startGame();
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

void Server::disconnectClient(const std::string &clientName) {
    // todo implement
}

void Server::invokeByeCommand(const std::string &senderName) {
    // todo implement
}

void Server::invokeHelpCommand(const std::string &senderName) {
    // todo implement
}

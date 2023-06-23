//
// Created by micha on 19/06/2023.
//

#include "Server.h"

ConnectionManager Server::connectionManager;

Server *Server::createServer(int serverPort, int maxNumOfPlayers) {
    return new Server(serverPort, maxNumOfPlayers);
}

Server::Server(int serverPort, int maxNumOfPlayers)
        : gameService(GameService::createGameService(this, maxNumOfPlayers)),
          maxNumOfPlayers(maxNumOfPlayers) {
    connectionManager = ConnectionManager::createConnectionManager(serverPort, maxNumOfPlayers);
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

void Server::startConsoleThread() {
    std::cout << MessagePrinter::serverStopCommandMessage() << std::endl;

    pthread_t consoleThread;
    pthread_create(&consoleThread, nullptr, handleConsole, this);
    pthread_detach(consoleThread);
}

[[noreturn]] void *Server::handleConsole(void *arg) {
    auto *server = static_cast<Server *>(arg);

    while (true) {
        std::string command;
        std::cin >> command;

        Command *serverCommand = ServerCommandFactory::createCommand(server, command);
        pthread_mutex_lock(&server->consoleMutex);
        serverCommand->execute();
        pthread_mutex_unlock(&server->consoleMutex);
    }
}

void Server::acceptClientConnections() {
    while (true) {
        pthread_t clientThread;
        try {
            ClientConnection *connection = connectionManager.acceptConnection([this]() {
                return gameService.isGameStarted();
            });

            addPlayer(connection->getName());

            HandleClientData data = HandleClientData::CreateHandleClientData(this, connection);
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
        pthread_mutex_lock(&serverInstance->consoleMutex);
        serverInstance->onMessageReceived(message, senderName);
        pthread_mutex_unlock(&serverInstance->consoleMutex);
    };

    connectionManager.startListening(clientConnection, onMsgReceive);

    try {
        serverInstance->removeDisconnectedPlayer(clientConnection->getName());
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    return nullptr;
}

void Server::onMessageReceived(const std::string &message, const std::string &senderName) {
    ParsedMessage parsedMessage = MessageIdentifier::identifyMessage(message, senderName);
    Command *clientCommand = ClientCommandFactory::createCommand(this, parsedMessage, &gameService);
    clientCommand->execute();
}

void Server::addPlayer(const std::string &playerName) {
    gameService.addPlayer(playerName);
    checkStartGame();
}

void Server::checkStartGame() {
    if (connectionManager.getNumOfPlayers() != maxNumOfPlayers) { return; }
    gameService.tryStartGame();
}

void Server::removeDisconnectedPlayer(const std::string &playerName) {
    gameService.removeDisconnectedPlayer(playerName);
}

// ServerGameController
void Server::disconnectClient(const std::string &clientName) {
    connectionManager.disconnectClient(clientName);
}

// ServerMessageSender
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

// ServerCommandController
void Server::stop() {
    broadcastMessage(MessagePrinter::serverStopMessage());
    connectionManager.disconnectAllClients();
    connectionManager.closeServerSocket();
    sleep(1);
    exit(0);
}

void Server::printGameInfo() {
    std::cout << gameService.toString() << std::endl;
}

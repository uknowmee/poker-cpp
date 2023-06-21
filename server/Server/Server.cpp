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
    sleep(1);
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

    try {
        serverInstance->removeDisconnectedPlayer(clientConnection->getName());
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }

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

void Server::removeDisconnectedPlayer(const std::string &playerName) {
    gameService.removeDisconnectedPlayer(playerName);

    std::string message = MessagePrinter::printRemovePlayerMessage(playerName);
    broadcastMessageExceptSender(message, playerName);
    usleep(50000);
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

// ServerGameController
void Server::disconnectClient(const std::string &clientName) {
    std::string message = MessagePrinter::printDisconnectClientMessage(clientName);
    broadcastMessageExceptSender(message, clientName);
    broadcastMessage(MessagePrinter::numberOfConnectedPlayersInfoMessage(connectionManager.getNumOfPlayers()));

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

// ServerCommandInvoker
void Server::invokeByeCommand(const std::string &senderName) {
    sendToClient(MessagePrinter::byeMessage(), senderName);
    connectionManager.disconnectClient(senderName);
    gameService.removeDisconnectedPlayer(senderName);
}

void Server::invokeHelpCommand(const std::string &senderName) {
    sendToClient(MessagePrinter::helpMessage(), senderName);
}

void Server::invokeCommand(const std::string &command, const std::string &senderName) {
    // todo implement
}

void Server::invokeCommand(const std::string &command, int value, const std::string &senderName) {
    // todo implement
}

void Server::invokeCommand(const std::string &command, std::vector<int> values, const std::string &senderName) {
    // todo implement
}

void Server::invokeInvalidCommand(const std::string &messageInfo, const std::string &senderName) {
    sendToClient(messageInfo, senderName);
}

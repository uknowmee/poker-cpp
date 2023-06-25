//
// Created by micha on 19/06/2023.
//

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H


#include <netinet/in.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <mutex>
#include "../Connections/ConnectionManager.h"
#include "../Connections/ClientConnection.h"
#include "MessageIdentifier.h"
#include "MessagePrinter.h"
#include "ServerMessageSender.h"
#include "../Game/GameService.h"
#include "ServerGameController.h"
#include "ServerCommandController.h"
#include "../Command/ClientCommand/ClientCommandFactory.h"
#include "../Command/ServerCommand/ServerCommandFactory.h"

class Server : public ServerGameController, ServerCommandController {

private:
    struct HandleClientData {
    private:
        HandleClientData(Server *serverInstance, ClientConnection *clientConnection)
                : serverInstance(serverInstance), clientConnection(clientConnection) {};

    public:
        Server *serverInstance;
        ClientConnection *clientConnection;

        HandleClientData() : serverInstance(nullptr), clientConnection(nullptr) {};
        static HandleClientData CreateHandleClientData(Server *serverInstance, ClientConnection *clientConnection) {
            return {serverInstance, clientConnection};
        }
    };

    pthread_mutex_t consoleMutex{};
    int maxNumOfPlayers;
    static ConnectionManager connectionManager;
    GameService gameService;

    // Server
    Server(int serverPort, int maxNumOfPlayers);
    void startConsoleThread();
    void acceptClientConnections();

    [[noreturn]] static void *handleConsole(void *arg);
    static void *handleClient(void *arg);
    void addPlayer(const std::string &playerName);
    void removeDisconnectedPlayer(const std::string &playerName);
    void onMessageReceived(const std::string &message, const std::string &senderName);
    void checkStartGame();

    // ServerGameController
    void disconnectClient(const std::string &clientName) override;

    // ServerMessageSender
    void broadcastMessageExceptSender(
            const std::string &message,
            const std::string &senderName
    ) override;
    void broadcastMessage(const std::string &message) override;
    void sendToClientFromSender(
            const std::string &message,
            const std::string &senderName,
            const std::string &receiverName
    ) override;
    void sendToClient(const std::string &message, const std::string &receiverName) override;

    // ServerCommandController
    void stop() override;
    void printGameInfo() override;

public:
    static Server *createServer(int serverPort, int maxNumOfPlayers);
    void start();
};


#endif //SERVER_SERVER_H

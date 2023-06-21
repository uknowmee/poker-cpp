//
// Created by micha on 19/06/2023.
//

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H


#include <netinet/in.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include "../Connections/ConnectionManager.h"
#include "../Connections/ClientConnection.h"
#include "MessageIdentifier.h"
#include "MessagePrinter.h"
#include "ServerMessageSender.h"
#include "../Game/GameService.h"
#include "ServerGameController.h"
#include "ServerCommandInvoker.h"

class Server : public ServerGameController, ServerCommandInvoker {

private:
    int maxNumOfPlayers;
    struct HandleClientData {
        Server *serverInstance;
        ClientConnection *clientConnection;
    };
    static ConnectionManager connectionManager;
    MessageIdentifier messageIdentifier;
    GameService gameService;

    // Server
    Server(int serverPort, int maxNumOfPlayers);
    void startConsoleThread();
    void acceptClientConnections();
    static void *handleConsole(void *arg);
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

    // ServerCommandInvoker
    void invokeCommand(const std::string &command, const std::string &senderName) override;
    void invokeCommand(const std::string &command, int value, const std::string &senderName) override;
    void invokeCommand(const std::string &command, std::vector<int> values, const std::string &senderName) override;
    void invokeInvalidCommand(const std::string &messageInfo, const std::string &senderName) override;
    void invokeByeCommand(const std::string &senderName) override;
    void invokeHelpCommand(const std::string &senderName) override;

public:
    static Server createServer(int serverPort, int maxNumOfPlayers);
    void start();
    void stop();
};


#endif //SERVER_SERVER_H

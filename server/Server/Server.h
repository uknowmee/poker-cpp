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
#include "MessageHandler.h"
#include "../Game/Game.h"
#include "ServerMessageSender.h"
#include "ServerCommandInvoker.h"

class Server : public ServerMessageSender, ServerCommandInvoker {

private:
    int maxNumOfPlayers;
    struct HandleClientData {
        Server *serverInstance;
        ClientConnection *clientConnection;
    };
    static ConnectionManager connectionManager;
    MessageHandler messageHandler;
    Game game;

    Server(int serverPort, int maxNumOfPlayers);
    void acceptClientConnections();
    void addPlayer(const std::string &playerName);
    void removePlayer(const std::string &playerName);
    static void *handleClient(void *arg);
    void onMessageReceived(const std::string &message, const std::string &senderName);
    void checkStartGame();
    void invokeByeCommand(const std::string &senderName);
    void invokeHelpCommand(const std::string &senderName);

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

    void invokeCommand(const std::string &command, const std::string &senderName) override;
    void invokeCommand(const std::string &command, int value, const std::string &senderName) override;
    void invokeInvalidCommand(const std::string &messageInfo, const std::string &senderName) override;

public:
    static Server createServer(int serverPort, int maxNumOfPlayers);
    void start();
};


#endif //SERVER_SERVER_H

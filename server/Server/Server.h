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

class Server {

private:
    static ConnectionManager connectionManager;
    bool isGameStarted = false;
    int maxNumOfPlayers;
    struct HandleClientData {
        Server *serverInstance;
        ClientConnection *clientConnection;
    };

    Server(int serverPort, int maxNumOfPlayers);
    static void *handleClient(void *arg);
    void addPlayer(const std::string &playerName);
    void removePlayer(const std::string &playerName);

public:
    static Server createServer(int serverPort, int maxNumOfPlayers);
    void start();
    void onMessageReceived(const std::string &message, const std::string &senderName);

    void checkStartGame();
};


#endif //SERVER_SERVER_H

//
// Created by micha on 19/06/2023.
//

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H


#include <netinet/in.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include "ConnectionManager.h"
#include "ClientConnection.h"

class Server {

private:
    static ConnectionManager connectionManager;
    struct HandleClientData {
        Server *serverInstance;
        ClientConnection *clientConnection;
    };

    Server(int serverPort, int numOfPlayers);
    static void *handleClient(void *arg);
    void addPlayer(const std::string &playerName);
    void removePlayer(const std::string &playerName);

public:
    static Server createServer(int serverPort, int numOfPlayers);
    void start();
    static void onMessageReceived(const Server &serverInstance,
                                  const std::string &message,
                                  const std::string &senderName
    );
};


#endif //SERVER_SERVER_H

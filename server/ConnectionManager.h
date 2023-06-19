//
// Created by micha on 19/06/2023.
//

#ifndef SERVER_CONNECTIONMANAGER_H
#define SERVER_CONNECTIONMANAGER_H


#include <csignal>
#include <vector>
#include <algorithm>
#include "ClientConnection.h"

class ConnectionManager {

private:
    std::vector<ClientConnection*> connections;
    pthread_mutex_t mutex{};
    int numOfPlayers{};

    int serverSocket{};
    int serverPort{};
    sockaddr_in serverAddress{};

    ConnectionManager(int serverPort, int numOfPlayers);
    void prepareServerSocket();

public:
    ConnectionManager();

    static ConnectionManager createConnectionManager(int serverPort, int numOfPlayers);
    ClientConnection* acceptConnection();
    void startListening(ClientConnection *connection, void (*onMessageReceived)(const std::string&, const std::string&));
    void closeServerSocket() const;
    void addConnection(ClientConnection* connection);
    void removeConnection(ClientConnection* connection);
    void broadcastMessage(const std::string& message, const std::string& senderName);
    void sendToClient(const std::string& message, const std::string& senderName, const std::string& receiverName);
    void sendToClient(const std::string& message, const std::string& receiverName);
};


#endif //SERVER_CONNECTIONMANAGER_H

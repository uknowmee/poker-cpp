//
// Created by micha on 19/06/2023.
//

#ifndef SERVER_CONNECTIONMANAGER_H
#define SERVER_CONNECTIONMANAGER_H


#include <csignal>
#include <vector>
#include <algorithm>
#include <functional>
#include "ClientConnection.h"

class ConnectionManager {

private:
    std::vector<ClientConnection*> connections;
    pthread_mutex_t mutex{};
    int maxNumOfPlayers{};

    int serverSocket{};
    int serverPort{};
    sockaddr_in serverAddress{};

    ConnectionManager(int serverPort, int numOfPlayers);
    void prepareServerSocket();
    std::string findNewClientName();

public:
    ConnectionManager();

    static ConnectionManager createConnectionManager(int serverPort, int numOfPlayers);
    ClientConnection *acceptConnection(bool isGameSta, bool should);
    void startListening(
            ClientConnection *connection,
            const std::function<void(const std::string&, const std::string&)>&
    );
    void closeServerSocket() const;
    void addConnection(ClientConnection* connection);
    void removeConnection(ClientConnection* connection);
    void broadcastMessageExceptSender(const std::string& message, const std::string& senderName);
    void broadcastMessage(const std::string& message);
    void sendToClientFromSender(const std::string& message, const std::string& senderName, const std::string& receiverName);
    void sendToClient(const std::string& message, const std::string& receiverName);
    int getNumOfPlayers() const;
    void disconnectAllClients();
    void disconnectClient(const std::string &clientName);
};


#endif //SERVER_CONNECTIONMANAGER_H

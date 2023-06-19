//
// Created by micha on 19/06/2023.
//

#include "ClientConnection.h"
#include "ConnectionManager.h"

ClientConnection::ClientConnection(int socket, sockaddr_in address, std::string name)
        : socket(socket), address(address), name(std::move(name)) {}

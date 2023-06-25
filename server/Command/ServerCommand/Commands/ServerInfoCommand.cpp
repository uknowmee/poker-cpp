//
// Created by micha on 23/06/2023.
//

#include "ServerInfoCommand.h"

ServerInfoCommand::ServerInfoCommand(ServerCommandController *pController)
        : ServerCommand(pController) {
}

void ServerInfoCommand::execute() {
    server->printGameInfo();
}


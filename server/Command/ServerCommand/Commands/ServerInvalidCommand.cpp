//
// Created by micha on 23/06/2023.
//

#include "ServerInvalidCommand.h"

ServerInvalidCommand::ServerInvalidCommand(ServerCommandController *pController) : ServerCommand(pController) {
}

void ServerInvalidCommand::execute() {
    std::cout << MessagePrinter::invalidServerCommand() << std::endl;
}


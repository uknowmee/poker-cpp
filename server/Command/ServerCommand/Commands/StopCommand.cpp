//
// Created by micha on 23/06/2023.
//

#include "StopCommand.h"

StopCommand::StopCommand(ServerCommandController *pController) : ServerCommand(pController) {
}

void StopCommand::execute() {
    server->stop();
}

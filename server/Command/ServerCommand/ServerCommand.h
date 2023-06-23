//
// Created by micha on 23/06/2023.
//

#ifndef MY_PROJECTS_SERVERCOMMAND_H
#define MY_PROJECTS_SERVERCOMMAND_H


#include <iostream>
#include "../Command.h"
#include "../../Server/ServerCommandController.h"

class ServerCommand : public Command {

protected:
    ServerCommandController *server;

    explicit ServerCommand(ServerCommandController *pController)
            : server(pController) {};
};

#endif //MY_PROJECTS_SERVERCOMMAND_H

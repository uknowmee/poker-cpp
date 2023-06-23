//
// Created by micha on 23/06/2023.
//

#ifndef MY_PROJECTS_SERVERINVALIDCOMMAND_H
#define MY_PROJECTS_SERVERINVALIDCOMMAND_H


#include "../ServerCommand.h"

class ServerInvalidCommand : public ServerCommand {

private:
    void execute() override;

public:
    explicit ServerInvalidCommand(ServerCommandController *pController);
};


#endif //MY_PROJECTS_SERVERINVALIDCOMMAND_H

//
// Created by micha on 23/06/2023.
//

#ifndef MY_PROJECTS_STOPCOMMAND_H
#define MY_PROJECTS_STOPCOMMAND_H


#include "../ServerCommand.h"

class StopCommand : public ServerCommand {

private:
    void execute() override;

public:
    explicit StopCommand(ServerCommandController *pController);
};


#endif //MY_PROJECTS_STOPCOMMAND_H

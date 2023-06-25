//
// Created by micha on 23/06/2023.
//

#ifndef MY_PROJECTS_BYECOMMAND_H
#define MY_PROJECTS_BYECOMMAND_H


#include "../../ClientCommand/ClientCommand.h"

class ByeCommand : public ClientCommand{

private:
    void execute() override;

public:
    explicit ByeCommand(
            ServerGameController *server,
            const ParsedMessage &parsedMessage,
            GameServiceCommandController *gameService
    );
};


#endif //MY_PROJECTS_BYECOMMAND_H

//
// Created by micha on 23/06/2023.
//

#ifndef MY_PROJECTS_BETCOMMAND_H
#define MY_PROJECTS_BETCOMMAND_H


#include "../ClientCommand.h"

class BetCommand : public ClientCommand {

private:
    void execute() override;

public:
    explicit BetCommand(
            ServerGameController *server,
            const ParsedMessage &parsedMessage,
            GameServiceCommandController *gameService
    );
};


#endif //MY_PROJECTS_BETCOMMAND_H

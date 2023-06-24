//
// Created by micha on 23/06/2023.
//

#ifndef MY_PROJECTS_RAISECOMMAND_H
#define MY_PROJECTS_RAISECOMMAND_H


#include "../ClientCommand.h"

class RaiseCommand : public ClientCommand {

private:
    bool exactExecute() override;

public:
    explicit RaiseCommand(
            ServerGameController *server,
            const ParsedMessage &parsedMessage,
            GameServiceCommandController *gameService
    );
};


#endif //MY_PROJECTS_RAISECOMMAND_H

//
// Created by micha on 23/06/2023.
//

#ifndef MY_PROJECTS_CHECKCOMMAND_H
#define MY_PROJECTS_CHECKCOMMAND_H


#include "../ClientCommand.h"

class CheckCommand : public ClientCommand {

private:
    bool exactExecute() override;

public:
    explicit CheckCommand(
            ServerGameController *server,
            const ParsedMessage &parsedMessage,
            GameServiceCommandController *gameService
    );
};


#endif //MY_PROJECTS_CHECKCOMMAND_H

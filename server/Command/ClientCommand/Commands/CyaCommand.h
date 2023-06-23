//
// Created by micha on 23/06/2023.
//

#ifndef MY_PROJECTS_CYACOMMAND_H
#define MY_PROJECTS_CYACOMMAND_H


#include "../ClientCommand.h"

class CyaCommand : public ClientCommand{

private:
    void execute() override;

public:
    explicit CyaCommand(
            ServerGameController *server,
            const ParsedMessage &parsedMessage,
            GameServiceCommandController *gameService
    );
};


#endif //MY_PROJECTS_CYACOMMAND_H

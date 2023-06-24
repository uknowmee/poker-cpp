//
// Created by micha on 23/06/2023.
//

#ifndef MY_PROJECTS_FOLDCOMMAND_H
#define MY_PROJECTS_FOLDCOMMAND_H


#include "../ClientCommand.h"

class FoldCommand : public ClientCommand{

private:
    void exactExecute() override;

    public:
    explicit FoldCommand(
            ServerGameController *server,
            const ParsedMessage &parsedMessage,
            GameServiceCommandController *gameService
    );
};


#endif //MY_PROJECTS_FOLDCOMMAND_H

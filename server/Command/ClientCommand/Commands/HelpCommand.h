//
// Created by micha on 23/06/2023.
//

#ifndef MY_PROJECTS_HELPCOMMAND_H
#define MY_PROJECTS_HELPCOMMAND_H


#include "../ClientCommand.h"

class HelpCommand : public ClientCommand {

private:
    std::string playerName;
    void execute() override;

public:
    explicit HelpCommand(
            ServerGameController *server,
            const ParsedMessage &parsedMessage,
            GameServiceCommandController *gameService
    );
};


#endif //MY_PROJECTS_HELPCOMMAND_H

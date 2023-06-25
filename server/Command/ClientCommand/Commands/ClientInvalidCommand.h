//
// Created by micha on 23/06/2023.
//

#ifndef MY_PROJECTS_CLIENTINVALIDCOMMAND_H
#define MY_PROJECTS_CLIENTINVALIDCOMMAND_H


#include "../ClientCommand.h"

class ClientInvalidCommand : public ClientCommand {
private:
    std::string playerName;
    std::string message;

    void execute() override;

public:
    explicit ClientInvalidCommand(
            ServerGameController *server,
            const ParsedMessage &parsedMessage,
            GameServiceCommandController *gameService
    );
};


#endif //MY_PROJECTS_CLIENTINVALIDCOMMAND_H

//
// Created by micha on 23/06/2023.
//

#ifndef MY_PROJECTS_CLIENTINFOCOMMAND_H
#define MY_PROJECTS_CLIENTINFOCOMMAND_H


#include "../ClientCommand.h"

class ClientInfoCommand : public ClientCommand{

private:
    std::string playerName;
    std::string playerInfo;

    void execute() override;
    std::string infoMessage();

public:
    explicit ClientInfoCommand(
            ServerGameController *server,
            const ParsedMessage &parsedMessage,
            GameServiceCommandController *gameService
    );
};


#endif //MY_PROJECTS_CLIENTINFOCOMMAND_H

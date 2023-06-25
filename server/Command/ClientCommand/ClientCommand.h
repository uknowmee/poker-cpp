//
// Created by micha on 23/06/2023.
//

#ifndef MY_PROJECTS_CLIENTCOMMAND_H
#define MY_PROJECTS_CLIENTCOMMAND_H

#include <iostream>
#include "../Command.h"
#include "../../Server/ServerGameController.h"
#include "../../Game/GameServiceCommandController.h"

class ClientCommand : public Command {

protected:
    ServerGameController *server;
    const ParsedMessage &parsedMessage;
    GameServiceCommandController *gameService;

    explicit ClientCommand(
            ServerGameController *pController,
            const ParsedMessage &pParsedMessage,
            GameServiceCommandController *gameService);

    virtual MoveInfo exactExecute() {return MoveInfo();};
    void execute() override;
    void sendGameNotStartedMessage();
    void sendNotYourTurnMessage();
    void sendToAllPlayersWhatHappened();
    void sendPlayerThatMoveNotAllowed();
    std::string gameNotStartedMessage();

private:

    std::string notYourTurnMessage();
    void sendExchangeStartedMessage();
};


#endif //MY_PROJECTS_CLIENTCOMMAND_H

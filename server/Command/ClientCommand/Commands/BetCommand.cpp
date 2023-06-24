//
// Created by micha on 23/06/2023.
//

#include "BetCommand.h"

BetCommand::BetCommand(
        ServerGameController *server,
        const ParsedMessage &parsedMessage,
        GameServiceCommandController *gameService
) : ClientCommand(server, parsedMessage, gameService) {
}

void BetCommand::exactExecute() {
    gameService->invokeBet(parsedMessage.senderName, parsedMessage.value);
}

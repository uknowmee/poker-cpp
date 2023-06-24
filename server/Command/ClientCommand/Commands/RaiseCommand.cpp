//
// Created by micha on 23/06/2023.
//

#include "RaiseCommand.h"

RaiseCommand::RaiseCommand(
        ServerGameController *server,
        const ParsedMessage &parsedMessage,
        GameServiceCommandController *gameService
) : ClientCommand(server, parsedMessage, gameService) {

}

void RaiseCommand::exactExecute() {
    gameService->invokeRaise(parsedMessage.senderName, parsedMessage.value);
}
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

bool RaiseCommand::exactExecute() {
    return gameService->invokeRaise(parsedMessage.senderName, parsedMessage.value);
}
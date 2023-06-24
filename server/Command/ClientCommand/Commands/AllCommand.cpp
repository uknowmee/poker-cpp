//
// Created by micha on 23/06/2023.
//

#include "AllCommand.h"

AllCommand::AllCommand(
        ServerGameController *server,
        const ParsedMessage &parsedMessage,
        GameServiceCommandController *gameService
) : ClientCommand(server, parsedMessage, gameService) {
}

bool AllCommand::exactExecute() {
    return gameService->invokeAll(parsedMessage.senderName);
}
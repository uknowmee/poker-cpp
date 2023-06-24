//
// Created by micha on 23/06/2023.
//

#include "CheckCommand.h"

CheckCommand::CheckCommand(
        ServerGameController *server,
        const ParsedMessage &parsedMessage,
        GameServiceCommandController *gameService
) : ClientCommand(server, parsedMessage, gameService) {

}

bool CheckCommand::exactExecute() {
    return gameService->invokeCheck(parsedMessage.senderName);
}
//
// Created by micha on 23/06/2023.
//

#include "CallCommand.h"

CallCommand::CallCommand(
        ServerGameController *server,
        const ParsedMessage &parsedMessage,
        GameServiceCommandController *gameService
) : ClientCommand(server, parsedMessage, gameService) {

}

bool CallCommand::exactExecute() {
    return gameService->invokeCall(parsedMessage.senderName);
}

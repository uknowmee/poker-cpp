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

void CallCommand::execute() {

}

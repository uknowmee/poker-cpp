//
// Created by micha on 23/06/2023.
//

#include "FoldCommand.h"

FoldCommand::FoldCommand(
        ServerGameController *server,
        const ParsedMessage &parsedMessage,
        GameServiceCommandController *gameService
) : ClientCommand(server, parsedMessage, gameService) {

}

void FoldCommand::execute() {

}
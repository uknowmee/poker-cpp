//
// Created by micha on 23/06/2023.
//

#include "HelpCommand.h"

HelpCommand::HelpCommand(
        ServerGameController *server,
        const ParsedMessage &parsedMessage,
        GameServiceCommandController *gameService
) : ClientCommand(server, parsedMessage, gameService) {
    playerName = parsedMessage.senderName;
}

void HelpCommand::execute() {
    server->sendToClient(MessagePrinter::helpMessage(), playerName);
}

//
// Created by micha on 23/06/2023.
//

#include "ClientInvalidCommand.h"

ClientInvalidCommand::ClientInvalidCommand(
        ServerGameController *server,
        const ParsedMessage &parsedMessage,
        GameServiceCommandController *gameService
) : ClientCommand(server, parsedMessage, gameService) {
    playerName = parsedMessage.senderName;
    message = parsedMessage.message;
}

void ClientInvalidCommand::execute() {
    server->sendToClient(MessagePrinter::invalidCommandMessage(message), playerName);
}
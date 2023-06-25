//
// Created by micha on 23/06/2023.
//

#include "ByeCommand.h"


ByeCommand::ByeCommand(
        ServerGameController *server,
        const ParsedMessage &parsedMessage,
        GameServiceCommandController *gameService
) : ClientCommand(server, parsedMessage, gameService) {

}

void ByeCommand::execute() {
    server->sendToClient(MessagePrinter::byeMessage(), parsedMessage.senderName);
    server->disconnectClient(parsedMessage.senderName);
}

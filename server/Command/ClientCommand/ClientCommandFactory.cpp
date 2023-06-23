//
// Created by micha on 23/06/2023.
//

#include "ClientCommandFactory.h"
#include "Commands/ClientInfoCommand.h"
#include "Commands/ByeCommand.h"
#include "Commands/HelpCommand.h"
#include "Commands/ClientInvalidCommand.h"

Command *ClientCommandFactory::createCommand(
        ServerGameController *server,
        const ParsedMessage &parsedMessage,
        GameServiceCommandController *gameService
) {
    if (parsedMessage.command == "info") { return new ClientInfoCommand(server, parsedMessage, gameService); }
    else if (parsedMessage.command == "bye") { return new ByeCommand(server, parsedMessage, gameService); }
    else if (parsedMessage.command == "help") { return new HelpCommand(server, parsedMessage, gameService); }
    else { return new ClientInvalidCommand(server, parsedMessage, gameService); }
}

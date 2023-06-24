//
// Created by micha on 23/06/2023.
//

#include "ClientCommandFactory.h"
#include "Commands/ClientInfoCommand.h"
#include "Commands/ByeCommand.h"
#include "Commands/HelpCommand.h"
#include "Commands/ClientInvalidCommand.h"
#include "Commands/AllCommand.h"
#include "Commands/BetCommand.h"
#include "Commands/CallCommand.h"
#include "Commands/CheckCommand.h"
#include "Commands/CyaCommand.h"
#include "Commands/ExchangeCommand.h"
#include "Commands/FoldCommand.h"
#include "Commands/RaiseCommand.h"

Command *ClientCommandFactory::createCommand(
        ServerGameController *server,
        const ParsedMessage &parsedMessage,
        GameServiceCommandController *gameService
) {
    if (parsedMessage.command == "info") { return new ClientInfoCommand(server, parsedMessage, gameService); }
    else if (parsedMessage.command == "bye") { return new ByeCommand(server, parsedMessage, gameService); }
    else if (parsedMessage.command == "help") { return new HelpCommand(server, parsedMessage, gameService); }
    else if (parsedMessage.command == "all") { return new AllCommand(server, parsedMessage, gameService); }
    else if (parsedMessage.command == "bet") { return new BetCommand(server, parsedMessage, gameService); }
    else if (parsedMessage.command == "call") { return new CallCommand(server, parsedMessage, gameService); }
    else if (parsedMessage.command == "check") { return new CheckCommand(server, parsedMessage, gameService); }
    else if (parsedMessage.command == "cya") { return new CyaCommand(server, parsedMessage, gameService); }
    else if (parsedMessage.command == "exchange") { return new ExchangeCommand(server, parsedMessage, gameService); }
    else if (parsedMessage.command == "fold") { return new FoldCommand(server, parsedMessage, gameService); }
    else if (parsedMessage.command == "raise") { return new RaiseCommand(server, parsedMessage, gameService); }
    else { return new ClientInvalidCommand(server, parsedMessage, gameService); }
}

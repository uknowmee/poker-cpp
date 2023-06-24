//
// Created by micha on 23/06/2023.
//

#include "ClientInfoCommand.h"

ClientInfoCommand::ClientInfoCommand(
        ServerGameController *server,
        const ParsedMessage &parsedMessage,
        GameServiceCommandController *gameService
) : ClientCommand(server, parsedMessage, gameService) {

    playerName = parsedMessage.senderName;
    playerInfo = gameService->isGameStarted()
                 ? gameService->playingPlayerInfo(playerName)
                 : gameNotStartedMessage();
}

void ClientInfoCommand::execute() {
    server->sendToClient(infoMessage(), playerName);
}

std::string ClientInfoCommand::infoMessage() {
    return MessagePrinter::playerInfoMessage(playerInfo);
}

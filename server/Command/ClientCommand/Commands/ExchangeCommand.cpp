//
// Created by micha on 23/06/2023.
//

#include "ExchangeCommand.h"

ExchangeCommand::ExchangeCommand(
        ServerGameController *server,
        const ParsedMessage &parsedMessage,
        GameServiceCommandController *gameService
) : ClientCommand(server, parsedMessage, gameService) {
}

void ExchangeCommand::execute() {
    if (!gameService->isGameStarted()) { return sendGameNotStartedMessage(); }
    if (!gameService->isPlayerTurn(parsedMessage.senderName)) { return sendNotYourTurnMessage(); }
    gameService->invokeExchange(parsedMessage.senderName, parsedMessage.values);
    return sendToAllPlayersWhatHappened();
}
//
// Created by micha on 23/06/2023.
//

#include "ClientCommand.h"

ClientCommand::ClientCommand(
        ServerGameController *pController,
        const ParsedMessage &pParsedMessage,
        GameServiceCommandController *gameService
) : server(pController),
    parsedMessage(pParsedMessage),
    gameService(gameService) {

}

void ClientCommand::execute() {
    if (!gameService->isGameStarted()) { return sendGameNotStartedMessage(); }
    if (!gameService->isPlayerTurn(parsedMessage.senderName)) { return sendNotYourTurnMessage(); }
    if (exactExecute()) { return sendToAllPlayersWhatHappened(); }

    sendPlayerThatMoveNotAllowed();
}

void ClientCommand::sendGameNotStartedMessage() {
    return server->sendToClient(gameNotStartedMessage(), parsedMessage.senderName);
}

void ClientCommand::sendNotYourTurnMessage() {
    return server->sendToClient(notYourTurnMessage(), parsedMessage.senderName);
}

std::string ClientCommand::notYourTurnMessage() {
    std::string currentPlayerName = gameService->currentPlayerName();
    return MessagePrinter::notYourTurnMessage(parsedMessage.senderName, currentPlayerName);
}

std::string ClientCommand::gameNotStartedMessage() {
    return MessagePrinter::gameNotStartedMessage(gameService->numOfPlayers());
}

void ClientCommand::sendToAllPlayersWhatHappened() {
    server->broadcastMessage(
            MessagePrinter::whatHappenedMessage(
                    parsedMessage.message, gameService->currentPlayerName(), gameService->lastPlayerName()
            )
    );
}

void ClientCommand::sendPlayerThatMoveNotAllowed() {
    server->sendToClient(MessagePrinter::moveNotAllowedMessage(parsedMessage.message), parsedMessage.senderName);
}

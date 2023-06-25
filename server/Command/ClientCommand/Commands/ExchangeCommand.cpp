//
// Created by micha on 23/06/2023.
//

#include "ExchangeCommand.h"

ExchangeCommand::ExchangeCommand(
        ServerGameController *server,
        const ParsedMessage &parsedMessage,
        GameServiceCommandController *gameService
) : ClientCommand(server, parsedMessage, gameService) {
    deckMaster = gameService->getDeckMaster();
    game = gameService->getGame();
    senderName = parsedMessage.senderName;
    cardsToExchange = parsedMessage.values;
}

void ExchangeCommand::execute() {
    if (!gameService->isGameStarted()) { return sendGameNotStartedMessage(); }
    if (!gameService->isPlayerTurn(parsedMessage.senderName)) { return sendNotYourTurnMessage(); }
    if (!game->currentPlayer().isExchange()) { return sendPlayerThatMoveNotAllowed(); }

    proceedExchange();

}

void ExchangeCommand::proceedExchange() {

    game->currentPlayer().setExchange(false);
    gameService->updateQueue();
    DeckMaster::collectPlayerCards(game->lastPlayer(), game->getCardsRef(), cardsToExchange);
    server->sendToClient(MessagePrinter::printExchangeAccepted(), senderName);
    sendToAllPlayersWhatHappened();

    if (!game->currentPlayer().isExchange()) {
        DeckMaster::dealTheCards(game->getPlayingPlayersRef(), game->getCardsRef());
        game->adjustPart();
        game->setBid(0);
        deckMaster->evaluatePlayingPlayersCards(game->getPlayingPlayersRef());

        server->broadcastMessage(MessagePrinter::printExchangeFinishedMessage());
        gameService->sendInfoToAllPlayingPlayers();

        checkAllInCase();
    }
}

void ExchangeCommand::checkAllInCase() {
    if (!game->isAllIn()) { return; }

    // bypassing allIn in standard poker game logic.
    MoveInfo moveInfo = gameService->exchangeMoveAcceptedInAllInCase();
    if (moveInfo == MoveInfo::ROUND_FINISHED) { return gameService->finishRound(); }
    else if (moveInfo == MoveInfo::GAME_FINISHED) { return gameService->finishGame(); }
}

//
// Created by micha on 23/06/2023.
//

#include "BetCommand.h"

BetCommand::BetCommand(
        ServerGameController *server,
        const ParsedMessage &parsedMessage,
        GameServiceCommandController *gameService
) : ClientCommand(server, parsedMessage, gameService) {
    deckMaster = gameService->getDeckMaster();
    game = gameService->getGame();
    senderName = parsedMessage.senderName;
    value = parsedMessage.value;
}

MoveInfo BetCommand::exactExecute() {
    deckMaster->evaluatePlayingPlayersCards(game->getPlayingPlayersRef());
    Player &player = game->currentPlayer();
    int bid = game->getBid();
    int credit = player.getCredit();
    if (bid != 0 || credit <= 2 || game->isAllIn() || player.isExchange()) { return MoveInfo::NOT_ALLOWED; }

    if (value <= 2) { return handleToLowBet(); }
    if (value > credit) { return handleToHighBet(); }

    game->setBid(value);
    game->addToBank(value);
    gameService->removeCreditFromPlayerAndPlayingPlayer(player, value);
    player.setBet(true);
    player.setTurn(false);

    std::for_each(
            game->getPlayingPlayersRef().begin(), game->getPlayingPlayersRef().end(),
            [this](Player &player) {
                if (player.isFold()) { return; }
                player.addToDiff(value);
                player.setCheck(false);
            }
    );

    return gameService->moveAccepted();
}

MoveInfo BetCommand::handleToLowBet() {
    server->sendToClient(MessagePrinter::printBetTooLowMessage(3), senderName);
    return MoveInfo::NOT_ALLOWED;
}

MoveInfo BetCommand::handleToHighBet() {
    server->sendToClient(MessagePrinter::printBetTooHighMessage(), senderName);
    return MoveInfo::NOT_ALLOWED;
}

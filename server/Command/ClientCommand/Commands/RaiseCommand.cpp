//
// Created by micha on 23/06/2023.
//

#include "RaiseCommand.h"

RaiseCommand::RaiseCommand(
        ServerGameController *server,
        const ParsedMessage &parsedMessage,
        GameServiceCommandController *gameService
) : ClientCommand(server, parsedMessage, gameService) {
    deckMaster = gameService->getDeckMaster();
    game = gameService->getGame();
    senderName = parsedMessage.senderName;
    value = parsedMessage.value;
}

MoveInfo RaiseCommand::exactExecute() {

    deckMaster->evaluatePlayingPlayersCards(game->getPlayingPlayersRef());
    Player &player = game->currentPlayer();
    int bid = game->getBid();
    int credit = player.getCredit();

    if (bid == 0 || credit <= 2 * bid || game->isAllIn() || player.isExchange()) { return MoveInfo::NOT_ALLOWED; }

    if (value <= 2 * bid) { return handleToLowRaise(bid); }
    if (value > credit) { return handleToHighRaise(); }

    game->addToBank(value);
    player.removeCredit(value);
    player.setRaise(true);
    player.setTurn(false);

    std::for_each(
            game->getPlayingPlayersRef().begin(), game->getPlayingPlayersRef().end(),
            [this, &bid](Player &player) {
                if (player.isFold()) { return; }
                player.addToDiff(value - bid);
                player.setCheck(false);
                player.setBet(false);
                player.setRaise(false);
            }
    );

    game->setBid(value);

    return gameService->moveAccepted();
}

MoveInfo RaiseCommand::handleToLowRaise(int bid) {
    server->sendToClient(MessagePrinter::printRaiseTooLowMessage(2 * bid), senderName);
    return MoveInfo::NOT_ALLOWED;
}

MoveInfo RaiseCommand::handleToHighRaise() {
    server->sendToClient(MessagePrinter::printRaiseTooHighMessage(), senderName);
    return MoveInfo::NOT_ALLOWED;
}

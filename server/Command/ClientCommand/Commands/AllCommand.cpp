//
// Created by micha on 23/06/2023.
//

#include "AllCommand.h"

AllCommand::AllCommand(
        ServerGameController *server,
        const ParsedMessage &parsedMessage,
        GameServiceCommandController *gameService
) : ClientCommand(server, parsedMessage, gameService) {
    deckMaster = gameService->getDeckMaster();
    game = gameService->getGame();
}

MoveInfo AllCommand::exactExecute() {
    deckMaster->evaluatePlayingPlayersCards(game->getPlayingPlayersRef());
    Player &player = game->currentPlayer();
    int bid = game->getBid();
    if (player.getCredit() > bid || bid == 0 || player.isExchange()) { return MoveInfo::NOT_ALLOWED; }

    player.setTurn(false);
    game->addToBank(player.getCredit());
    player.setCredit(0);
    player.setDiff(0);
    game->setAllIn(true);

    return gameService->moveAccepted();
}
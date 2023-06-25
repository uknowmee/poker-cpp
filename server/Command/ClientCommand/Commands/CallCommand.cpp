//
// Created by micha on 23/06/2023.
//

#include "CallCommand.h"

CallCommand::CallCommand(
        ServerGameController *server,
        const ParsedMessage &parsedMessage,
        GameServiceCommandController *gameService
) : ClientCommand(server, parsedMessage, gameService) {
    deckMaster = gameService->getDeckMaster();
    game = gameService->getGame();
}

MoveInfo CallCommand::exactExecute() {
    deckMaster->evaluatePlayingPlayersCards(game->getPlayingPlayersRef());
    Player &player = game->currentPlayer();
    int bid = game->getBid();
    if (player.getCredit() <= bid || bid == 0 || player.isExchange()) { return MoveInfo::NOT_ALLOWED; }

    player.setTurn(false);
    player.removeCredit(player.getDiff());
    game->addToBank(player.getDiff());
    player.setDiff(0);

    return gameService->moveAccepted();
}

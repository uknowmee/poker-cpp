//
// Created by micha on 23/06/2023.
//

#include "CheckCommand.h"

CheckCommand::CheckCommand(
        ServerGameController *server,
        const ParsedMessage &parsedMessage,
        GameServiceCommandController *gameService
) : ClientCommand(server, parsedMessage, gameService) {
    deckMaster = gameService->getDeckMaster();
    game = gameService->getGame();
}

MoveInfo CheckCommand::exactExecute() {
    deckMaster->evaluatePlayingPlayersCards(game->getPlayingPlayersRef());
    int bid = game->getBid();
    Player &player = game->currentPlayer();
    if (bid != 0 || player.getCredit() <= 0 || player.isExchange()) { return MoveInfo::NOT_ALLOWED; }

    player.setCheck(true);
    player.setTurn(false);

    return gameService->moveAccepted();
}

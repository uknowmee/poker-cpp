//
// Created by micha on 23/06/2023.
//

#include "CyaCommand.h"

CyaCommand::CyaCommand(
        ServerGameController *server,
        const ParsedMessage &parsedMessage,
        GameServiceCommandController *gameService
) : ClientCommand(server, parsedMessage, gameService) {
    deckMaster = gameService->getDeckMaster();
    game = gameService->getGame();
}

MoveInfo CyaCommand::exactExecute() {
    deckMaster->evaluatePlayingPlayersCards(game->getPlayingPlayersRef());
    Player &player = game->currentPlayer();
    if (player.getCredit() >= 0 || player.isExchange()) { return MoveInfo::NOT_ALLOWED; }

    DeckMaster::collectCardsFromPlayer(player, game->getCardsRef());
    MoveInfo moveInfo = gameService->moveAccepted();
    server->disconnectClient(player.getName());
    return moveInfo;
}

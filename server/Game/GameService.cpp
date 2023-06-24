//
// Created by micha on 21/06/2023.
//

#include "GameService.h"

#include <unistd.h>


GameService GameService::createGameService(ServerGameController *serverGameController, int maxNumOfPlayers) {
    return GameService(maxNumOfPlayers, serverGameController);
}

GameService::GameService(int maxNumOfPlayers, ServerGameController *serverGameController)
        : serverGameController(serverGameController) {
    game = Game::createGame(maxNumOfPlayers);
    deckMaster = DeckMaster::createDeckMaster();
}

bool GameService::isGameStarted() {
    return game.isStarted();
}

void GameService::tryStartGame() {
    if (!isGameStarted()) {
        startGame();

        MessagePrinter::printStartGameMessage();

        serverGameController->broadcastMessage(MessagePrinter::gameStartMessage());
        sendInfoToAllPlayingPlayers();
    }
}

void GameService::startGame() {
    game.resetCards(DeckMaster::createFabricDeck());
    game.setStarted(true);
    game.addToBank(game.getMaxNumOfPlayers() * 2);
    removeCreditFromPlayers(game.getPlayersRef());
    game.firstPlayer().setTurn(true);
    game.setFirstPart();
    game.setPlayingPlayers(game.getPlayersCopy());
    game.setCurrent(game.firstPlayingPlayer());
    game.setNext(game.secondPlayingPlayer());
    DeckMaster::dealTheCards(game.getPlayingPlayersRef(), game.getCardsRef());
    deckMaster.evaluatePlayingPlayersCards(game.getPlayingPlayersRef());
}

void GameService::sendInfoToAllPlayingPlayers() {
    serverGameController->sendToClient(
            MessagePrinter::yourTurnMessage(game.currentPlayer().getName()),
            game.currentPlayer().getName()
    );

    std::vector<Player> playingPlayers = game.getPlayingPlayers();
    std::for_each(
            playingPlayers.begin(), playingPlayers.end(),
            [&](Player &player) {
                std::string info = player.toString();
                serverGameController->sendToClient(MessagePrinter::playerInfoMessage(info), player.getName());
            }
    );
}

void GameService::removeCreditFromPlayers(std::vector<Player> &players) {
    for (auto &player: players) {
        player.removeCredit(2);
    }
}

void GameService::gameResetBetweenRounds() {
    game.setBid(0);
    adjustWinnersBalance(game.getWinnersRef(), game.getBankValue());
    game.setBank(0);
    game.resetWinners();
    DeckMaster::collectCardsFromPlayingPlayers(game.getPlayingPlayersRef(), game.getCardsRef());
    DeckMaster::dealTheCards(game.getPlayingPlayersRef(), game.getCardsRef());
    game.addToBank(game.getNumOfPlayingPlayers());
    removeCreditFromPlayers(game.getPlayingPlayersRef());
    game.setLast(nullptr);
    game.setCurrent(game.firstPlayingPlayer());
    game.setNext(game.secondPlayingPlayer());
    deckMaster.evaluatePlayingPlayersCards(game.getPlayingPlayersRef());
}

void GameService::adjustWinnersBalance(std::vector<Player *> &players, int value) {
    for (auto &player: players) {
        player->addCredit(value);
    }
}

void GameService::addPlayer(const std::string &playerName) {
    game.addPlayer(Player::createPlayer(playerName));

    std::string message = MessagePrinter::printAddPlayerMessage(playerName);
    serverGameController->broadcastMessageExceptSender(message, playerName);
    serverGameController->sendToClient(MessagePrinter::welcomeMessage(playerName), playerName);
    usleep(50000);
    serverGameController->broadcastMessage(MessagePrinter::numberOfConnectedPlayersInfoMessage(game.getNumOfPlayers()));
}

void GameService::removeDisconnectedPlayer(const std::string &playerName) {
    if (!game.isInLobby(playerName)) { return; }
    if (!game.isStarted()) { return removePlayerIfNotStarted(playerName); }
    if (playerIsNotKicked(playerName)) { return resetGame(playerName); }
}

void GameService::removePlayerIfNotStarted(const std::string &playerName) {
    game.removePlayer(playerName);

    std::string message = MessagePrinter::printRemovePlayerMessage(playerName);
    serverGameController->broadcastMessageExceptSender(message, playerName);
    usleep(50000);
    serverGameController->broadcastMessage(MessagePrinter::numberOfConnectedPlayersInfoMessage(game.getNumOfPlayers()));
}

void GameService::resetGame(const std::string &playerName) {
    std::vector<Player> players = game.getPlayingPlayers();
    serverGameController->broadcastMessage(MessagePrinter::printGameEndedDueToDisconnectionMessage(playerName));

    for (Player &player: players) {
        MessagePrinter::printRemovePlayerMessage(player.getName());
        serverGameController->disconnectClient(player.getName());
    }

    game = Game::createGame(game.getMaxNumOfPlayers());
    deckMaster = DeckMaster::createDeckMaster();
}

bool GameService::playerIsNotKicked(const std::string &playerName) {
    std::vector<Player> players = game.getPlayingPlayers();
    Player player = std::find_if(
            players.begin(), players.end(),
            [&playerName](const Player &player) {
                return player.getName() == playerName;
            }
    ).operator*();
    return !player.isKicked();
}

//GameServiceCommandController
std::string GameService::playingPlayerInfo(const std::string &playerName) {
    return game.playingPlayer(playerName).toString();
}

bool GameService::isPlayerTurn(const std::string &senderName) {
    return game.currentPlayer().getName() == senderName;
}

int GameService::numOfPlayers() {
    return game.getNumOfPlayers();
}

std::string GameService::currentPlayerName() {
    return game.currentPlayer().getName();
}

void GameService::invokeFold(const std::string &senderName) {
    deckMaster.evaluatePlayingPlayersCards(game.getPlayingPlayersRef());
    Player &player = game.currentPlayer();

    if (player.getCredit() <= 0 || player.isExchange()) { return; }

    player.setFold(true);
    player.setTurn(false);
    DeckMaster::collectCardsFromPlayer(player, game.getCardsRef());

    moveAccepted();
}

void GameService::invokeCheck(const std::string &senderName) {
    deckMaster.evaluatePlayingPlayersCards(game.getPlayingPlayersRef());
    int bid = game.getBid();
    Player &player = game.currentPlayer();
    if (bid != 0 || player.getCredit() <= 0 || player.isExchange()) { return; }

    player.setCheck(true);
    player.setTurn(false);

    moveAccepted();
}

void GameService::invokeCall(const std::string &senderName) {
    deckMaster.evaluatePlayingPlayersCards(game.getPlayingPlayersRef());
    Player &player = game.currentPlayer();
    int bid = game.getBid();
    if (player.getCredit() <= bid || bid == 0 || player.isExchange()) { return; }

    player.setTurn(false);
    player.removeCredit(player.getDiff());
    game.addToBank(player.getDiff());
    player.setDiff(0);

    moveAccepted();
}

void GameService::invokeAll(const std::string &senderName) {
    deckMaster.evaluatePlayingPlayersCards(game.getPlayingPlayersRef());
    Player &player = game.currentPlayer();
    int bid = game.getBid();
    if (player.getCredit() > bid || bid == 0 || player.isExchange()) { return; }

    player.setTurn(false);
    game.addToBank(player.getCredit());
    player.setCredit(0);
    player.setDiff(0);
    game.setAllIn(true);

    moveAccepted();
}

void GameService::invokeCya(const std::string &senderName) {
    deckMaster.evaluatePlayingPlayersCards(game.getPlayingPlayersRef());
    Player &player = game.currentPlayer();
    if (player.getCredit() >= 0 || player.isExchange()) { return; }

    DeckMaster::collectCardsFromPlayer(player, game.getCardsRef());
    moveAccepted();
}

void GameService::invokeBet(const std::string &senderName, int value) {
    deckMaster.evaluatePlayingPlayersCards(game.getPlayingPlayersRef());
    Player &player = game.currentPlayer();
    int bid = game.getBid();
    int credit = player.getCredit();
    if (bid != 0 || credit <= 2 || game.isAllIn() || player.isExchange()) { return; }

    if (value <= 2) { return serverGameController->sendToClient(MessagePrinter::printBetTooLowMessage(3), senderName); }
    if (value > credit) {
        return serverGameController->sendToClient(MessagePrinter::printBetTooHighMessage(), senderName);
    }

    game.setBid(value);
    game.addToBank(value);
    player.removeCredit(value);
    player.setBet(true);
    player.setTurn(false);

    std::for_each(
            game.getPlayingPlayersRef().begin(), game.getPlayingPlayersRef().end(),
            [&value](Player &player) {
                if (player.isFold()) { return; }
                player.addToDiff(value);
                player.setCheck(false);
            }
    );

    moveAccepted();
}

void GameService::invokeRaise(const std::string &senderName, int value) {
    deckMaster.evaluatePlayingPlayersCards(game.getPlayingPlayersRef());
    Player &player = game.currentPlayer();
    int bid = game.getBid();
    int credit = player.getCredit();

    if (bid == 0 || credit <= 2 * bid || game.isAllIn() || player.isExchange()) { return; }

    if (value <= 2 * bid) {
        return serverGameController->sendToClient(MessagePrinter::printRaiseTooLowMessage(2 * bid), senderName);
    }
    if (value > credit) {
        return serverGameController->sendToClient(MessagePrinter::printRaiseTooHighMessage(), senderName);
    }

    game.addToBank(value);
    player.removeCredit(value);
    player.setRaise(true);
    player.setTurn(false);

    std::for_each(
            game.getPlayingPlayersRef().begin(), game.getPlayingPlayersRef().end(),
            [&value, &bid](Player &player) {
                if (player.isFold()) { return; }
                player.addToDiff(value - bid);
                player.setCheck(false);
                player.setBet(false);
                player.setRaise(false);
            }
    );

    game.setBid(value);
    moveAccepted();
}

void GameService::invokeExchange(const std::string &senderName, const std::vector<int> &values) {
    Player &player = game.currentPlayer();
    if (!player.isExchange()) { return; }

    DeckMaster::collectPlayerCards(game.currentPlayer(), game.getCardsRef(), values);
    player.setExchange(false);
    updateQueue();
    serverGameController->sendToClient(MessagePrinter::printExchangeAccepted(), senderName);

    if (!player.isExchange()) {
        DeckMaster::dealTheCards(game.getPlayingPlayersRef(), game.getCardsRef());
        game.adjustPart();
        game.setBid(0);
        deckMaster.evaluatePlayingPlayersCards(game.getPlayingPlayersRef());

        serverGameController->broadcastMessage(MessagePrinter::printExchangeFinishedMessage());
        sendInfoToAllPlayingPlayers();
    }
}

void GameService::moveAccepted() {
    updateQueue();
    updateIfFirstPlayerReadyForExchange();
    updateIfSecondPartFinished();
}

void GameService::makeWinners() {
    int maxPoints = 0;
    int maxType = 0;
    std::vector<Player> &playingPlayers = game.getPlayingPlayersRef();
    std::vector<Player *> &winners = game.getWinnersRef();

    for (auto &player: playingPlayers) {
        if (!player.isFold()) { continue; }

        if (player.getPoints() > maxPoints) {
            maxPoints = player.getPoints();
            maxType = player.getHandType();
            winners.clear();
            winners.push_back(&player);
        } else if (player.getPoints() == maxPoints && player.getHandType() > maxType) {
            maxType = player.getHandType();
            winners.clear();
            winners.push_back(&player);
        } else if (player.getPoints() == maxPoints && player.getHandType() == maxType) {
            winners.push_back(&player);
        }
    }
}

void GameService::updateQueue() {
    Player &player = game.currentPlayer();
    game.removeFirstFromPlayingPlayers();
    game.setLastPlayer(player);
    if (player.getCredit() >= 0) { game.addToPlayingPlayers(player); }

    std::vector<Player> &playingPlayers = game.getPlayingPlayersRef();
    while (playingPlayers.front().isFold()) {
        playingPlayers.push_back(playingPlayers.front());
        playingPlayers.erase(playingPlayers.begin());
    }
    game.setCurrent(playingPlayers.front());
    game.currentPlayer().setTurn(true);
    playingPlayers.size() == 1 ? game.setNext(playingPlayers.front()) : game.setNext(playingPlayers[1]);
}

void GameService::updateIfFirstPlayerReadyForExchange() {
    Player &player = game.currentPlayer();
    if (!(player.getBet() || player.getRaise() || player.getCheck()) || game.getPart() % 2 != 1) { return; }

    std::vector<Player> &playingPlayers = game.getPlayingPlayersRef();
    std::for_each(
            playingPlayers.begin(), playingPlayers.end(),
            [](Player &player) {
                if (player.isFold()) { return; }
                player.setCheck(false);
                player.setBet(false);
                player.setRaise(false);
                player.setExchange(true);
            }
    );
}

void GameService::updateIfSecondPartFinished() {
    Player &player = game.currentPlayer();

    if (!(player.getBet() || player.getRaise() || player.getCheck() || game.isAllIn())
        || game.getPart() % 2 != 0) {
        return;
    }

    std::vector<Player> &playingPlayers = game.getPlayingPlayersRef();
    std::for_each(
            playingPlayers.begin(), playingPlayers.end(),
            [](Player &player) {
                if (player.isFold()) { return; }
                player.setCheck(false);
                player.setBet(false);
                player.setRaise(false);
            }
    );
    makeWinners();
    finishGameOrRound();
}

void GameService::finishGameOrRound() {
    // todo implement
    int a = 2;
}

std::string GameService::toString() const {
    return "Game{\n" +
           MessagePrinter::addIndentation(game.toString(), "\t") +
           "\n}";
}

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
    game.firstPlayer().setTurn(true);
    game.setFirstPart();
    game.setPlayingPlayers(game.getPlayersCopy());
    removeCreditFromPlayers(game.getPlayingPlayersRef());
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

    std::deque<Player> playingPlayers = game.getPlayingPlayers();
    std::for_each(
            playingPlayers.begin(), playingPlayers.end(),
            [&](Player &player) {
                std::string info = player.toString();
                serverGameController->sendToClient(MessagePrinter::playerInfoMessage(info), player.getName());
            }
    );
}

void GameService::removeCreditFromPlayers(std::deque<Player> &players) {
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
    std::deque<Player> players = game.getPlayingPlayers();
    serverGameController->broadcastMessage(MessagePrinter::printGameEndedDueToDisconnectionMessage(playerName));

    for (Player &player: players) {
        MessagePrinter::printRemovePlayerMessage(player.getName());
        serverGameController->disconnectClient(player.getName());
    }

    game = Game::createGame(game.getMaxNumOfPlayers());
    deckMaster = DeckMaster::createDeckMaster();
}

bool GameService::playerIsNotKicked(const std::string &playerName) {
    std::deque<Player> players = game.getPlayingPlayers();
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

std::string GameService::lastPlayerName() {
    return game.lastPlayer().getName();
}

MoveInfo GameService::invokeFold(const std::string &senderName) {
    deckMaster.evaluatePlayingPlayersCards(game.getPlayingPlayersRef());
    Player &player = game.currentPlayer();

    if (player.getCredit() <= 0 || player.isExchange()) { return MoveInfo::NOT_ALLOWED; }

    player.setFold(true);
    player.setTurn(false);
    DeckMaster::collectCardsFromPlayer(player, game.getCardsRef());

    return moveAccepted();
}

MoveInfo GameService::invokeCheck(const std::string &senderName) {
    deckMaster.evaluatePlayingPlayersCards(game.getPlayingPlayersRef());
    int bid = game.getBid();
    Player &player = game.currentPlayer();
    if (bid != 0 || player.getCredit() <= 0 || player.isExchange()) { return MoveInfo::NOT_ALLOWED; }

    player.setCheck(true);
    player.setTurn(false);

    return moveAccepted();
}

MoveInfo GameService::invokeCall(const std::string &senderName) {
    deckMaster.evaluatePlayingPlayersCards(game.getPlayingPlayersRef());
    Player &player = game.currentPlayer();
    int bid = game.getBid();
    if (player.getCredit() <= bid || bid == 0 || player.isExchange()) { return MoveInfo::NOT_ALLOWED; }

    player.setTurn(false);
    player.removeCredit(player.getDiff());
    game.addToBank(player.getDiff());
    player.setDiff(0);

    return moveAccepted();
}

MoveInfo GameService::invokeAll(const std::string &senderName) {
    deckMaster.evaluatePlayingPlayersCards(game.getPlayingPlayersRef());
    Player &player = game.currentPlayer();
    int bid = game.getBid();
    if (player.getCredit() > bid || bid == 0 || player.isExchange()) { return MoveInfo::NOT_ALLOWED; }

    player.setTurn(false);
    game.addToBank(player.getCredit());
    player.setCredit(0);
    player.setDiff(0);
    game.setAllIn(true);

    return moveAccepted();
}

MoveInfo GameService::invokeCya(const std::string &senderName) {
    deckMaster.evaluatePlayingPlayersCards(game.getPlayingPlayersRef());
    Player &player = game.currentPlayer();
    if (player.getCredit() >= 0 || player.isExchange()) { return MoveInfo::NOT_ALLOWED; }

    DeckMaster::collectCardsFromPlayer(player, game.getCardsRef());

    return moveAccepted();
}

MoveInfo GameService::invokeBet(const std::string &senderName, int value) {
    deckMaster.evaluatePlayingPlayersCards(game.getPlayingPlayersRef());
    Player &player = game.currentPlayer();
    int bid = game.getBid();
    int credit = player.getCredit();
    if (bid != 0 || credit <= 2 || game.isAllIn() || player.isExchange()) { return MoveInfo::NOT_ALLOWED; }

    if (value <= 2) {
        serverGameController->sendToClient(MessagePrinter::printBetTooLowMessage(3), senderName);
        return MoveInfo::NOT_ALLOWED;
    }
    if (value > credit) {
        serverGameController->sendToClient(MessagePrinter::printBetTooHighMessage(), senderName);
        return MoveInfo::NOT_ALLOWED;
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

    return moveAccepted();
}

MoveInfo GameService::invokeRaise(const std::string &senderName, int value) {
    deckMaster.evaluatePlayingPlayersCards(game.getPlayingPlayersRef());
    Player &player = game.currentPlayer();
    int bid = game.getBid();
    int credit = player.getCredit();

    if (bid == 0 || credit <= 2 * bid || game.isAllIn() || player.isExchange()) { return MoveInfo::NOT_ALLOWED; }

    if (value <= 2 * bid) {
        serverGameController->sendToClient(MessagePrinter::printRaiseTooLowMessage(2 * bid), senderName);
        return MoveInfo::NOT_ALLOWED;
    }
    if (value > credit) {
        serverGameController->sendToClient(MessagePrinter::printRaiseTooHighMessage(), senderName);
        return MoveInfo::NOT_ALLOWED;
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

    return moveAccepted();
}

void GameService::invokeExchange(const std::string &senderName, const std::vector<int> &values) {
    if (!game.currentPlayer().isExchange()) { return; }

    game.currentPlayer().setExchange(false);
    updateQueue();
    DeckMaster::collectPlayerCards(game.lastPlayer(), game.getCardsRef(), values);
    serverGameController->sendToClient(MessagePrinter::printExchangeAccepted(), senderName);

    if (!game.currentPlayer().isExchange()) {
        DeckMaster::dealTheCards(game.getPlayingPlayersRef(), game.getCardsRef());
        game.adjustPart();
        game.setBid(0);
        deckMaster.evaluatePlayingPlayersCards(game.getPlayingPlayersRef());

        serverGameController->broadcastMessage(MessagePrinter::printExchangeFinishedMessage());
        sendInfoToAllPlayingPlayers();
    }
}

MoveInfo GameService::moveAccepted() {
    updateQueue();
    updateIfFirstPlayerReadyForExchange();

    if (updateIfSecondPartFinished()) {
        makeWinners();
        return eitherGameOrRoundFinished();
    }

    return MoveInfo::ACCEPTED;
}

void GameService::makeWinners() {
    int maxPoints = 0;
    int maxType = 0;
    std::deque<Player> &playingPlayers = game.getPlayingPlayersRef();
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
    Player playerCopy = Player(game.currentPlayer());

    game.removeFirstFromPlayingPlayers();
    if (playerCopy.getCredit() >= 0) { game.addToPlayingPlayers(playerCopy); }
    game.setLastPlayer(game.getPlayingPlayersRef().back());

    std::deque<Player> &playingPlayers = game.getPlayingPlayersRef();
    while (playingPlayers.front().isFold()) {
        playingPlayers.push_back(playingPlayers.front());
        playingPlayers.pop_front();
    }
    game.setCurrent(playingPlayers.front());
    game.currentPlayer().setTurn(true);
    playingPlayers.size() == 1 ? game.setNext(playingPlayers.front()) : game.setNext(playingPlayers[1]);
}

void GameService::updateIfFirstPlayerReadyForExchange() {
    Player &player = game.currentPlayer();
    if (!(player.getBet() || player.getRaise() || player.getCheck()) || game.getPart() % 2 != 1) { return; }

    std::deque<Player> &playingPlayers = game.getPlayingPlayersRef();
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

    serverGameController->broadcastMessage(MessagePrinter::printExchangeStartedMessage());
}

bool GameService::updateIfSecondPartFinished() {
    Player &player = game.currentPlayer();

    if (!(player.getBet() || player.getRaise() || player.getCheck() || game.isAllIn())
        || game.getPart() % 2 != 0) {
        return false;
    }

    std::deque<Player> &playingPlayers = game.getPlayingPlayersRef();
    std::for_each(
            playingPlayers.begin(), playingPlayers.end(),
            [](Player &player) {
                if (player.isFold()) { return; }
                player.setCheck(false);
                player.setBet(false);
                player.setRaise(false);
            }
    );

    return true;
}

MoveInfo GameService::eitherGameOrRoundFinished() {
    // todo implement
    int a = 2;
}

std::string GameService::toString() const {
    return "Game{\n" +
           MessagePrinter::addIndentation(game.toString(), "\t") +
           "\n}";
}

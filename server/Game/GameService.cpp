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

void GameService::removeCreditFromPlayers(std::deque<Player> &playingPlayers) {
    for (auto &player: playingPlayers) {
        removeCreditFromPlayerAndPlayingPlayer(player, 2);
    }
}

void GameService::removeCreditFromPlayerAndPlayingPlayer(Player &player, int value) {
    player.removeCredit(value);
    game.getPlayer(player.getName()).removeCredit(value);
}

void GameService::gameResetBetweenRounds() {
    game.setBid(0);
    adjustWinnersBalance(game.getWinnersRef(), game.getBankValue());
    game.setBank(0);
    game.resetWinners();
    DeckMaster::collectCardsFromPlayingPlayers(game.getPlayingPlayersRef(), game.getCardsRef());
    DeckMaster::dealTheCards(game.getPlayingPlayersRef(), game.getCardsRef());
    game.addToBank(game.getNumOfPlayingPlayers() * 2);
    removeCreditFromPlayers(game.getPlayingPlayersRef());
    game.setLast(nullptr);
    game.setCurrent(game.firstPlayingPlayer());
    game.setNext(game.secondPlayingPlayer());
    deckMaster.evaluatePlayingPlayersCards(game.getPlayingPlayersRef());
}

void GameService::adjustWinnersBalance(std::vector<Player *> &winnersInPlayingPlayers, int value) {
    for (auto &player: winnersInPlayingPlayers) {
        player->addCredit(value);
        game.getPlayer(player->getName()).addCredit(value);
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
    std::vector<Player> players = game.getPlayersCopy();
    Player player = std::find_if(
            players.begin(), players.end(),
            [&playerName](const Player &player) {
                return player.getName() == playerName;
            }
    ).operator*();
    return !player.isKicked();
}

//GameServiceCommandController
void GameService::finishRound() {
    // todo info print
    gameResetBetweenRounds();
}

void GameService::finishGame() {
    // todo info print + disconnect
    game = Game::createGame(game.getMaxNumOfPlayers());
    deckMaster = DeckMaster::createDeckMaster();
}

bool GameService::isPlayerTurn(const std::string &senderName) {
    return game.currentPlayer().getName() == senderName;
}

DeckMaster *GameService::getDeckMaster() {
    return &deckMaster;
}

Game *GameService::getGame() {
    return &game;
}

std::string GameService::currentPlayerName() {
    return game.currentPlayer().getName();
}

std::string GameService::lastPlayerName() {
    return game.lastPlayer().getName();
}

int GameService::numOfPlayers() {
    return game.getNumOfPlayers();
}

std::string GameService::playingPlayerInfo(const std::string &playerName) {
    return game.playingPlayer(playerName).toString();
}

MoveInfo GameService::moveAccepted() {
    updateQueue();
    if (updateIfFirstPlayerReadyForExchange()) { return MoveInfo::EXCHANGE_STARTED; }

    if (updateIfSecondPartFinished()) {
        makeWinners();
        return eitherGameOrRoundFinished();
    }

    return MoveInfo::ACCEPTED;
}

void GameService::updateQueue() {
    Player playerCopy = Player(game.currentPlayer());

    game.removeFirstFromPlayingPlayers();

    if (playerCopy.getCredit() >= 0) { game.addToPlayingPlayers(playerCopy); }
    else { game.setPlayerKicked(playerCopy.getName()); }

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

bool GameService::updateIfFirstPlayerReadyForExchange() {
    Player &player = game.currentPlayer();
    if (!(player.getBet() || player.getRaise() || player.getCheck()) || game.getPart() % 2 != 1) { return false; }

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

    return true;
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

void GameService::makeWinners() {
    int maxPoints = 0;
    int maxType = 0;
    std::deque<Player> &playingPlayers = game.getPlayingPlayersRef();
    std::vector<Player *> &winners = game.getWinnersRef();

    for (auto &player: playingPlayers) {
        if (player.isFold()) { continue; }

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

MoveInfo GameService::eitherGameOrRoundFinished() {
    return game.getPlayingPlayers().size() == 1
           ? MoveInfo::GAME_FINISHED
           : MoveInfo::ROUND_FINISHED;
}

std::string GameService::toString() const {
    return "Game{\n" +
           MessagePrinter::addIndentation(game.toString(), "\t") +
           "\n}";
}


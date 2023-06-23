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

void GameService::addPlayer(const std::string& playerName) {
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

void GameService::resetGame(const std::string& playerName) {
    std::vector <Player> players = game.getPlayingPlayers();
    serverGameController->broadcastMessage(MessagePrinter::printGameEndedDueToDisconnectionMessage(playerName));

    for (Player &player: players) {
        MessagePrinter::printRemovePlayerMessage(player.getName());
        serverGameController->disconnectClient(player.getName());
    }

    game = Game::createGame(game.getMaxNumOfPlayers());
    deckMaster = DeckMaster::createDeckMaster();
}

bool GameService::isSenderNotCurrent(const std::string &senderName) {
    return game.currentPlayer().getName() != senderName;
}

void GameService::notCurrent(const std::string &senderName) {
    serverGameController->sendToClient(MessagePrinter::notYourTurnMessage(senderName), senderName);
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
std::string GameService::playingPlayerInfo(const std::string &playerName){
    return game.playingPlayer(playerName).toString();
}

void GameService::invokeFold(const std::string &senderName) {
    // todo implement
}

void GameService::invokeCheck(const std::string &senderName) {
    // todo implement
}

void GameService::invokeCall(const std::string &senderName) {
    // todo implement
}

void GameService::invokeAll(const std::string &senderName) {
    // todo implement
}

void GameService::invokeCya(const std::string &senderName) {
    // todo implement
}

void GameService::invokeBet(const std::string &senderName, int value) {
    // todo implement
}

void GameService::invokeRaise(const std::string &senderName, int value) {
    // todo implement
}

void GameService::invokeExchange(const std::string &senderName, const std::vector<int> &values) {
    // todo implement
}

void GameService::moveAccepted(){
    updateQueue();
    updateIfFirstPlayerReadyForExchange();
    updateIfSecondPartFinished();
}

void GameService::proceedExchange(){
    // todo implement
}

void GameService::winnersCheck(){
    // todo implement finish round / game if needed
}

void GameService::updateQueue() {
// todo implement
}

void GameService::updateIfFirstPlayerReadyForExchange() {
// todo implement
}

void GameService::updateIfSecondPartFinished() {
// todo implement
}

std::string GameService::toString() const {
    return "Game{\n" +
            MessagePrinter::addIndentation(game.toString(), "\t") +
           "\n}";
}

void GameService::notStarted(const std::string &senderName) {
    serverGameController->sendToClient(MessagePrinter::gameNotStartedMessage(), senderName);
}

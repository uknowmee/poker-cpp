//
// Created by micha on 21/06/2023.
//

#include "GameService.h"
#include "../Server/Server.h"

GameService::GameService(Game game, ServerGameController *serverGameController)
        : game(game), serverGameController(serverGameController) {
}

GameService GameService::createGameService(ServerGameController *serverGameController) {
    return GameService(Game::createGame(), serverGameController);
}

bool GameService::isGameStarted() {
    return game.isStarted();
}

void GameService::startGame() {
    game.start();
}

void GameService::addPlayer(const std::string &playerName) {
    game.addPlayer(playerName);
}

void GameService::removeDisconnectedPlayer(const std::string &playerName) {
    game.removePlayer(playerName);
}

void GameService::invokeCommand(const std::string &command, const std::string &senderName) {

}

void GameService::invokeCommand(const std::string &command, int value, const std::string &senderName) {

}

void GameService::invokeCommand(const std::string &command, const std::vector<int>& values, const std::string &senderName) {

}
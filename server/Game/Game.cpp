//
// Created by micha on 20/06/2023.
//

#include "Game.h"

Game Game::createGame() {
    return {};
}

bool Game::isStarted() const {
    return started;
}

void Game::start() {
    started = true;
}

void Game::addPlayer(const std::basic_string<char> &playerName) {
    // todo add player to game
}

void Game::removePlayer(const std::string &playerName) {
    // todo remove player from game
}

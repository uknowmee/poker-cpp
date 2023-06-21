//
// Created by micha on 20/06/2023.
//

#include "Game.h"

Game Game::createGame() {
    return {};
}

Game::Game() {
    this->started = false;
}

bool Game::isStarted() const {
    return started;
}

void Game::start() {
    started = true;
}

void Game::removePlayer(const std::string &playerName) {

}

void Game::addPlayer(const std::string &playerName) {

}

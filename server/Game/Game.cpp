//
// Created by micha on 20/06/2023.
//

#include <numeric>
#include "Game.h"

Game Game::createGame(int maxNumOfPlayers) {
    return Game(maxNumOfPlayers);
}

Game::Game(int maxNumOfPlayers)
        : maxNumOfPlayers(maxNumOfPlayers), cards({}), players({}), playingPlayers({}), winners({}),
          last(nullptr), current(nullptr), next(nullptr), started(false),
          allIn(false), bid(0), bank(0), part{0} {}

Game::Game()
        : Game(0) {
}

bool Game::isStarted() const {
    return started;
}

void Game::removePlayer(const std::string &playerName) {
    players.erase(std::remove_if(
                          players.begin(), players.end(),
                          [&playerName](const Player &player) { return player.getName() == playerName; }
                  ),
                  players.end()
    );
}

void Game::addPlayer(const Player &player) {
    players.push_back(player);
}

void Game::setStarted(bool toSet) {
    started = toSet;
}

int Game::getMaxNumOfPlayers() const {
    return maxNumOfPlayers;
}

void Game::addToBank(int toAdd) {
    bank += toAdd;
}

std::vector<Player> &Game::getPlayersRef() {
    return players;
}

Player &Game::firstPlayer() {
    return players[0];
}

void Game::setFirstPart() {
    part = 1;
}

std::vector<Player> Game::getPlayersCopy() {
    return players;
}

void Game::setPlayingPlayers(std::vector<Player> toSet) {
    playingPlayers = std::move(toSet);
}

Player &Game::firstPlayingPlayer() {
    return playingPlayers[0];
}

Player &Game::secondPlayingPlayer() {
    return playingPlayers[1];
}

void Game::setCurrent(Player &player) {
    current = &player;
}

void Game::setNext(Player &player) {
    next = &player;
}

std::vector<Card> &Game::getCardsRef() {
    return cards;
}

std::vector<Player> &Game::getPlayingPlayersRef() {
    return playingPlayers;
}

void Game::setBid(int toSet) {
    bid = toSet;
}

int Game::getBankValue() const {
    return bank;
}

std::vector<Player *> &Game::getWinnersRef() {
    return winners;
}

void Game::setBank(int toSet) {
    bank = toSet;
}

void Game::resetWinners() {
    winners = {};
}

int Game::getNumOfPlayingPlayers() {
    return (int) playingPlayers.size();
}

void Game::setLast(Player *player) {
    last = player;
}

std::vector<Player> Game::getPlayingPlayers() {
    return playingPlayers;
}

Player &Game::currentPlayer() {
    return *current;
}

void Game::resetCards(std::vector<Card> cardsToSet) {
    cards = std::move(cardsToSet);
}

const Player &Game::playingPlayer(const std::string &playerName) {
    return *std::find_if(
            playingPlayers.begin(), playingPlayers.end(),
            [&playerName](const Player &player) { return player.getName() == playerName; }
    );
}

std::string Game::toString() const {
    std::string cardList;
    if (!cards.empty()) {
        cardList = "\ncards in deck=";
        cardList += std::accumulate(cards.begin(), cards.end(), std::string(),
                                    [](const std::string &acc, const Card &card) {
                                        return acc + (acc.empty() ? "" : ", ") + card.toString();
                                    });
    }

    return "maxNumOfPlayers=" + std::to_string(maxNumOfPlayers) +
           cardList +
           (!players.empty() ? "\nplayers=" + getPlayersNames() : "") +
           (!playingPlayers.empty() ? "\nplayingPlayers=\n" + getPlayingPlayerNamesAndCards() : "") +
           (!winners.empty() ? "\nwinners=" + getWinnersNames() : "") +
           (last != nullptr ? "\nlast=" + last->getName() : "") +
           (current != nullptr ? "\ncurrent=" + current->getName() : "") +
           (next != nullptr ? "\nnext=" + next->getName() : "") +
           "\nstarted=" + (started ? "true" : "false") +
           "\nallIn=" + (allIn ? "true" : "false") +
           "\nbid=" + std::to_string(bid) +
           "\nbank=" + std::to_string(bank) +
           "\npart=" + std::to_string(part);
}

std::string Game::getWinnersNames() const {
    std::string winnersNames;
    for (const auto &winner: winners) {
        winnersNames += winner->getName() + ", ";
    }
    return winnersNames;
}

std::string Game::getPlayersNames() const {
    std::string playersNames;
    for (const auto &player: players) {
        playersNames += player.getName() + ", ";
    }
    return playersNames;
}

std::string Game::getPlayingPlayerNamesAndCards() const {
    std::string playingPlayersNamesAndCards;
    for (Player player: playingPlayers) {
        std::vector<Card> playerCards = player.cards();
        playingPlayersNamesAndCards += "\t" + player.getName() + ", cards=";
        for (int i = 0; i < playerCards.size(); ++i) {
            playingPlayersNamesAndCards += playerCards[i].toString();
            if (i != playerCards.size() - 1) { playingPlayersNamesAndCards += ", "; }
        }
        if (player.getName() != playingPlayers.back().getName()) { playingPlayersNamesAndCards += "\n"; }
    }
    return playingPlayersNamesAndCards;
}

bool Game::isInLobby(const std::string &playerName) {
    return std::find_if(
            players.begin(), players.end(),
            [&playerName](const Player &player) { return player.getName() == playerName; }
    ) != players.end();
}

int Game::getNumOfPlayers() {
    return (int) players.size();
}


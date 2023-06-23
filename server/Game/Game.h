//
// Created by micha on 20/06/2023.
//

#ifndef MY_PROJECTS_GAME_H
#define MY_PROJECTS_GAME_H


#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include "Player.h"

class Game {

private:
    int maxNumOfPlayers;

    std::vector<Card> cards;
    std::vector<Player> players;
    std::vector<Player> playingPlayers;
    std::vector<Player*> winners;
    Player *last;
    Player *current;
    Player *next;
    bool started;
    bool allIn;
    int bid;
    int bank;
    int part;

    explicit Game(int maxNumOfPlayers);
    std::string getWinnersNames() const;
    std::string getPlayersNames() const;
    std::string getPlayingPlayerNamesAndCards() const;

public:
    Game();
    static Game createGame(int maxNumOfPlayers);
    void addPlayer(const Player& player);
    void removePlayer(const std::string &playerName);
    void addToBank(int toAdd);
    void resetCards(std::vector<Card> cardsToSet);

    std::vector<Player> &getPlayersRef();
    std::vector<Player> getPlayersCopy();
    std::vector<Player> &getPlayingPlayersRef();
    std::vector<Player> getPlayingPlayers();
    std::vector<Player *> &getWinnersRef();
    const Player &playingPlayer(const std::string &playerName);
    Player &firstPlayer();
    Player &firstPlayingPlayer();
    Player &secondPlayingPlayer();
    Player &currentPlayer();
    std::vector<Card> &getCardsRef();
    int getMaxNumOfPlayers() const;
    int getBankValue() const;
    int getNumOfPlayingPlayers();
    int getNumOfPlayers();
    bool isStarted() const;
    bool isInLobby(const std::string &playerName);

    std::string toString() const;
    void setStarted(bool toSet);
    void setFirstPart();
    void setPlayingPlayers(std::vector<Player> players);
    void setCurrent(Player &player);
    void setNext(Player &player);
    void setBid(int toSet);
    void setBank(int toSet);
    void resetWinners();
    void setLast(Player *player);
};


#endif //MY_PROJECTS_GAME_H

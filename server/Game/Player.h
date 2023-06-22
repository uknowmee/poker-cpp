//
// Created by micha on 20/06/2023.
//

#ifndef MY_PROJECTS_PLAYER_H
#define MY_PROJECTS_PLAYER_H


#include <string>
#include <utility>
#include <vector>
#include "../CustomDeck/Card.h"

class Player {

private:
    std::vector<Card> playerCards;
    std::string playerName;
    bool fold;
    bool check;
    bool bet;
    bool raise;
    bool exchange;
    bool turn;
    bool kicked;
    int credit;
    int diff;
    int points;
    Rank type;

    explicit Player(std::string playerName);

public:
    static Player createPlayer(const std::string& playerName);
    void removeCredit(int toRemove);
    void addCredit(int toAdd);
    void addCard(Card &card);
    void setTurn(bool toSet);
    std::vector<Card> &playerCardsRef();
    std::vector<Card> cards();
    std::string getName() const;
    std::string toString() const;
    bool isKicked() const;
    void setPoints(int pointsToSet);
    void setType(Rank rank);
};


#endif //MY_PROJECTS_PLAYER_H

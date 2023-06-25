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
    static Player createPlayer(std::string playerName);
    Player(const Player &other);
    void removeCredit(int toRemove);
    void addCredit(int toAdd);
    void addToDiff(int toAdd);
    void addCard(Card &card);

    std::vector<Card> &playerCardsRef();
    std::vector<Card> cards();
    std::string getName() const;
    std::string toString() const;
    bool isKicked() const;
    bool isExchange() const;
    bool isFold() const;
    bool getBet() const;
    bool getRaise() const;
    bool getCheck() const;
    int getCredit() const;
    int getPoints() const;
    int getHandType();
    int getDiff() const;

    void setPoints(int pointsToSet);
    void setType(Rank rank);
    void setRaise(bool toSet);
    void setBet(bool toSet);
    void setCheck(bool toSet);
    void setExchange(bool toSet);
    void setTurn(bool toSet);
    void setDiff(int toSet);
    void setFold(bool toSet);
    void setKicked(bool toSet);
};


#endif //MY_PROJECTS_PLAYER_H

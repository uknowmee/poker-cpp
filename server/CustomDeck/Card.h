//
// Created by micha on 20/06/2023.
//

#ifndef MY_PROJECTS_CARD_H
#define MY_PROJECTS_CARD_H


#include <string>
#include "CardMapper.h"
#include "Rank.h"
#include "Suit.h"

class Card {

private:
    Rank rank;
    Suit suit;
    int num;

public:
    Rank getRank() const;
    Suit getSuit() const;
    std::string toString() const;
    int getNumber() const;
    Card(const Card &other);
    Card(Rank rankToSet, Suit suitToSet, int num);
};


#endif //MY_PROJECTS_CARD_H

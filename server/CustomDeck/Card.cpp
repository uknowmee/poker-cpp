//
// Created by micha on 20/06/2023.
//

#include "Card.h"

Rank Card::getRank() const {
    return rank;
}

Suit Card::getSuit() const {
    return suit;
}

std::string Card::toString() const {
    return CardMapper::rankMap[rank] + " " + CardMapper::suitMap[suit];
}

Card::Card(Rank rankToSet, Suit suitToSet, int num)
        : rank(rankToSet), suit(suitToSet), num(num) {

}

int Card::getNumber() const {
    return num;
}

Card::Card(const Card &other) {
    rank = other.rank;
    suit = other.suit;
    num = other.num;
}

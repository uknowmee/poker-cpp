//
// Created by micha on 20/06/2023.
//

#include "Player.h"

Player Player::createPlayer(const std::string& playerName) {
    return Player(playerName);
}

Player::Player(std::string playerName)
        : playerCards({}), playerName(std::move(playerName)),
          fold(false), check(false), bet(false), raise(false), exchange(false), turn(false), kicked(false),
          credit(100), diff(0), points(0), type{} {}

bool Player::isKicked() const {
    return kicked;
}

std::string Player::toString() const {
    std::string result = "Player: " + playerName + "\n";
    result += "Credit: " + std::to_string(credit) + "\n";
    result += "Points: " + std::to_string(points) + " - " + CardMapper::pointsMap[points] + "\n";
    result += "Type: " + CardMapper::rankMap[type] + "\n";
    result += "Cards: ";
    for (int i = 0; i < playerCards.size(); ++i) {
        result += playerCards[i].toString();
        if (i != playerCards.size() - 1) { result += ", "; }
    }
    return result;
}

void Player::removeCredit(int toRemove) {
    credit -= toRemove;
}

void Player::setTurn(bool toSet) {
    turn = toSet;
}

void Player::addCredit(int toAdd) {
    credit += toAdd;
}

std::string Player::getName() const {
    return playerName;
}

void Player::addCard(Card &card) {
    playerCards.push_back(card);
}

std::vector<Card> &Player::playerCardsRef() {
    return playerCards;
}

std::vector<Card> Player::cards() {
    return playerCards;
}

void Player::setPoints(int pointsToSet) {
    points = pointsToSet;
}

void Player::setType(Rank rank) {
    type = rank;
}

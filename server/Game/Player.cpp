//
// Created by micha on 20/06/2023.
//

#include "Player.h"

#include <utility>

Player Player::createPlayer(std::string playerName) {
    return Player(std::move(playerName));
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
    result += (turn ? "Turn: true\n" : "Turn: false\n");
    result += "Credit: " + std::to_string(credit) + "\n";
    result += "Diff: " + std::to_string(diff) + "\n";
    result += "Points: " + std::to_string(points) + " - " + CardMapper::pointsMap[points] + "\n";
    result += "Type: " + CardMapper::rankMap[type] + "\n";
    result += "Cards: ";
    for (int i = 0; i < playerCards.size(); ++i) {
        result += playerCards[i].toString();
        if (i != playerCards.size() - 1) { result += ", "; }
        else { result += "\n"; }
    }
    result += (fold ? "Fold: true, " : "Fold: false, ");
    result += (check ? "Check: true, " : "Check: false, ");
    result += (bet ? "Bet: true, " : "Bet: false, ");
    result += (raise ? "Raise: true, " : "Raise: false, ");
    result += (exchange ? "Exchange: true, " : "Exchange: false, ");
    result += (kicked ? "Kicked: true, " : "Kicked: false, ");
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

bool Player::isExchange() const {
    return exchange;
}

void Player::setRaise(bool toSet) {
    raise = toSet;
}

void Player::setBet(bool toSet) {
    bet = toSet;
}

void Player::setCheck(bool toSet) {
    check = toSet;
}

bool Player::isFold() const {
    return fold;
}

bool Player::getBet() const {
    return bet;
}

bool Player::getRaise() const {
    return raise;
}

bool Player::getCheck() const {
    return check;
}

void Player::setExchange(bool toSet) {
    exchange = toSet;
}

int Player::getCredit() const {
    return credit;
}

int Player::getPoints() const {
    return points;
}

int Player::getHandType() {
    return type;
}

void Player::addToDiff(int toAdd) {
    diff += toAdd;
}

void Player::setDiff(int toSet) {
    diff = toSet;
}

int Player::getDiff() const {
    return diff;
}

void Player::setFold(bool toSet) {
    fold = toSet;
}

Player::Player(const Player &other) {
    playerName = other.playerName;
    fold = other.fold;
    check = other.check;
    bet = other.bet;
    raise = other.raise;
    exchange = other.exchange;
    turn = other.turn;
    kicked = other.kicked;
    credit = other.credit;
    diff = other.diff;
    points = other.points;
    type = other.type;

    // Create a deep copy of the playerCards vector
    playerCards.reserve(other.playerCards.size());
    for (const Card &card: other.playerCards) {
        playerCards.emplace_back(card);
    }
}

void Player::setKicked(bool toSet) {
    kicked = toSet;
}

//
// Created by micha on 20/06/2023.
//

#include "Evaluator.h"

#include <utility>

Evaluator Evaluator::createEvaluator() {
    return Evaluator();
}

Evaluator::Evaluator()
        : points(0), type{}, cards({}), color(0),
          diffRanks(0), maxOfOneRank(0) {
}

void Evaluator::reset(std::vector<Card> sortedPlayerCards) {
    this->cards = std::move(sortedPlayerCards);
    points = 0;
    type = {};
    color = 0;
    diffRanks = 0;
    maxOfOneRank = 0;
}

Evaluator::Evaluation Evaluator::eval(std::vector<Card> sortedPlayerCards) {
    reset(std::move(sortedPlayerCards));

    color = howMuchColors();
    diffRanks = howMuchDiffRanks();
    maxOfOneRank = howMuchMaxOfOneRank();

    if (isRoyalPoker()) {
        points = Class::ROYAL_POKER;
    } else if (isPoker()) {
        points = Class::POKER;
    } else if (isFourOfKind()) {
        points = Class::FOUR_OF_KIND;
    } else if (isFullHouse()) {
        points = Class::FULL_HOUSE;
    } else if (isFlush()) {
        points = Class::FLUSH;
    } else if (isStraight()) {
        points = Class::STRAIGHT;
    } else if (isThreeOfKind()) {
        points = Class::THREE_OF_KIND;
    } else if (isTwoPairs()) {
        points = Class::TWO_PAIRS;
    } else if (isOnePair()) {
        points = Class::ONE_PAIR;
    } else {
        points = Class::HIGH_CARD;
        type = cards[4].getRank();
    }

    return {points, type};
}

bool Evaluator::isRoyalPoker() {
    if (color != 1 || diffRanks != 5 || maxOfOneRank != 1) { return false; }

    std::set<int> ranks;
    for (const auto &card: cards) {
        ranks.insert(card.getRank());
    }

    return ranks.count(AS) &&
           ranks.count(KROL) &&
           ranks.count(DAMA) &&
           ranks.count(WALET) &&
           ranks.count(DZIESIATKA);
}

bool Evaluator::isPoker() {
    if (color != 1 || diffRanks != 5 || maxOfOneRank != 1) { return false; }

    std::set<int> ranks;
    for (const auto &card: cards) {
        ranks.insert(card.getRank());
    }

    return ranks.count(KROL) &&
           ranks.count(DAMA) &&
           ranks.count(WALET) &&
           ranks.count(DZIESIATKA) &&
           ranks.count(DZIEWIATKA);
}

bool Evaluator::isFourOfKind() {
    if (color != 4 || diffRanks != 2 || maxOfOneRank != 4) return false;

    if (cards[0].getRank() == cards[1].getRank()) {
        type = cards[0].getRank();
    } else {
        type = cards[1].getRank();
    }
    return true;
}

bool Evaluator::isFullHouse() {
    if (!(color == 3 || color == 4) || diffRanks != 2 || maxOfOneRank != 3) { return false; }

    if (cards[0].getRank() == cards[1].getRank() && cards[1].getRank() == cards[2].getRank()) {
        type = cards[0].getRank();
    } else {
        type = cards[2].getRank();
    }
    return true;
}

bool Evaluator::isFlush() {
    if (color != 1 || diffRanks != 5 || maxOfOneRank != 1) { return false; }

    type = cards[cards.size() - 1].getRank();
    return true;
}

bool Evaluator::isStraight() {
    std::vector<int> ranks;
    for (const auto &card: cards) {
        ranks.push_back(card.getRank());
    }

    std::sort(ranks.begin(), ranks.end());

    if (((color == 2 || color == 3 || color == 4) && diffRanks == 5 && maxOfOneRank == 1) &&
        ((ranks == std::vector<int>{DZIEWIATKA, DZIESIATKA, WALET, DAMA, KROL}) ||
         (ranks == std::vector<int>{DZIESIATKA, WALET, DAMA, KROL, AS})
        )) {
        type = cards.back().getRank();
        return true;
    }

    return false;
}

bool Evaluator::isThreeOfKind() {
    if (color != 3 && color != 4 || diffRanks != 3 || maxOfOneRank != 3) { return false; }

    if (cards[0].getRank() == cards[1].getRank() && cards[1].getRank() == cards[2].getRank()) {
        type = cards[0].getRank();
    } else if (cards[1].getRank() == cards[2].getRank() && cards[2].getRank() == cards[3].getRank()) {
        type = cards[1].getRank();
    } else {
        type = cards[2].getRank();
    }

    return true;
}

bool Evaluator::isTwoPairs() {
    if (!(color == 2 || color == 3 || color == 4) || diffRanks != 3 || maxOfOneRank != 2) { return false; }

    type = cards[3].getRank();
    return true;
}

bool Evaluator::isOnePair() {
    if (color < 2 || color > 4 || diffRanks != 4 || maxOfOneRank != 2) { return false; }

    type = cards[0].getRank() == cards[1].getRank() || cards[1].getRank() == cards[2].getRank()
           ? cards[1].getRank()
           : cards[3].getRank();

    return true;
}

int Evaluator::howMuchColors() {
    std::array<int, 4> colors = {0};
    for (const auto &card: cards) {
        colors[card.getSuit()] = 1;
    }
    return (int) std::count(colors.begin(), colors.end(), 1);
}

int Evaluator::howMuchDiffRanks() {
    std::array<bool, 15> ranks = {false};
    for (const auto &card: cards) {
        ranks[card.getRank()] = true;
    }
    return (int) std::count(ranks.begin(), ranks.end(), true);
}

int Evaluator::howMuchMaxOfOneRank() {
    std::array<int, 15> ranks = {0};
    for (const auto &card: cards) {
        ranks[card.getRank()]++;
    }
    return *std::max_element(ranks.begin(), ranks.end());
}

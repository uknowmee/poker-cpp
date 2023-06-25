//
// Created by micha on 20/06/2023.
//

#ifndef MY_PROJECTS_EVALUATOR_H
#define MY_PROJECTS_EVALUATOR_H


#include <utility>
#include <vector>
#include <algorithm>
#include <set>
#include "../CustomDeck/Card.h"
#include "../CustomDeck/Class.h"

class Evaluator {

public:
    struct Evaluation{
        int points;
        Rank type;
    };

    explicit Evaluator();

private:
    std::vector<Card> cards;
    int points;
    Rank type;
    int color;
    int maxOfOneRank;
    int diffRanks;

    void reset(std::vector<Card> cards);
    bool isRoyalPoker();
    bool isPoker();
    bool isFourOfKind();
    bool isFullHouse();
    bool isFlush();
    bool isStraight();
    bool isThreeOfKind();
    bool isTwoPairs();
    bool isOnePair();
    int howMuchColors();
    int howMuchDiffRanks();
    int howMuchMaxOfOneRank();

public:
    static Evaluator createEvaluator();
    Evaluator::Evaluation eval(std::vector<Card> sortedPlayerCards);
};


#endif //MY_PROJECTS_EVALUATOR_H

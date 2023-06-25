//
// Created by micha on 22/06/2023.
//

#ifndef MY_PROJECTS_CARDMAPPER_H
#define MY_PROJECTS_CARDMAPPER_H


#include <string>
#include <map>
#include "Rank.h"
#include "Suit.h"
#include "Class.h"

class CardMapper {

public:
    static std::map<Rank, std::string> rankMap;
    static std::map<Suit, std::string> suitMap;
    static std::map<int, std::string> pointsMap;
};


#endif //MY_PROJECTS_CARDMAPPER_H

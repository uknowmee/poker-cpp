//
// Created by micha on 22/06/2023.
//

#include "CardMapper.h"

std::map<Rank, std::string> CardMapper::rankMap = {
        {DZIEWIATKA, "DZIEWIATKA"},
        {DZIESIATKA, "DZIESIATKA"},
        {WALET,      "WALET"},
        {DAMA,       "DAMA"},
        {KROL,       "KROL"},
        {AS,         "AS"}
};

std::map<Suit, std::string> CardMapper::suitMap = {
        {TREFL, "TREFL"},
        {KARO,  "KARO"},
        {KIER,  "KIER"},
        {PIK,   "PIK"}
};

std::map<int, std::string> CardMapper::pointsMap = {
        {ROYAL_POKER, "ROYAL_POKER"},
        {POKER, "POKER"},
        {FOUR_OF_KIND, "FOUR_OF_KIND"},
        {FULL_HOUSE, "FULL_HOUSE"},
        {FLUSH, "FLUSH"},
        {STRAIGHT, "STRAIGHT"},
        {THREE_OF_KIND, "THREE_OF_KIND"},
        {TWO_PAIRS, "TWO_PAIRS"},
        {ONE_PAIR, "ONE_PAIR"},
        {HIGH_CARD, "HIGH_CARD"}
};
//
// Created by micha on 20/06/2023.
//

#ifndef MY_PROJECTS_DECKMASTER_H
#define MY_PROJECTS_DECKMASTER_H


#include <vector>
#include <deque>
#include "Player.h"
#include "Evaluator.h"

class DeckMaster {

private:

    Evaluator evaluator;

    explicit DeckMaster(Evaluator  evaluator);
    static void shuffleBeforeDealing(std::vector<Card> &cards);
    static void sortPlayingPlayersCards(std::deque<Player> &players);

public:
    DeckMaster();
    static DeckMaster createDeckMaster();
    static void dealTheCards(std::deque<Player> &players, std::vector<Card> &cards);
    static void collectCardsFromPlayingPlayers(std::deque<Player> &players, std::vector<Card> &cards);
    static void collectCardsFromPlayer(Player &player, std::vector<Card> &cards);
    static void collectPlayerCards(Player &player, std::vector<Card> &cards, std::vector<int> cardsToCollect);
    void evaluatePlayingPlayersCards(std::deque<Player> &players);
    static std::vector<Card> createFabricDeck();
};


#endif //MY_PROJECTS_DECKMASTER_H

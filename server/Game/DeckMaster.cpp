//
// Created by micha on 20/06/2023.
//

#include <random>
#include <utility>
#include "DeckMaster.h"

DeckMaster DeckMaster::createDeckMaster() {
    return DeckMaster(Evaluator::createEvaluator());
}

DeckMaster::DeckMaster(Evaluator  evaluator) : evaluator(std::move(evaluator)) {

}

DeckMaster::DeckMaster() = default;

void DeckMaster::dealTheCards(std::deque<Player> &players, std::vector<Card> &cards) {
    shuffleBeforeDealing(cards);

    for (int i = 0; i < 5; i++) {
        for (auto &player: players) {
            player.addCard(cards.back());
            cards.pop_back();
        }
    }

    sortPlayingPlayersCards(players);
}

void DeckMaster::shuffleBeforeDealing(std::vector<Card> &cards) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
}

void DeckMaster::sortPlayingPlayersCards(std::deque<Player> &players) {
    for (Player& player : players) {
        std::vector<Card>& cards = player.playerCardsRef();

        if (cards.empty()) { continue; }

        std::sort(cards.begin(), cards.end(), [](const Card& a, const Card& b) {
            return a.getNumber() < b.getNumber();
        });
    }
}

void DeckMaster::collectCardsFromPlayingPlayers(std::deque<Player> &players, std::vector<Card> &cards) {
    for (auto &player: players) {
        collectCardsFromPlayer(player, cards);
    }
}

void DeckMaster::collectCardsFromPlayer(Player &player, std::vector<Card> &cards) {
    for (auto &card: player.playerCardsRef()) {
        cards.push_back(card);
    }
    player.playerCardsRef().clear();
}

void DeckMaster::collectPlayerCards(Player &player, std::vector<Card> &cards, std::vector<int> cardsToCollect) {
    std::vector<Card> &playerCards = player.playerCardsRef();

    for (int i : cardsToCollect) {
        Card card = playerCards[i];
        cards.push_back(card);
    }

    for (int i = (int)playerCards.size() - 1; i >= 0; i--) {
        if (std::find(cardsToCollect.begin(), cardsToCollect.end(), i) != cardsToCollect.end()) {
            playerCards.erase(playerCards.begin() + i);
        }
    }
}

void DeckMaster::evaluatePlayingPlayersCards(std::deque<Player> &players) {
    for (Player &player: players) {

        Evaluator::Evaluation evaluation = evaluator.eval(player.cards());
        player.setPoints(evaluation.points);
        player.setType(evaluation.type);
    }
}

std::vector<Card> DeckMaster::createFabricDeck() {
    int num = 0;
    std::vector<Card> fabric;
    for (Rank rank : {Rank::DZIEWIATKA, Rank::DZIESIATKA, Rank::WALET, Rank::DAMA, Rank::KROL, Rank::AS}) {
        for (Suit suit : {Suit::PIK, Suit::TREFL, Suit::KIER, Suit::KARO}) {
            fabric.emplace_back(rank, suit, num);
            num++;
        }
    }
    return fabric;
}


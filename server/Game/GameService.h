//
// Created by micha on 21/06/2023.
//

#ifndef MY_PROJECTS_GAMESERVICE_H
#define MY_PROJECTS_GAMESERVICE_H


#include <algorithm>
#include <string>
#include <vector>
#include "Game.h"
#include "DeckMaster.h"
#include "GameServiceCommandController.h"
#include "../Server/ServerGameController.h"
#include "../Server/MessagePrinter.h"

class GameService : public GameServiceCommandController {

private:
    Game game;
    DeckMaster deckMaster;
    ServerGameController *serverGameController;

    explicit GameService(int maxNumOfPlayers, ServerGameController *serverGameController);
    void startGame();
    void removeCreditFromPlayers(std::deque<Player> &playingPlayers);
    void gameResetBetweenRounds();
    void adjustWinnersBalance(std::vector<Player *> &winnersInPlayingPlayers, int value);
    void removePlayerIfNotStarted(const std::string &playerName);
    void resetGame(const std::string& playerName);
    bool updateIfFirstPlayerReadyForExchange();
    bool updateIfSecondPartFinished();
    void makeWinners();
    MoveInfo eitherGameOrRoundFinished();
    bool playerIsNotKicked(const std::string &playerName);
    static void removeDiffFromPlayers(std::deque<Player> &playingPlayers);

    //GameServiceCommandController
    void removeCreditFromPlayerAndPlayingPlayer(Player &player, int value) override;
    void sendInfoToAllPlayingPlayers() override;
    void updateQueue() override;
    MoveInfo moveAccepted() override;
    MoveInfo exchangeMoveAcceptedInAllInCase() override;
    void finishRound() override;
    void finishGame() override;
    bool isPlayerTurn(const std::string &senderName) override;
    DeckMaster *getDeckMaster() override;
    Game *getGame() override;
    std::string currentPlayerName() override;
    std::string lastPlayerName() override;
    int numOfPlayers() override;
    std::string playingPlayerInfo(const std::string &senderName) override;

public:
    static GameService createGameService(ServerGameController *serverGameController, int maxNumOfPlayers);
    void tryStartGame();
    void addPlayer(const std::string& playerName);
    void removeDisconnectedPlayer(const std::string &playerName);
    bool isGameStarted() override;
    std::string toString() const;
};


#endif //MY_PROJECTS_GAMESERVICE_H

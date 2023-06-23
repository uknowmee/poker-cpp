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
    pthread_mutex_t gameStop{};
    Game game;
    DeckMaster deckMaster;
    ServerGameController *serverGameController;

    explicit GameService(int maxNumOfPlayers, ServerGameController *serverGameController);
    void startGame();
    void gameResetBetweenRounds();
    void resetGame(const std::string& playerName);
    void removePlayerIfNotStarted(const std::string &playerName);
    static void adjustWinnersBalance(std::vector<Player *> &players, int value);
    static void removeCreditFromPlayers(std::vector<Player> &players);
    bool isSenderNotCurrent(const std::string &senderName);
    bool playerIsNotKicked(const std::string &playerName);
    void notCurrent(const std::string &senderName);
    void notStarted(const std::string &senderName);
    void winnersCheck();
    void moveAccepted();
    void proceedExchange();
    void updateQueue();
    void updateIfFirstPlayerReadyForExchange();
    void updateIfSecondPartFinished();

    //GameServiceCommandController
    std::string playingPlayerInfo(const std::string &senderName) override;
    void invokeFold(const std::string &senderName) override;
    void invokeCheck(const std::string &senderName) override;
    void invokeCall(const std::string &senderName) override;
    void invokeAll(const std::string &senderName) override;
    void invokeCya(const std::string &senderName) override;
    void invokeBet(const std::string &senderName, int value) override;
    void invokeRaise(const std::string &senderName, int value) override;
    void invokeExchange(const std::string &senderName, const std::vector<int> &values) override;

public:
    static GameService createGameService(ServerGameController *serverGameController, int maxNumOfPlayers);
    void tryStartGame();
    void addPlayer(const std::string& playerName);
    void removeDisconnectedPlayer(const std::string &playerName);
    bool isGameStarted() override;
    std::string toString() const;
};


#endif //MY_PROJECTS_GAMESERVICE_H

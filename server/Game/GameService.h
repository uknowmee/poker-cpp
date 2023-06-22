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
#include "../Server/ServerGameController.h"
#include "../Server/MessagePrinter.h"

class GameService {

private:
    Game game;
    DeckMaster deckMaster;
    ServerGameController *serverGameController;

    explicit GameService(int maxNumOfPlayers, ServerGameController *serverGameController);
    void gameResetBetweenRounds();
    void resetGame(const std::string& playerName);
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

    void invokeInfo(const std::string &senderName);
    void invokeFold(const std::string &senderName);
    void invokeCheck(const std::string &senderName);
    void invokeCall(const std::string &senderName);
    void invokeAll(const std::string &senderName);
    void invokeCya(const std::string &senderName);
    void invokeBet(const std::string &senderName, int value);
    void invokeRaise(const std::string &senderName, int value);
    void invokeExchange(const std::string &senderName, const std::vector<int> &values);

public:
    static GameService createGameService(ServerGameController *serverGameController, int maxNumOfPlayers);
    void startGame();

    void addPlayer(std::string playerName);
    void removeDisconnectedPlayer(const std::string &playerName);
    void invokeCommand(const std::string &command, const std::string &senderName);
    void invokeCommand(const std::string &command, int value, const std::string &senderName);
    void invokeCommand(const std::string &command, const std::vector<int>& values, const std::string &senderName);

    bool isGameStarted();
    std::string toString() const;
};


#endif //MY_PROJECTS_GAMESERVICE_H

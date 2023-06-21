//
// Created by micha on 21/06/2023.
//

#ifndef MY_PROJECTS_GAMESERVICE_H
#define MY_PROJECTS_GAMESERVICE_H


#include <string>
#include "Game.h"
#include "../Server/ServerGameController.h"

class GameService {

private:
    Game game;
    ServerGameController *serverGameController;

    explicit GameService(Game game, ServerGameController *serverGameController);

public:
    static GameService createGameService(ServerGameController *serverGameController);
    bool isGameStarted();
    void startGame();

    void addPlayer(const std::string &playerName);
    void removeDisconnectedPlayer(const std::string &playerName);

    void invokeInfo(const std::string &senderName);
    void invokeFold(const std::string &senderName);
    void invokeCheck(const std::string &senderName);
    void invokeCall(const std::string &senderName);
    void invokeAll(const std::string &senderName);
    void invokeCya(const std::string &senderName);
    void invokeBet(const std::string &senderName);
    void invokeRaise(const std::string &senderName);
    void invokeExchange(const std::string &senderName);
};


#endif //MY_PROJECTS_GAMESERVICE_H

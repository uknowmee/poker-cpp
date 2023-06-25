//
// Created by micha on 23/06/2023.
//

#ifndef MY_PROJECTS_GAMESERVICECOMMANDCONTROLLER_H
#define MY_PROJECTS_GAMESERVICECOMMANDCONTROLLER_H

#include "MoveInfo.h"

class GameServiceCommandController {
public:
    virtual std::string playingPlayerInfo(const std::string &senderName) = 0;
    virtual bool isGameStarted() = 0;
    virtual bool isPlayerTurn(const std::string &senderName) = 0;
    virtual int numOfPlayers() = 0;
    virtual std::string currentPlayerName() = 0;
    virtual std::string lastPlayerName() = 0;

    virtual MoveInfo invokeFold(const std::string &senderName) = 0;
    virtual MoveInfo invokeCheck(const std::string &senderName) = 0;
    virtual MoveInfo invokeCall(const std::string &senderName) = 0;
    virtual MoveInfo invokeAll(const std::string &senderName) = 0;
    virtual MoveInfo invokeCya(const std::string &senderName) = 0;
    virtual MoveInfo invokeBet(const std::string &senderName, int value) = 0;
    virtual MoveInfo invokeRaise(const std::string &senderName, int value) = 0;
    virtual void invokeExchange(const std::string &senderName, const std::vector<int> &values) = 0;
};

#endif //MY_PROJECTS_GAMESERVICECOMMANDCONTROLLER_H

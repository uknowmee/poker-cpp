//
// Created by micha on 23/06/2023.
//

#ifndef MY_PROJECTS_GAMESERVICECOMMANDCONTROLLER_H
#define MY_PROJECTS_GAMESERVICECOMMANDCONTROLLER_H

class GameServiceCommandController {
public:
    virtual std::string playingPlayerInfo(const std::string &senderName) = 0;
    virtual bool isGameStarted() = 0;
    virtual bool isPlayerTurn(const std::string &senderName) = 0;
    virtual int numOfPlayers() = 0;
    virtual std::string currentPlayerName() = 0;
    virtual std::string lastPlayerName() = 0;

    virtual bool invokeFold(const std::string &senderName) = 0;
    virtual bool invokeCheck(const std::string &senderName) = 0;
    virtual bool invokeCall(const std::string &senderName) = 0;
    virtual bool invokeAll(const std::string &senderName) = 0;
    virtual bool invokeCya(const std::string &senderName) = 0;
    virtual bool invokeBet(const std::string &senderName, int value) = 0;
    virtual bool invokeRaise(const std::string &senderName, int value) = 0;
    virtual void invokeExchange(const std::string &senderName, const std::vector<int> &values) = 0;
};

#endif //MY_PROJECTS_GAMESERVICECOMMANDCONTROLLER_H

//
// Created by micha on 23/06/2023.
//

#ifndef MY_PROJECTS_GAMESERVICECOMMANDCONTROLLER_H
#define MY_PROJECTS_GAMESERVICECOMMANDCONTROLLER_H

class GameServiceCommandController {
public:
    virtual std::string playingPlayerInfo(const std::string &senderName) = 0;
    virtual bool isGameStarted() = 0;

    virtual void invokeFold(const std::string &senderName) = 0;
    virtual void invokeCheck(const std::string &senderName) = 0;
    virtual void invokeCall(const std::string &senderName) = 0;
    virtual void invokeAll(const std::string &senderName) = 0;
    virtual void invokeCya(const std::string &senderName) = 0;
    virtual void invokeBet(const std::string &senderName, int value) = 0;
    virtual void invokeRaise(const std::string &senderName, int value) = 0;
    virtual void invokeExchange(const std::string &senderName, const std::vector<int> &values) = 0;
};

#endif //MY_PROJECTS_GAMESERVICECOMMANDCONTROLLER_H

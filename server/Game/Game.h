//
// Created by micha on 20/06/2023.
//

#ifndef MY_PROJECTS_GAME_H
#define MY_PROJECTS_GAME_H


#include <string>

class Game {

private:
    bool started = false;

public:
    static Game createGame();
    bool isStarted() const;
    void start();

    void addPlayer(const std::__cxx11::basic_string<char> &playerName);
    void removePlayer(const std::string &playerName);

    void invokeInfo(const std::string &senderName);
    void invokeFold(const std::string &senderName);
    void invokeCheck(const std::string &senderName);
    void invokeCall(const std::string &senderName);
    void invokeBet(const std::string &senderName);
    void invokeRaise(const std::string &senderName);
    void invokeAll(const std::string &senderName);
    void invokeExchange(const std::string &senderName);
    void invokeCya(const std::string &senderName);
};


#endif //MY_PROJECTS_GAME_H

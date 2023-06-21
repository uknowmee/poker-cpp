//
// Created by micha on 20/06/2023.
//

#ifndef MY_PROJECTS_GAME_H
#define MY_PROJECTS_GAME_H


#include <string>

class Game {

private:
    bool started;
    Game();

public:
    static Game createGame();
    bool isStarted() const;
    void start();
    void addPlayer(const std::string &playerName);
    void removePlayer(const std::string &playerName);
};


#endif //MY_PROJECTS_GAME_H

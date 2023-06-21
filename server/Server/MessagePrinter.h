//
// Created by micha on 20/06/2023.
//

#ifndef MY_PROJECTS_MESSAGEPRINTER_H
#define MY_PROJECTS_MESSAGEPRINTER_H


#include <string>
#include <iostream>

class MessagePrinter {

public:
    MessagePrinter() = default;
    static void printMessage(
            const std::__cxx11::basic_string<char> &message,
            const std::__cxx11::basic_string<char> &senderName
    );
    static void printStartGameMessage();
    static std::string printRemovePlayerMessage(const std::string &playerName);
    static std::string printAddPlayerMessage(const std::string &playerName);
    static std::string gameStartMessage();
    static std::string welcomeMessage(const std::string &playerName);
    static std::string numberOfConnectedPlayersInfoMessage(int numOfPlayers);
    static std::string serverStopCommandMessage();
    static std::string serverStopMessage();
};


#endif //MY_PROJECTS_MESSAGEPRINTER_H

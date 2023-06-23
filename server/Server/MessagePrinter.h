//
// Created by micha on 20/06/2023.
//

#ifndef MY_PROJECTS_MESSAGEPRINTER_H
#define MY_PROJECTS_MESSAGEPRINTER_H


#include <string>
#include <iostream>
#include "ParsedMessage.h"

class MessagePrinter {

public:
    MessagePrinter() = default;
    static void printMessage(
            const std::string &message,
            const std::string &senderName
    );
    static void printStartGameMessage();
    static std::string printRemovePlayerMessage(const std::string &playerName);
    static std::string printAddPlayerMessage(const std::string &playerName);
    static std::string gameStartMessage();
    static std::string welcomeMessage(const std::string &playerName);
    static std::string numberOfConnectedPlayersInfoMessage(int numOfPlayers);
    static std::string serverStopCommandMessage();
    static std::string serverStopMessage();
    static std::string byeMessage();
    static std::string invalidCommandMessage(const std::string& message);
    static std::string helpMessage();
    static std::string gameAlreadyStartedMessage();
    static std::string addIndentation(const std::string &input, const std::string &indent);
    static std::string printGameEndedDueToDisconnectionMessage(const std::string& playerName);
    static std::string notYourTurnMessage(const std::string& playerName);
    static std::string playerInfoMessage(const std::string& playerInfo);
    static std::string gameNotStartedMessage();
    static std::string invalidServerCommand();
    static void printParsedMessage(const ParsedMessage& parsedMessage);
};


#endif //MY_PROJECTS_MESSAGEPRINTER_H

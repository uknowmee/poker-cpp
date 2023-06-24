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
    static std::string printBetTooLowMessage(int minimumBet);
    static std::string printBetTooHighMessage();
    static std::string printRaiseTooLowMessage(int minimumRaise);
    static std::string printRaiseTooHighMessage();
    static std::string printExchangeAccepted();
    static std::string printExchangeFinishedMessage();
    static std::string gameStartMessage();
    static std::string welcomeMessage(const std::string &playerName);
    static std::string numberOfConnectedPlayersInfoMessage(int numOfPlayers);
    static std::string serverStopCommandMessage();
    static std::string serverStopMessage();
    static std::string byeMessage();
    static std::string invalidCommandMessage(const std::string& message);
    static std::string helpMessage();
    static std::string addIndentation(const std::string &input, const std::string &indent);
    static std::string printGameEndedDueToDisconnectionMessage(const std::string& playerName);
    static std::string notYourTurnMessage(const std::string& playerName, const std::string& currentPlayerName);
    static std::string playerInfoMessage(const std::string& playerInfo);
    static std::string gameNotStartedMessage(const int &numOfPlayers);
    static std::string invalidServerCommand();
    static void printParsedMessage(const ParsedMessage& parsedMessage);
    static std::string yourTurnMessage(const std::string& currentPlayerName);
    static std::string whatHappenedMessage(const std::string& message, const std::string& currentPlayer, const std::string& lastPlayer);

    static std::string moveNotAllowedMessage(const std::string& message);
};


#endif //MY_PROJECTS_MESSAGEPRINTER_H

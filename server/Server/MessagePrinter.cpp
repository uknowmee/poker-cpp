//
// Created by micha on 20/06/2023.
//

#include "MessagePrinter.h"

void MessagePrinter::printMessage(
        const std::basic_string<char> &message,
        const std::basic_string<char> &senderName
) {
    std::string fullMessage = "[" + senderName + "]: " + message;
    std::cout << fullMessage << std::endl;
}

void MessagePrinter::printStartGameMessage() {
    std::cout << gameStartMessage() << std::endl;
}

std::string MessagePrinter::printRemovePlayerMessage(const std::string &playerName) {
    std::string message = playerName + " has left the game!";
    std::cout << message << std::endl;

    return message;
}

std::string MessagePrinter::printAddPlayerMessage(const std::string &playerName) {
    std::string message = playerName + " has joined the game!";
    std::cout << message << std::endl;

    return message;
}

std::string MessagePrinter::gameStartMessage() {
    return "The game has started!";
}

std::string MessagePrinter::welcomeMessage(const std::string &playerName) {
    return "Welcome to the game! You are " + playerName;
}

std::string MessagePrinter::numberOfConnectedPlayersInfoMessage(int numOfPlayers) {
    return numOfPlayers == 1
    ? "There is 1 player in the game"
    : "There are " + std::to_string(numOfPlayers) + " players in the game";
}

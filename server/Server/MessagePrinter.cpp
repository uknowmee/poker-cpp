//
// Created by micha on 20/06/2023.
//

#include <sstream>
#include "MessagePrinter.h"

void MessagePrinter::printMessage(
        const std::string &message,
        const std::string &senderName
) {
    std::string fullMessage = "~~[" + senderName + "]: " + message;
    std::cout << fullMessage << std::endl;
}

void MessagePrinter::printParsedMessage(const ParsedMessage &parsedMessage) {
    std::cout << "Parsed message as: "
              << std::endl
              << addIndentation(parsedMessage.toString(), "\t") << std::endl;
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

std::string MessagePrinter::printGameEndedDueToDisconnectionMessage(const std::string &playerName) {
    std::string message = "Game has ended due to " + playerName + " disconnection";
    std::cout << message << std::endl;
    return message;
}

std::string MessagePrinter::gameStartMessage() {
    return "The game has started!";
}

std::string MessagePrinter::welcomeMessage(const std::string &playerName) {
    return "Welcome to the game! You are " + playerName + "! Type /help to see all commands.";
}

std::string MessagePrinter::numberOfConnectedPlayersInfoMessage(int numOfPlayers) {
    return numOfPlayers == 1
           ? "There is 1 player in the game"
           : "There are " + std::to_string(numOfPlayers) + " players in the game";
}

std::string MessagePrinter::serverStopCommandMessage() {
    return "Type /stop to close server";
}

std::string MessagePrinter::serverStopMessage() {
    return "Server is closing...";
}

std::string MessagePrinter::byeMessage() {
    return "You are disconnecting. Cya later!";
}

std::string MessagePrinter::invalidCommandMessage(const std::string &message) {
    return "Couldn't parse command from message: " + message;
}

std::string MessagePrinter::helpMessage() {
    return R"(
###########################################################
commands:
/help - print all commands
/info - show in game info about current player (if in game)
/fold - ONLY IN GAME use it if u want to pass
/check - ONLY IN GAME use it if u want to check a player
/call - ONLY IN GAME use it if u want to call someones bet
/all - ONLY IN GAME use it if u cant call
/cya - ONLY IN GAME use it after u went down with money
/bet <money> - ONLY IN GAME use it if u want to bet
/raise <money> - ONLY IN GAME use it if u want to re-bet
/exchange <cardNum cardNum> - ONLY IN GAME use it to exchange cards
/bye - exit
###########################################################
)";
}

std::string MessagePrinter::addIndentation(const std::string &input, const std::string &indent) {
    std::stringstream result = std::stringstream();
    std::stringstream ss(input);
    std::string line;

    while (std::getline(ss, line)) {
        result << indent << line;
        if (!ss.eof()) {
            result << '\n';
        }
    }

    return result.str();
}

std::string MessagePrinter::notYourTurnMessage(const std::string &playerName, const std::string &currentPlayerName) {
    return "It's not your turn " + playerName + "!" + "\n" + "Wait for " + currentPlayerName + " to finish his turn";
}

std::string MessagePrinter::playerInfoMessage(const std::string &playerInfo) {
    return "Player info:\n" + addIndentation(playerInfo, "\t");
}

std::string MessagePrinter::gameNotStartedMessage(const int &numOfPlayers) {
    return "Game has not started yet. There are currently " +
           std::to_string(numOfPlayers) +
           (numOfPlayers == 1 ? " player" : " players") +
           " in the lobby.";
}

std::string MessagePrinter::invalidServerCommand() {
    return "Invalid server command";
}

std::string MessagePrinter::yourTurnMessage(const std::string& currentPlayerName) {
    return "It's your turn " + currentPlayerName + "!";
}

std::string MessagePrinter::printBetTooLowMessage(int minimumBet) {
    std::string message = "Bet is too low! Minimum bet is " + std::to_string(minimumBet);
    std::cout << message << std::endl;
    return message;
}

std::string MessagePrinter::printBetTooHighMessage() {
    std::string message = "Bet is too high! You don't have enough money";
    std::cout << message << std::endl;
    return message;
}

std::string MessagePrinter::printRaiseTooLowMessage(int minimumRaise) {
    std::string message = "Raise is too low! Minimum raise is " + std::to_string(minimumRaise);
    std::cout << message << std::endl;
    return message;
}

std::string MessagePrinter::printRaiseTooHighMessage() {
    std::string message = "Raise is too high! You don't have enough money";
    std::cout << message << std::endl;
    return message;
}

std::string MessagePrinter::printExchangeAccepted() {
    std::string message = "Exchange accepted!";
    std::cout << message << std::endl;
    return message;
}

std::string MessagePrinter::printExchangeFinishedMessage() {
    std::string message = "Exchange finished!";
    std::cout << message << std::endl;
    return message;
}

std::string MessagePrinter::whatHappenedMessage(const std::string& message, const std::string& currentPlayer, const std::string& lastPlayer) {
    return "Player: " + lastPlayer + ", made a move: " + message + " now is turn of: " + currentPlayer;
}

std::string MessagePrinter::moveNotAllowedMessage(const std::string& message) {
    return "Move not allowed: " + message;
}

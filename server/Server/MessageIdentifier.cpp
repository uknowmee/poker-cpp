//
// Created by micha on 20/06/2023.
//

#include <algorithm>
#include "MessageIdentifier.h"

ParsedMessage MessageIdentifier::identifyMessage(
        const std::string &message,
        const std::string &senderName
) {
    MessagePrinter::printMessage(message, senderName);
    ParsedMessage parsedMessage = MessageIdentifier::parseMessage(message, senderName);
    MessagePrinter::printParsedMessage(parsedMessage);

    return parsedMessage;
}

ParsedMessage MessageIdentifier::parseMessage(
        const std::string &message,
        const std::string &senderName
) {
    std::vector<std::string> messageVector = splitString(message);
    ParsedMessage parsedMessage;

    if (message == "/bye") { return makeBye(senderName); }
    else if (message == "/help") { return makeHelp(senderName); }
    else if (message == "/info") { return makeInfo(senderName); }
    else if (message == "/fold") { return makeFold(senderName); }
    else if (message == "/check") { return makeCheck(senderName); }
    else if (message == "/call") { return makeCall(senderName); }
    else if (message == "/all") { return makeAll(senderName); }
    else if (message == "/cya") { return makeCya(senderName); }
    else if (messageVector[0] == "/bet") { parsedMessage = tryMakeBet(message, messageVector, senderName); }
    else if (messageVector[0] == "/raise") { parsedMessage = tryMakeRaise(message, messageVector, senderName); }
    else if (messageVector[0] == "/exchange") { parsedMessage = tryMakeExchange(message, messageVector, senderName); }

    return parsedMessage.isValidCommand
           ? parsedMessage
           : makeInvalid(message, senderName);
}

std::vector<std::string> MessageIdentifier::splitString(const std::string &input) {
    std::istringstream iss(input);
    std::vector<std::string> tokens;
    std::string token;

    while (std::getline(iss, token, ' ')) {
        tokens.push_back(token);
    }

    return tokens;
}

ParsedMessage MessageIdentifier::makeBye(const std::string &senderName) {
    return {
            "/bye",
            senderName,
            "bye",
            0,
            {},
            false,
            false,
            true
    };
}

ParsedMessage MessageIdentifier::makeHelp(const std::string &senderName) {
    return {
            "/help",
            senderName,
            "help",
            0,
            {},
            false,
            false,
            true
    };
}

ParsedMessage MessageIdentifier::makeFold(const std::string &senderName) {
    return {
            "/fold",
            senderName,
            "fold",
            0,
            {},
            false,
            false,
            true
    };
}

ParsedMessage MessageIdentifier::makeCheck(const std::string &senderName) {
    return {
            "/check",
            senderName,
            "check",
            0,
            {},
            false,
            false,
            true
    };
}

ParsedMessage MessageIdentifier::makeCall(const std::string &senderName) {
    return {
            "/call",
            senderName,
            "call",
            0,
            {},
            false,
            false,
            true
    };
}

ParsedMessage MessageIdentifier::makeAll(const std::string &senderName) {
    return {
            "/all",
            senderName,
            "all",
            0,
            {},
            false,
            false,
            true
    };
}

ParsedMessage MessageIdentifier::makeCya(const std::string &senderName) {
    return {
            "/cya",
            senderName,
            "cya",
            0,
            {},
            false,
            false,
            true
    };
}

ParsedMessage MessageIdentifier::makeInfo(const std::string &senderName) {
    return {
            "/info",
            senderName,
            "info",
            0,
            {},
            false,
            false,
            true
    };
}

ParsedMessage MessageIdentifier::tryMakeBet(
        const std::string &message,
        std::vector<std::string> messageVector,
        const std::string &senderName
) {
    if (messageVector.size() != 2) { return makeInvalid(message, senderName); }
    if (!isNumber(messageVector[1])) { return makeInvalid(message, senderName); }
    if (std::stoi(messageVector[1]) < 0) { return makeInvalid(message, senderName); }

    return {
            message,
            senderName,
            "bet",
            std::stoi(messageVector[1]),
            {},
            true,
            false,
            true
    };
}

ParsedMessage MessageIdentifier::tryMakeRaise(
        const std::string &message,
        std::vector<std::string> messageVector,
        const std::string &senderName
) {
    if (messageVector.size() != 2) { return makeInvalid(message, senderName); }
    if (!isNumber(messageVector[1])) { return makeInvalid(message, senderName); }
    if (std::stoi(messageVector[1]) < 0) { return makeInvalid(message, senderName); }

    return {
            message,
            senderName,
            "raise",
            std::stoi(messageVector[1]),
            {},
            true,
            false,
            true
    };
}

ParsedMessage MessageIdentifier::tryMakeExchange(
        const std::string &message,
        std::vector<std::string> messageVector,
        const std::string &senderName
) {
    if (messageVector.size() <= 1) { return makeInvalid(message, senderName); }

    std::vector<int> cardsToExchange;
    for (int i = 1; i < messageVector.size(); i++) {
        if (!isNumber(messageVector[i])) { return makeInvalid(message, senderName); }
        if (std::stoi(messageVector[i]) < 0 || std::stoi(messageVector[i]) > 4) { return makeInvalid(message, senderName); }
        cardsToExchange.push_back(std::stoi(messageVector[i]));
    }

    return {
            message,
            senderName,
            "exchange",
            0,
            cardsToExchange,
            false,
            true,
            true
    };
}

ParsedMessage MessageIdentifier::makeInvalid(const std::string &message, const std::string &senderName) {
    return {
            message,
            senderName,
            "",
            0,
            {},
            false,
            false,
            false
    };
}

bool MessageIdentifier::isNumber(const std::string &mayBeValue) {
    return !mayBeValue.empty() &&
           std::find_if(
                   mayBeValue.begin(),
                   mayBeValue.end(),
                   [](char c) { return !std::isdigit(c); }
           ) == mayBeValue.end();
}

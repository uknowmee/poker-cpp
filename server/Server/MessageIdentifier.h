//
// Created by micha on 20/06/2023.
//

#ifndef MY_PROJECTS_MESSAGEIDENTIFIER_H
#define MY_PROJECTS_MESSAGEIDENTIFIER_H

#include <sstream>
#include <string>
#include <vector>
#include "MessagePrinter.h"
#include "ParsedMessage.h"

class MessageIdentifier {

private:
    static ParsedMessage parseMessage(const std::string &message, const std::string &senderName);
    static ParsedMessage makeBye(const std::string &senderName);
    static ParsedMessage makeHelp(const std::string &senderName);
    static ParsedMessage makeFold(const std::string &senderName);
    static ParsedMessage makeCheck(const std::string &senderName);
    static ParsedMessage makeCall(const std::string &senderName);
    static ParsedMessage makeAll(const std::string &senderName);
    static ParsedMessage makeCya(const std::string &senderName);
    static ParsedMessage makeInfo(const std::string &senderName);
    static ParsedMessage tryMakeBet(const std::string &message, std::vector<std::string> messageVector, const std::string &senderName);
    static ParsedMessage tryMakeRaise(const std::string &message, std::vector<std::string> messageVector, const std::string &senderName);
    static ParsedMessage tryMakeExchange(const std::string &message, std::vector<std::string> messageVector, const std::string &senderName);
    static ParsedMessage makeInvalid(const std::string &message, const std::string &senderName);

    static std::vector<std::string> splitString(const std::string &input);
    static bool isNumber(const std::string& mayBeValue);

public:
    MessageIdentifier() = default;
    static ParsedMessage identifyMessage(const std::string &message, const std::string &senderName);
};


#endif //MY_PROJECTS_MESSAGEIDENTIFIER_H

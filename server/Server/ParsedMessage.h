//
// Created by micha on 21/06/2023.
//

#ifndef MY_PROJECTS_PARSEDMESSAGE_H
#define MY_PROJECTS_PARSEDMESSAGE_H


#include <string>
#include <vector>

class ParsedMessage {

public:
    std::string message;
    std::string senderName;
    std::string command;
    int value;
    std::vector<int> values = {};
    bool hasValue;
    bool hasValues;
    bool isValidCommand;


    ParsedMessage();
    ParsedMessage(
            std::string message,
            std::string senderName,
            std::string command,
            int value,
            std::vector<int> values,
            bool hasValue,
            bool hasValues,
            bool isValidCommand
    );
    std::string toString() const;
    std::string toStringForInvalidCommand() const;
    std::string toStringForValidCommand() const;
};


#endif //MY_PROJECTS_PARSEDMESSAGE_H

//
// Created by micha on 21/06/2023.
//

#include "ParsedMessage.h"

#include <utility>

ParsedMessage::ParsedMessage() {
    message = "";
    senderName = "";
    command = "";
    value = 0;
    values = {};
    hasValue = false;
    hasValues = false;
    isValidCommand = false;
}

std::string ParsedMessage::toString() const {
    if (isValidCommand) { return toStringForValidCommand(); }
    return toStringForInvalidCommand();
}

std::string ParsedMessage::toStringForInvalidCommand() const {
    std::string result = "Sender: " + senderName + "\n";
    result += "Message: " + message + "\n";
    result += "Is invalid message!!!";
    return result;
}

std::string ParsedMessage::toStringForValidCommand() const {
    std::string result = "Sender: " + senderName + "\n";
    result += "Command: " + command + "\n";
    if (hasValue) { result += "Value: " + std::to_string(value); }
    if (hasValues) {
        result += "Values: ";
        for (int i = 0; i < values.size(); ++i) {
            result += std::to_string(values[i]);
            if (i != values.size() - 1) { result += " "; }
        }
    }
    return result;
}

ParsedMessage::ParsedMessage(
        std::string message,
        std::string senderName,
        std::string command,
        int value,
        std::vector<int> values,
        bool hasValue,
        bool hasValues,
        bool isValidCommand
)
        : message(std::move(message)),
          senderName(std::move(senderName)),
          command(std::move(command)),
          value(value),
          values(std::move(values)),
          hasValue(hasValue),
          hasValues(hasValues),
          isValidCommand(isValidCommand) {}

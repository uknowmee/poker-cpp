//
// Created by micha on 20/06/2023.
//

#include "MessageIdentifier.h"

MessageIdentifier MessageIdentifier::createMessageIdentifier(ServerCommandInvoker *serverCommandInvoker) {
    return MessageIdentifier(serverCommandInvoker);
}

MessageIdentifier::MessageIdentifier() : serverCommandInvoker(nullptr) {
}

MessageIdentifier::MessageIdentifier(ServerCommandInvoker *serverMessageSender) {
    this->serverCommandInvoker = serverMessageSender;
}

void MessageIdentifier::identifyMessage(
        const std::basic_string<char> &message,
        const std::basic_string<char> &senderName
) {
    MessagePrinter::printMessage(message, senderName);
    // todo logic for handling messages
    serverCommandInvoker->invokeCommand(message, senderName);
}

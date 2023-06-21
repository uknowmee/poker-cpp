//
// Created by micha on 20/06/2023.
//

#include "MessageHandler.h"

MessageHandler MessageHandler::createMessageHandler(ServerCommandInvoker *serverMessageSender) {
    return MessageHandler(serverMessageSender);
}

MessageHandler::MessageHandler() : serverMessageInvoker(nullptr) {
}

MessageHandler::MessageHandler(ServerCommandInvoker *serverMessageSender) {
    this->serverMessageInvoker = serverMessageSender;
}

void MessageHandler::handleMessage(
        const std::basic_string<char> &message,
        const std::basic_string<char> &senderName
) {
    MessagePrinter::printMessage(message, senderName);
    // todo logic for handling messages
    serverMessageInvoker->invokeCommand(message, senderName);
}

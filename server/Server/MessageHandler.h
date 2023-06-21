//
// Created by micha on 20/06/2023.
//

#ifndef MY_PROJECTS_MESSAGEHANDLER_H
#define MY_PROJECTS_MESSAGEHANDLER_H


#include <string>
#include "ServerCommandInvoker.h"
#include "MessagePrinter.h"

class MessageHandler {

private:

    ServerCommandInvoker *serverMessageInvoker;

    explicit MessageHandler(ServerCommandInvoker *serverMessageInvoker);

public:
    static MessageHandler createMessageHandler(ServerCommandInvoker *serverMessageInvoker);
    explicit MessageHandler();
    void handleMessage(
            const std::__cxx11::basic_string<char> &message,
            const std::__cxx11::basic_string<char> &senderName
    );
};


#endif //MY_PROJECTS_MESSAGEHANDLER_H

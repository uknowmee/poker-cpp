//
// Created by micha on 20/06/2023.
//

#ifndef MY_PROJECTS_MESSAGEIDENTIFIER_H
#define MY_PROJECTS_MESSAGEIDENTIFIER_H


#include <string>
#include "ServerCommandInvoker.h"
#include "MessagePrinter.h"

class MessageIdentifier {

private:
    ServerCommandInvoker *serverCommandInvoker;
    explicit MessageIdentifier(ServerCommandInvoker *serverCommandInvoker);

public:
    static MessageIdentifier createMessageIdentifier(ServerCommandInvoker *serverCommandInvoker);
    explicit MessageIdentifier();
    void identifyMessage(
            const std::__cxx11::basic_string<char> &message,
            const std::__cxx11::basic_string<char> &senderName
    );
};


#endif //MY_PROJECTS_MESSAGEIDENTIFIER_H

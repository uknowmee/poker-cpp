//
// Created by micha on 21/06/2023.
//

#ifndef MY_PROJECTS_SERVERCOMMANDINVOKER_H
#define MY_PROJECTS_SERVERCOMMANDINVOKER_H

#include <string>

class ServerCommandInvoker {

public:

    virtual void invokeByeCommand(const std::string &senderName) = 0;
    virtual void invokeHelpCommand(const std::string &senderName) = 0;
    virtual void invokeCommand(
            const std::string &command,
            const std::string &senderName
    ) = 0;
    virtual void invokeCommand(
            const std::string &command,
            int value,
            const std::string &senderName
    ) = 0;
    virtual void invokeInvalidCommand(
            const std::string &messageInfo,
            const std::string &senderName
    ) = 0;
};


#endif //MY_PROJECTS_SERVERCOMMANDINVOKER_H

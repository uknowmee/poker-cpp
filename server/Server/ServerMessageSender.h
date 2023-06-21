//
// Created by micha on 20/06/2023.
//

#ifndef MY_PROJECTS_SERVERMESSAGESENDER_H
#define MY_PROJECTS_SERVERMESSAGESENDER_H

#include <string>

class ServerMessageSender {

public:
    virtual void broadcastMessageExceptSender(const std::string &message, const std::string &senderName) = 0;
    virtual void broadcastMessage(const std::string &message) = 0;
    virtual void sendToClientFromSender(
            const std::string &message,
            const std::string &senderName,
            const std::string &receiverName
    ) = 0;
    virtual void sendToClient(const std::string &message, const std::string &receiverName) = 0;
};


#endif //MY_PROJECTS_SERVERMESSAGESENDER_H

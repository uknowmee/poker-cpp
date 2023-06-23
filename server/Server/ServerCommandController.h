//
// Created by micha on 21/06/2023.
//

#ifndef MY_PROJECTS_SERVERCOMMANDCONTROLLER_H
#define MY_PROJECTS_SERVERCOMMANDCONTROLLER_H

#include <string>

class ServerCommandController {

public:
    virtual void stop() = 0;
    virtual void printGameInfo() = 0;
};


#endif //MY_PROJECTS_SERVERCOMMANDCONTROLLER_H

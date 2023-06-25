//
// Created by micha on 23/06/2023.
//

#ifndef MY_PROJECTS_COMMAND_H
#define MY_PROJECTS_COMMAND_H

#include "../Server/MessagePrinter.h"

class Command {

public:
    virtual void execute() = 0;
    virtual ~Command() = default;
};

#endif //MY_PROJECTS_COMMAND_H

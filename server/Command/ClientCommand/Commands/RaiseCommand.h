//
// Created by micha on 23/06/2023.
//

#ifndef MY_PROJECTS_RAISECOMMAND_H
#define MY_PROJECTS_RAISECOMMAND_H


#include "../ClientCommand.h"
#include "../../../Game/Game.h"
#include "../../../Game/DeckMaster.h"

class RaiseCommand : public ClientCommand {

private:
    DeckMaster *deckMaster;
    Game *game;
    std::string senderName;
    int value;

    MoveInfo exactExecute() override;
    MoveInfo handleToLowRaise(int bid);
    MoveInfo handleToHighRaise();

public:
    explicit RaiseCommand(
            ServerGameController *server,
            const ParsedMessage &parsedMessage,
            GameServiceCommandController *gameService
    );
};


#endif //MY_PROJECTS_RAISECOMMAND_H

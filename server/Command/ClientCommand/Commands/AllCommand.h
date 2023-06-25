//
// Created by micha on 23/06/2023.
//

#ifndef MY_PROJECTS_ALLCOMMAND_H
#define MY_PROJECTS_ALLCOMMAND_H


#include "../ClientCommand.h"
#include "../../../Game/Game.h"
#include "../../../Game/DeckMaster.h"

class AllCommand : public ClientCommand {

private:
    DeckMaster *deckMaster;
    Game *game;

    MoveInfo exactExecute() override;

public:
    explicit AllCommand(
            ServerGameController *server,
            const ParsedMessage &parsedMessage,
            GameServiceCommandController *gameService
    );
};


#endif //MY_PROJECTS_ALLCOMMAND_H

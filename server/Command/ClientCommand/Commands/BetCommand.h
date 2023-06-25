//
// Created by micha on 23/06/2023.
//

#ifndef MY_PROJECTS_BETCOMMAND_H
#define MY_PROJECTS_BETCOMMAND_H


#include "../ClientCommand.h"
#include "../../../Game/Game.h"
#include "../../../Game/DeckMaster.h"

class BetCommand : public ClientCommand {

private:
    DeckMaster *deckMaster;
    Game *game;
    std::string senderName;
    int value;

    MoveInfo exactExecute() override;
    MoveInfo handleToLowBet();
    MoveInfo handleToHighBet();

public:
    explicit BetCommand(
            ServerGameController *server,
            const ParsedMessage &parsedMessage,
            GameServiceCommandController *gameService
    );
};


#endif //MY_PROJECTS_BETCOMMAND_H

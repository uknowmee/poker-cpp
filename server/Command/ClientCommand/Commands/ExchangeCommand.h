//
// Created by micha on 23/06/2023.
//

#ifndef MY_PROJECTS_EXCHANGECOMMAND_H
#define MY_PROJECTS_EXCHANGECOMMAND_H


#include "../ClientCommand.h"

class ExchangeCommand : public ClientCommand {

private:
    DeckMaster *deckMaster;
    Game *game;
    std::string senderName;
    std::vector<int> cardsToExchange;

    void execute() override;
    void proceedExchange();
    void checkAllInCase();

    public:
    explicit ExchangeCommand(
            ServerGameController *server,
            const ParsedMessage &parsedMessage,
            GameServiceCommandController *gameService
    );
};


#endif //MY_PROJECTS_EXCHANGECOMMAND_H

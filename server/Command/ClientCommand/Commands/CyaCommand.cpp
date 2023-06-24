//
// Created by micha on 23/06/2023.
//

#include "CyaCommand.h"

CyaCommand::CyaCommand(
        ServerGameController *server,
        const ParsedMessage &parsedMessage,
        GameServiceCommandController *gameService
) : ClientCommand(server, parsedMessage, gameService) {

}

void CyaCommand::exactExecute() {
    gameService->invokeCya(parsedMessage.senderName);
}

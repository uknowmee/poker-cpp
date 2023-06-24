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

bool CyaCommand::exactExecute() {
    return gameService->invokeCya(parsedMessage.senderName);
}

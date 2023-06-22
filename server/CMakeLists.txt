cmake_minimum_required(VERSION 3.18.4)
project(server)

set(CMAKE_CXX_STANDARD 14)

add_executable(server main.cpp Connections/ClientConnection.cpp Connections/ClientConnection.h Server/Server.cpp Server/Server.h Connections/ConnectionManager.cpp Connections/ConnectionManager.h Game/Game.cpp Game/Game.h Game/Player.cpp Game/Player.h Game/Evaluator.cpp Game/Evaluator.h Game/DeckMaster.cpp Game/DeckMaster.h CustomDeck/Card.cpp CustomDeck/Card.h CustomDeck/Rank.h CustomDeck/Suit.h Server/MessageIdentifier.cpp Server/MessageIdentifier.h Server/MessagePrinter.cpp Server/MessagePrinter.h Server/ServerMessageSender.h Server/ServerCommandInvoker.h Game/GameService.cpp Game/GameService.h Server/ServerGameController.h Server/ParsedMessage.h Server/ParsedMessage.cpp CustomDeck/Class.h CustomDeck/CardMapper.cpp CustomDeck/CardMapper.h)

target_link_libraries(server pthread)
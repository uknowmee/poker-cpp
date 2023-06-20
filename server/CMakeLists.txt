cmake_minimum_required(VERSION 3.18.4)
project(server)

set(CMAKE_CXX_STANDARD 14)

add_executable(server main.cpp Connections/ClientConnection.cpp Connections/ClientConnection.h Server/Server.cpp Server/Server.h Connections/ConnectionManager.cpp Connections/ConnectionManager.h Game/Game.cpp Game/Game.h Game/Player.cpp Game/Player.h Game/Evaluator.cpp Game/Evaluator.h CustomDeck/Deck.cpp CustomDeck/Deck.h CustomDeck/Card.cpp CustomDeck/Card.h CustomDeck/Rank.h CustomDeck/Suit.h Game/GameResponse.cpp Game/GameResponse.h)

target_link_libraries(server pthread)
cmake_minimum_required(VERSION 3.18.4)
project(server)

set(CMAKE_CXX_STANDARD 14)

add_executable(
        server
        main.cpp

        Connections/ClientConnection.cpp
        Connections/ClientConnection.h
        Connections/ConnectionManager.cpp
        Connections/ConnectionManager.h

        Server/Server.cpp
        Server/Server.h

        Game/Game.cpp
        Game/Game.h
        Game/Player.cpp
        Game/Player.h
        Game/Evaluator.cpp
        Game/Evaluator.h
        Game/DeckMaster.cpp
        Game/DeckMaster.h
        Game/GameService.cpp
        Game/GameService.h
        Game/GameServiceCommandController.h

        CustomDeck/Card.cpp
        CustomDeck/Card.h
        CustomDeck/Rank.h
        CustomDeck/Suit.h
        CustomDeck/Class.h
        CustomDeck/CardMapper.cpp
        CustomDeck/CardMapper.h

        Server/MessageIdentifier.cpp
        Server/MessageIdentifier.h
        Server/MessagePrinter.cpp
        Server/MessagePrinter.h
        Server/ServerMessageSender.h
        Server/ServerCommandController.h
        Server/ServerGameController.h
        Server/ParsedMessage.h
        Server/ParsedMessage.cpp

        Command/Command.h

        Command/ServerCommand/ServerCommand.h
        Command/ServerCommand/Commands/StopCommand.cpp
        Command/ServerCommand/Commands/StopCommand.h
        Command/ServerCommand/Commands/ServerInfoCommand.cpp
        Command/ServerCommand/Commands/ServerInfoCommand.h
        Command/ServerCommand/Commands/ServerInvalidCommand.cpp
        Command/ServerCommand/Commands/ServerInvalidCommand.h
        Command/ServerCommand/ServerCommandFactory.cpp
        Command/ServerCommand/ServerCommandFactory.h

        Command/ClientCommand/ClientCommand.h
        Command/ClientCommand/ClientCommandFactory.cpp
        Command/ClientCommand/ClientCommandFactory.h
        Command/ClientCommand/Commands/ClientInfoCommand.cpp
        Command/ClientCommand/Commands/ClientInfoCommand.h
        Command/ClientCommand/Commands/FoldCommand.cpp
        Command/ClientCommand/Commands/FoldCommand.h
        Command/ClientCommand/Commands/CheckCommand.cpp
        Command/ClientCommand/Commands/CheckCommand.h
        Command/ClientCommand/Commands/CallCommand.cpp
        Command/ClientCommand/Commands/CallCommand.h
        Command/ClientCommand/Commands/AllCommand.cpp
        Command/ClientCommand/Commands/AllCommand.h
        Command/ClientCommand/Commands/CyaCommand.cpp
        Command/ClientCommand/Commands/CyaCommand.h
        Command/ClientCommand/Commands/BetCommand.cpp
        Command/ClientCommand/Commands/BetCommand.h
        Command/ClientCommand/Commands/RaiseCommand.cpp
        Command/ClientCommand/Commands/RaiseCommand.h
        Command/ClientCommand/Commands/ExchangeCommand.cpp
        Command/ClientCommand/Commands/ExchangeCommand.h
        Command/ClientCommand/Commands/ExchangeCommand.h
        Command/ClientCommand/Commands/ByeCommand.cpp Command/ClientCommand/Commands/ByeCommand.h Command/ClientCommand/Commands/HelpCommand.cpp Command/ClientCommand/Commands/HelpCommand.h Command/ClientCommand/Commands/ClientInvalidCommand.cpp Command/ClientCommand/Commands/ClientInvalidCommand.h Command/ClientCommand/ClientCommand.cpp Game/MoveInfo.h Game/MoveInfo.h)

target_link_libraries(server pthread)
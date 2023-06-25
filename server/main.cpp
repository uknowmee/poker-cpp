#include "Server/Server.h"

int main() {
    Server *server = Server::createServer(8987,4);
    server->start();
    delete server;
    return 0;
}

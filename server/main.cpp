#include "Server.h"

int main() {
    Server server = Server::createServer(8987,4);
    server.start();

    return 0;
}

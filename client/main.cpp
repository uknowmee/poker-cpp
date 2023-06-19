#include "Client.h"

int main() {
    const char *hostname = "127.0.0.1";
    int port = 8987;

    Client client = Client::createClient(hostname, port);
    client.start();

    return 0;
}

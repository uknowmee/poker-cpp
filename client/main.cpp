#include "Client.h"

int main() {
    const char *hostname = "129.152.23.234";
    int port = 7123;

    Client client = Client::createClient(hostname, port);
    client.start();

    return 0;
}

//
// Created by micha on 19/06/2023.
//

#include "Client.h"

Client Client::createClient(const char *hostname, int port) {
    return {hostname, port};
}

Client::Client(const char *hostname, int port)
        : hostname(hostname), port(port) {
    this->client_socket = 0;
    this->server = {};
}

void Client::start() {
    createSocket();
    createServer();
    connectToServer();

    try {
        startThreads();
    } catch (const std::exception &e) {
        close(this->client_socket);
    }
}

void Client::createSocket() {
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::throw_with_nested(std::runtime_error("Error creating socket."));
    }
}

void Client::createServer() {
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    if (inet_pton(AF_INET, hostname, &(server.sin_addr)) <= 0) {
        std::throw_with_nested(std::runtime_error("Invalid address/Address not supported."));
    }
}

void Client::connectToServer() {
    if (connect(client_socket, reinterpret_cast<struct sockaddr *>(&server), sizeof(server)) < 0) {
        std::throw_with_nested(std::runtime_error("Error connecting to the server."));
    }
}

void Client::startThreads() {
    pthread_t receiveThread, sendThread;
    pthread_create(&receiveThread, nullptr, receive_message, &client_socket);
    pthread_create(&sendThread, nullptr, send_message, &client_socket);

    pthread_join(receiveThread, nullptr);
    pthread_join(sendThread, nullptr);
}

void *Client::receive_message(void *arg) {
    int client_socket = *(static_cast<int *>(arg));
    char buffer[1024];

    while (true) {
        ssize_t num_bytes = recv(client_socket, buffer, sizeof(buffer), 0);
        if (num_bytes <= 0) {
            exit(1);
        }
        buffer[num_bytes] = '\0';
        std::cout << buffer << std::endl;

        if (strcmp(buffer, "Server is closing...") == 0) {
            exit(1);
        }

        if (strcmp(buffer, "You are disconnecting. Cya later!") == 0) {
            exit(1);
        }
    }
}

void *Client::send_message(void *arg) {
    int client_socket = *(static_cast<int *>(arg));
    char buffer[1024];

    while (true) {
        std::cin.getline(buffer, sizeof(buffer));

        if (strlen(buffer) == 0) {
            continue;
        }

        ssize_t num_bytes = send(client_socket, buffer, strlen(buffer), 0);
        if (num_bytes <= 0) {
            exit(1);
        }
    }
}

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

void Client::execute() {
    createSocket();
    createServer();
    connectToServer();

    startThreads();

    close(this->client_socket);
}

void Client::createSocket() {
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "Error creating socket." << std::endl;
        return;
    }
}

void Client::createServer() {
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    if (inet_pton(AF_INET, hostname, &(server.sin_addr)) <= 0) {
        std::cerr << "Invalid address/Address not supported." << std::endl;
        return;
    }
}

void Client::connectToServer() {
    if (connect(client_socket, reinterpret_cast<struct sockaddr *>(&server), sizeof(server)) < 0) {
        std::cerr << "Error connecting to the server." << std::endl;
        return;
    }
}

void Client::startThreads() {
    pthread_t receiveThread, sendThread;
    pthread_create(&receiveThread, nullptr, receive_messages, &client_socket);
    pthread_create(&sendThread, nullptr, send_message, &client_socket);

    pthread_join(receiveThread, nullptr);
    pthread_join(sendThread, nullptr);
}

void *Client::receive_messages(void *arg) {
    int client_socket = *(static_cast<int *>(arg));
    char buffer[1024];

    while (true) {
        ssize_t num_bytes = recv(client_socket, buffer, sizeof(buffer), 0);
        if (num_bytes <= 0) {
            std::cerr << "Error occurred while receiving messages from the server." << std::endl;
            break;
        }
        buffer[num_bytes] = '\0';
        std::cout << "Server: " << buffer << std::endl;
    }

    pthread_exit(nullptr);
}

void *Client::send_message(void *arg) {
    int client_socket = *(static_cast<int *>(arg));
    char buffer[1024];

    while (true) {
        std::cin.getline(buffer, sizeof(buffer));
        ssize_t num_bytes = send(client_socket, buffer, strlen(buffer), 0);
        if (num_bytes <= 0) {
            std::cerr << "Error occurred while sending the message to the server." << std::endl;
            break;
        }
    }

    pthread_exit(nullptr);
}

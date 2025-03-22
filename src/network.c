#include "../include/network.h"

int create_socket_descriptor() {
    return socket(AF_INET, SOCK_STREAM, 0);
}

int resolve_hostname(const char* host, struct addrinfo** res) {
    struct addrinfo hints;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    return getaddrinfo(host, NULL, &hints, res);
}
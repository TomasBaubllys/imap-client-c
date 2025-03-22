#include "../include/debug.h"

void get_host_ipv4(struct addrinfo *addst, char *res, size_t res_size) {
    inet_ntop(AF_INET, &((struct sockaddr_in*)addst -> ai_addr) -> sin_addr, res, res_size);
}
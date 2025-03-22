#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#define _XOPEN_SOURCE 700 // what the fuck???

#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/socket.h>

int create_socket_descriptor();

int resolve_hostname(const char* host, struct addrinfo** res);

#endif // NETWORK_H_INCLUDED
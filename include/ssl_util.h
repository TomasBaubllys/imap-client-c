#ifndef SSL_UTIL_H_INCLUDED
#define SSL_UTIL_H_INCLUDED

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <unistd.h>

void init_openssl();

SSL_CTX* create_ssl_context();

SSL* create_ssl_connection(SSL_CTX* ctx, int sockfd);

#endif // SSL_UTIL_H_INCLUDED

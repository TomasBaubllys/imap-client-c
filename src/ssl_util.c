#include "../include/ssl_util.h"

void init_openssl() {
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();
}

SSL_CTX* create_ssl_context() {
    SSL_CTX* ctx = SSL_CTX_new(TLS_client_method());

    if(!ctx) {
        perror("SSL_CTX_new failed\n");
        exit(1);
    }

    return ctx;
}

SSL* create_ssl_connection(SSL_CTX* ctx, int sockfd) {
    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sockfd);

    if(SSL_connect(ssl) != 1) {
        perror("SSL connection failed\n");
        SSL_free(ssl);
        close(sockfd);
        SSL_CTX_free(ctx);
        exit(1);
    }

    return ssl;
}
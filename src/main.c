/*
TODO :
1) Fix error handling
2) Add -h function
*/

#define _XOPEN_SOURCE 700 // what the fuck???

#include "../include/network.h"
#include "../include/colours.h"
#include <stdio.h>
#include "../include/ssl_util.h"
#include "../include/debug.h"
#include "../include/imap.h"

#define SERVER_INFO_DEBUG
#define INVALID_PROGRAM_ARG "Invalid argument count: must be ./imap <server> or ./imap -h(NOT IMPLEMENTED YET)\n"
#define ERR_MEM_ALLOC "Memory allocation failed\n"
#define ERR_SOCKET_CREATION "Socket creation failed\n"
#define ERR_HOSTNAME_CANNOT_BE_RESOLVED "Provide hostname couldn`t be resolved\n"
#define ERR_CONNECTION_FAILED "Connection failed\n"
#define ERR_READING_DATA "Error occured while reading the data: " 

#define SERVER_SIDE_MSG "Server: "
#define CLIENT_SIDE_MSG "> " 

int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr, INVALID_PROGRAM_ARG);
        exit(1);
    }

    int sockfd;
    struct sockaddr_in server_addr;
    struct addrinfo* res;
    SSL_CTX *ctx;
    SSL *ssl;
    char buffer[MAX_REQ_LEN];

    char tag[TAG_LENGTH];  
    char tag_good[TAG_LENGTH + RESPONSE_LENGTH_OK_BAD];
    char tag_bad[TAG_LENGTH + RESPONSE_LENGTH_OK_BAD];

    char* res_buffer = calloc(MAX_RESPONSE_SIZE, sizeof(char));
    if(!res_buffer) {
        perror(ERR_MEM_ALLOC);
    }

    #ifdef SERVER_INFO_DEBUG
        char ip_str[INET_ADDRSTRLEN];
    #endif // SERVER_INFO_DEBUG

    init_openssl();
    ctx = create_ssl_context();

    sockfd = create_socket_descriptor();
    if (sockfd < 0) {
        perror(ERR_SOCKET_CREATION);
        SSL_CTX_free(ctx);
        return 1;
    }

    if(resolve_hostname(argv[1], &res) != 0) {
        perror(ERR_HOSTNAME_CANNOT_BE_RESOLVED);
        close(sockfd);
        SSL_CTX_free(ctx);
        return 1;
    }

    #ifdef SERVER_INFO_DEBUG
        get_host_ipv4(res, ip_str, sizeof(ip_str));
        printf("Connecting to: %s\n", ip_str);
    #endif // SERVER_INFO_DEBUG

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(IMAP_PORT);
    memcpy(&server_addr.sin_addr, &((struct sockaddr_in*)res->ai_addr)->sin_addr, sizeof(struct in_addr));
    
    freeaddrinfo(res);

    // Connect to the IMAP server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror(RED ERR_CONNECTION_FAILED RESET);
        close(sockfd);
        SSL_CTX_free(ctx);
        free(res_buffer);
        return 1;
    }

    // Create an SSL connection
    ssl = create_ssl_connection(ctx, sockfd);

    printf("Connected to %s over SSL\n", argv[1]);

    // Read IMAP server response
    memset(buffer, 0, sizeof(buffer));
    SSL_read(ssl, res_buffer, MAX_RESPONSE_SIZE - 1);
    printf(GREEN SERVER_SIDE_MSG "%s\n" RESET, buffer);

    while (1) {
        // Read input from stdin
        printf(YELLOW CLIENT_SIDE_MSG RESET);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input.\n");
            break;
        }

        if(send_IMAP_req(ssl, buffer, tag) <= 0) {
            break;
        }
 
        // Read the server's response
        int res_counter = 0;
        while(1) {
            if(res_counter == 0) {
                strcat(tag_good, SERVER_RES_OK);
                strcat(tag_bad, SERVER_RES_BAD);
                ++res_counter;
            }

            int bytes_read = rec_IMAP_res(ssl, res_buffer);
            if (bytes_read <= 0) {
                fprintf(stderr, ERR_READING_DATA "%s\n", ERR_error_string(ERR_get_error(), NULL));
                break;
            }

            res_buffer[bytes_read] = '\0';
            printf(GREEN SERVER_SIDE_MSG RESET "%s\n", res_buffer);

            if (strstr(res_buffer, tag_good) || strstr(res_buffer, tag_bad)) {
                break;
            }
        }

        // replace withs macros later!!
        memset(tag_good, 0, sizeof(tag_good));
        memset(tag_bad, 0, sizeof(tag_bad));
    }

    // 8. Cleanup
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(sockfd);
    SSL_CTX_free(ctx);
    free(res_buffer);

    return 0;
}

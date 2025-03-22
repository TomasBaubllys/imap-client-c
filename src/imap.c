#include "../include/imap.h"

// used as a fail safe if client tries to send more than 1 messege a second
static int _imap_tag_counter = 0;

int create_unique_tag(char *res) {
    if(!res) {
        fprintf(stderr, ERR_NULL_REQ);
        return - 1;
    }

    snprintf(res, TAG_LENGTH, "T%ld%d ", time(NULL), _imap_tag_counter);
    update_counter();

    #ifdef IMAP_TAG_DEBUG
        printf("Current tag: %s\n", res);
    #endif // IMAP_DEBUG

    return 0;
}

int add_tag(char *req, char *what_tag) {
    if(!req) {
        fprintf(stderr, ERR_NULL_REQ);
        return -1;
    }

    char *tag = calloc(MAX_REQ_LEN, sizeof(char));
    if(!tag) {
        perror(ERR_TIMESTAMP_MEM_ALLOC);
        return -1;
    }

    create_unique_tag(tag);

    if(what_tag) {
        strcpy(what_tag, tag);
    }

    strcat(tag, req);
    // optimize later, maybe just swap the pointers? make sure then that req is dynamic lolzzz
    strcpy(req, tag);

    #ifdef IMAP_FULL_REQ_PRINT
        printf("%s\n", req);
    #endif // IMAP_FULL_REQ_PRINT

    free(tag);

    return 0;
}

void update_counter() {
    _imap_tag_counter >= 99? _imap_tag_counter = 0 : ++_imap_tag_counter;
}

int add_rn_to_req(char* req) {
    if(!req) {
        fprintf(stderr, ERR_NULL_REQ);
        return -1;
    }

    int len = strlen(req);
    if(len > 0 && req[len - 1] == '\n') {
        req[len - 1] = '\r';
        req[len] = '\n';
        req[len + 1] = '\0';

        return 0;
    }

    return -1;
}

int send_IMAP_req(SSL* ssl, char* req, char *tag) {
    if(!ssl) {
        fprintf(stderr, ERR_NULL_SSL);
        return -1;
    }

    if(!req) {
        fprintf(stderr, ERR_NULL_REQ);
        return -1;
    }

    add_rn_to_req(req);
    if(add_tag(req, tag) != 0) {
        fprintf(stderr, ERR_GENERAL_TAG);
        return -1;
    }    

    int ssl_write_ret = SSL_write(ssl, req, strlen(req));

    if(ssl_write_ret <= 0) {
        fprintf(stderr, ERR_FAILED_SSL_WRITE "%d\n", SSL_get_error(ssl, ssl_write_ret));
    }

    return ssl_write_ret;
}

int rec_IMAP_res(SSL* ssl, char* res) {
    if(!ssl) {
        fprintf(stderr, ERR_NULL_SSL);
        return -1;
    }

    if(!res) {
        fprintf(stderr, ERR_NULL_RES_BUFFER);
        return -1;
    }

    int bytes_read = SSL_read(ssl, res, MAX_RESPONSE_SIZE - 1);
    if(bytes_read <= 0) {
        fprintf(stderr, ERR_RECEIVING_DATA "%d\n", SSL_get_error(ssl, bytes_read));
    }

    return bytes_read;
}
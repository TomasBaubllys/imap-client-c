#ifndef IMAP_H_INCLUDED
#define IMAP_H_INCLUDED

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <openssl/err.h>
#include <openssl/ssl.h>

#define IMAP_PORT 993

#define TAG_LENGTH 24
#define RESPONSE_LENGTH_OK_BAD 4
#define MAX_REQ_LEN 1024
#define MAX_REQ_USABLE_LEN 1000

#define MAX_RESPONSE_SIZE 4096 // 4kb

#define ERR_TIMESTAMP_MEM_ALLOC "Failed to allocate the memory for a timestamp\n"
#define ERR_GENERAL_TAG "Failed to add a tag to the request\n"
#define ERR_NULL_REQ "A request cannot be NULL\n"
#define ERR_NULL_SSL "SSL structure passed to a function call was NULL\n"
#define ERR_FAILED_SSL_WRITE "Failed to write the request: "
#define ERR_NULL_RES_BUFFER "Response buffer cannot be NULL\n"
#define ERR_RECEIVING_DATA "Error reading data: "

#define SERVER_RES_OK "OK"
#define SERVER_RES_BAD "BAD"

// #define IMAP_TAG_DEBUG
// #define IMAP_FULL_REQ_PRINT

// create a unique tag for request 
int create_unique_tag(char* res);

// concats a tag to to the current request req 0 - if success, -1 - if error
// if what_tag is not null, stores the current tag in what tag
int add_tag(char* req, char* what_tag);

// adds "\r\n" to the end of the request
int add_rn_to_req(char* req);

// updates the internal counter
void update_counter();

// wraps the given request in needed arguments, sends it to the server bytes written - succes -1 error
// if tag is not NULL it will store the current request tag
int send_IMAP_req(SSL* ssl, char* req, char *tag);

// a function that waits and receive a request response from the server
int rec_IMAP_res(SSL* ssl, char* res);



#endif // IMAP_H_INCLUDED
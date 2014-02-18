#ifndef CLIENT_HEAD
#define CLIENT_HEAD

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_PORT "27993"
#define MAX_BUFFER 256

/*
 * gets the struct addrinfo for the designated node
 */
struct addrinfo *setupAddrList(char *node, char *port);

/*
 * Initializes the socket and connects it to the provided node
 */
int openConnection(char *node, char *port);

/*
 * I know this function is obnoxiously long and verbose, but such is
 * my knowledge of string parsing in C
 *
 * The purpose of this function is to comply with the protocol described
 * on the project page for this assignment
 */
char *doMath(char *buf);

/*
 * Sets everything up for the while(1) loop that accepts requests until
 * it receives the final BYE message
 */
void makeSession(char *node, char *port, char *HELLO_MESSAGE, int str_size);

#endif

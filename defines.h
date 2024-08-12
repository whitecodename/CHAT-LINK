#ifndef DEFINES_H
#define DEFINES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdarg.h>
#include <netdb.h>
#include <poll.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/socket.h>

#define LISTENING_PORT 5094
#define MAX_CLIENTS 10
#define BUF_SIZE_1 64
#define BUF_SIZE_2 1024
#define BUF_SIZE_3 (BUF_SIZE_2 * 2)
#define START_POLL_SIZE (1 + 5)
#define CONNECTION_HOST "127.0.0.1"
#define POLL_TIME 2000

typedef enum {
    SERVER, CLIENT
} HOST;

typedef enum {
    false, true
} bool;

typedef enum {
    ERROR = -1, SUCCESS
} Status;

typedef struct {
    int fd;
    int port;
    char err_msg[BUF_SIZE_1];
} Server;

typedef struct {
    int fd;
    char name[BUF_SIZE_1];
    char msg[BUF_SIZE_2];
} Client;

typedef struct {
    int size, active_size;
    struct pollfd *fds;
    Client *clients;
} Hosts;

#include "utils.h"
#include "socket.h"

#endif // DEFINES_H
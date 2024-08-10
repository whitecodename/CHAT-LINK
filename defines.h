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
#define BUF_SIZE_1 1024
#define BUF_SIZE_2 (BUF_SIZE_1 * 2)
#define START_POLL_SIZE (1 + 5)
#define CONNECTION_HOST "127.0.0.1"

typedef enum {
    SERVER, CLIENT
} HOST;

typedef enum {
    false, true
} bool;

typedef struct {
    int socket_fd;
    int port;
} Server;

typedef struct {
    struct pollfd *fds;
    int fds_size, fds_active_size;
} Fds;

#include "utils.h"
#include "socket.h"

#endif // DEFINES_H
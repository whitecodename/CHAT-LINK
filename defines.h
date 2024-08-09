#ifndef DEFINES_H
#define DEFINES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LISTENING_PORT 5094
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

#define CONNECTION_HOST "127.0.0.1"

typedef enum {
    SERVER, CLIENT
} HOST;

#endif // DEFINES_H
#ifndef UTILS_H
#define UTILS_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>

#include "defines.h"

void redirect_to_error(HOST host, char *message);
int create_socket(void);
int send_message(int connected_socket_fd, char *message);
int recv_message(int connected_socket_fd, char *message);
int readString(char *buffer, int buffer_size);

#endif // UTILS_H
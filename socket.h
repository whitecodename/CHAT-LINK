#ifndef SOCKET_H
#define SOCKET_H

#include "defines.h"

int do_connect(int socket_fd, struct sockaddr *socket_addr);
int do_bind(int socket_fd, struct sockaddr *socket_addr);
int send_message(int socket_fd, char *message);
int recv_message(int socket_fd, char *buffer);
int init_server_socket(const int port);

#endif // SOCKET_H
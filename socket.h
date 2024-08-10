#ifndef SOCKET_H
#define SOCKET_H

#include "defines.h"

int do_connect(int socket_fd, struct sockaddr *socket_addr);
int do_bind(int socket_fd, struct sockaddr *socket_addr);
int send_message(int socket_fd, char *message);
int recv_message(int socket_fd, char *buffer);
void init_server_socket(Server *server, int argc, char *argv[]);
int init_fds(Fds *fds, Server *server);
int fd_is_ready(struct pollfd *fd);
Client* accept_client(Server *server, Fds *fds);
void resize_fds(Fds *fds);
void append_client(Fds *fds, Client *client);
int read_client_message(Server *server, Fds *fds, struct pollfd *fd);
void diffuse_message(Server *server, Fds *fds, int sender_fd, char msg[BUF_SIZE_3]);
void init_socket_address(struct sockaddr_in *socket_addr);

#endif // SOCKET_H
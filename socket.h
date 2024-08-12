#ifndef SOCKET_H
#define SOCKET_H

#include "defines.h"

#include "socket.h"

int do_connect(int socket_fd, struct sockaddr *socket_addr);
int do_bind(int socket_fd, struct sockaddr *socket_addr);
int send_msg(int socket_fd, char *message);
int recv_msg(int socket_fd, char *buffer, int size);
Status init_server_port(Server *server, int argc, char *argv[]);
Status init_server_socket(Server *server, int argc, char *argv[]);
void init_socket_address(struct sockaddr_in *socket_addr, int port);
Status init_fds(Hosts *hosts, Server *server);
bool fd_is_ready(struct pollfd *poll_fd);
bool fd_is_ended(struct pollfd *poll_fd);
Client* get_client(Hosts *hosts, int i);
Client* find_client(Hosts *hosts, int fd);
Status accept_client(Server *server, Hosts *hosts);
void resize_fds(Hosts *hosts);
Client* append_client(Hosts *hosts, int client_fd);
int read_client_message(Server *server, Hosts *hosts, struct pollfd *poll_fd, int i);
Status diffuse_message(Server *server, Hosts *hosts, int sender_fd, char msg[BUF_SIZE_3]);
void free_hosts(Hosts *hosts);

#endif // SOCKET_H
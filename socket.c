#include "socket.h"

int do_connect(int socket_fd, struct sockaddr *socket_addr)
{
    return connect(socket_fd, socket_addr, sizeof(*socket_addr));
}

int do_bind(int socket_fd, struct sockaddr *socket_addr)
{
    return bind(socket_fd, socket_addr, sizeof(*socket_addr));
}

int send_msg(int socket_fd, char *message)
{
    if(socket_fd == -1)
        return -1;
    else
        return send(socket_fd, message, strlen(message), 0);
}

int recv_msg(int socket_fd, char *buffer, int size)
{
    if(socket_fd == -1)
        return -1;
    else
        return recv(socket_fd, buffer, size, 0);
}

Status init_server_port(Server *server, int argc, char *argv[])
{
    if (argc < 2) {
        return ERROR;
    }

    server->port = atoi(argv[1]);

    return SUCCESS;
}

Status init_server_socket(Server *server, int argc, char *argv[])
{
    /* Get port from command line */
    if (init_server_port(server, argc, argv) == ERROR) {
        strcpy(server->err_msg, "Usage: ./server <PORT>");
        return ERROR;
    }

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == ERROR) {
        strcpy(server->err_msg, "Failed to create socket");
        return ERROR;
    }

    server->fd = socket_fd;

    /* Initializing socket address */
    struct sockaddr_in socket_addr = {};
    init_socket_address(&socket_addr, server->port);

    /* Binding socket to socket address */
    if(do_bind(socket_fd, (struct sockaddr*)&socket_addr) == ERROR) {
        strcpy(server->err_msg, "Failed to bind socket");
        return ERROR;
    }

    /* Listening on socket */
    if (listen(socket_fd, MAX_CLIENTS) == ERROR) {
        strcpy(server->err_msg, "Failed to listen on socket");
        return ERROR;
    }

    return SUCCESS;
}

void init_socket_address(struct sockaddr_in *socket_addr, int port)
{
    socket_addr->sin_family = AF_INET;
    socket_addr->sin_port = htons(port);
    socket_addr->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
}

/* Initialize fds structure */
Status init_fds(Hosts *hosts, Server *server)
{
    /* Initialize clients array */
    hosts->clients = calloc(START_POLL_SIZE, sizeof(Client));
    if (hosts->clients == NULL) {
        return ERROR;
    }
    
    /* Initialize fds array */
    struct pollfd **poll_fds = &hosts->fds;

    *poll_fds = calloc(START_POLL_SIZE, sizeof(*poll_fds));
    if (*poll_fds == NULL) {
        return ERROR;
    }

    /* Initialize hosts fds size */
    hosts->size = START_POLL_SIZE;

    /* Initialize hosts fds active_size */
    (*poll_fds)[0].fd = server->fd;
    (*poll_fds)[0].events = POLLIN;
    hosts->active_size = 1;

    return SUCCESS;
}

bool fd_is_ready(struct pollfd *poll_fd) {
    return (poll_fd->revents & POLLIN) != 0;
}

Client* get_client(Hosts *hosts, int i) {
    return &hosts->clients[i];
}

Client* find_client(Hosts *hosts, int fd) {
    for (int i = 0; i < hosts->active_size - 1; i++) {
        Client *client = get_client(hosts, i);
        if (fd == client->fd) {
            return client;
        }
    }
    return NULL;
}

Status accept_client(Server *server, Hosts *hosts)
{
    int client_fd = accept(server->fd, NULL, NULL);
    if (client_fd == ERROR) {
        strcpy(server->err_msg, "Failed to accept connection");
        return ERROR;
    }

    if (hosts->active_size >= hosts->size) {
        resize_fds(hosts);
    }

    Client *client = append_client(hosts, client_fd);
    char buffer[BUF_SIZE_2];
    
    server_message("New client: %d", client->fd);
    sprintf(buffer, "Welcome [%d]\n", client->fd);

    int status = send_msg(client->fd, buffer);
    if (status == ERROR) {
        strcpy(server->err_msg, "Failed to send message");
        return ERROR;
    }

    return SUCCESS;
}

void resize_fds(Hosts *hosts) {
    hosts->size *= 2;
    hosts->fds = realloc(hosts->fds, sizeof(*(hosts->fds)) * hosts->size);
    hosts->clients = realloc(hosts->clients, sizeof(*(hosts->clients)) * hosts->size);
}

/* Append client in Hosts structure */
Client* append_client(Hosts *hosts, int client_fd)
{
    int active_size = hosts->active_size;

    /* Initialize client in Client structure */
    Client *client = get_client(hosts, active_size - 1);
    client->fd = client_fd;

    /* Initialize client in poll_fd structure */
    struct pollfd *fd = &hosts->fds[active_size]; // Last element
    fd->fd = client_fd;
    fd->events = POLLIN;

    /* Update active_size */
    hosts->active_size++;

    return client;
}

/* Read client message */
int read_client_message(Server *server, Hosts *hosts, struct pollfd *poll_fd, int i)
{
    char msg_to_send[BUF_SIZE_3] = {};
    int read_count;
    Status status;

    Client *sender = find_client(hosts, poll_fd->fd);

    if (!sender) {
        sprintf(server->err_msg, "Failed to find client %d", poll_fd->fd);
        return ERROR;
    }

    read_count = recv_msg(sender->fd, sender->msg, sizeof sender->msg);

    /* Check for errors */
    if (read_count <= 0) {
        if (read_count == ERROR) {
            server_message("Failed to read message from client %d", sender->fd);
        } else {
            server_message("Client %d closed connection", sender->fd);
        }

        /* Close client socket */
        close(sender->fd);

        /* Remove client from Hosts structure */
        struct pollfd *last_poll_fd = &hosts->fds[hosts->active_size - 1];
        
        hosts->fds[i] = *last_poll_fd;

        Client *last_client = find_client(hosts, last_poll_fd->fd);

        /* Update last client in Hosts structure */
        sender->fd = last_client->fd;
        strcpy(sender->name, last_client->name);
        strcpy(sender->msg, last_client->msg);

        hosts->active_size--;

        return ERROR;
    }
    
    /* Send message to all clients */
    server_message("Client %d message: %s", sender->fd, sender->msg);

    sprintf(msg_to_send, "[%d]: %s", sender->fd, sender->msg);
    
    
    status = diffuse_message(server, hosts, sender->fd, msg_to_send);

    return status;
}

/* Diffuse message to all clients except sender */
Status diffuse_message(Server *server, Hosts *hosts, int sender_fd, char msg[BUF_SIZE_3])
{
    int client_fd, status;

    /* Send message to all clients except sender */
    for(int j = 0; j < hosts->active_size; j++) {
        fprintf(stderr, "diffuse_message: for loop\n");
        client_fd = hosts->fds[j].fd;

        /* Skip sender and server */
        if (client_fd == server->fd || client_fd == sender_fd) {
            continue;
        }
        
        /* Send message */
        status = send_msg(client_fd, msg);
        if (status == ERROR) {
            strcpy(server->err_msg, "Failed to send message");
            return status;
        }
    }

    return SUCCESS;
}

void free_hosts(Hosts *hosts)
{
    free(hosts->fds);
    free(hosts->clients);
}
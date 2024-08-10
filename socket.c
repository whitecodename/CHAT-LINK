#include "socket.h"

int do_connect(int socket_fd, struct sockaddr *socket_addr)
{
    return connect(socket_fd, socket_addr, sizeof(*socket_addr));
}

int do_bind(int socket_fd, struct sockaddr *socket_addr)
{
    return bind(socket_fd, socket_addr, sizeof(*socket_addr));
}

/**
 * @brief Send a message to a socket.
 * 
 * @param socket_fd The file descriptor for the socket to send data to.
 * @param message Message to send
 * 
 * @return The number of bytes received on success, or -1 on error.
 */
int send_message(int socket_fd, char *message)
{
    if(socket_fd == -1)
        return -1;
    else
        return send(socket_fd, message, strlen(message), 0);
}

/**
 * @brief Receives a message from a socket and stores it in a buffer.
 *
 * @param socket_fd The file descriptor for the socket to receive data from.
 * @param buffer A pointer to a character array where the received message will be stored.
 *
 * @return The number of bytes received on success, or -1 on error.
 */
int recv_message(int socket_fd, char *buffer)
{
    if(socket_fd == -1)
        return -1;
    else
        return recv(socket_fd, buffer, strlen(buffer), 0);
}

void init_server_socket(Server *server, int argc, char *argv[])
{
    /* Get port from command line */
    if (argc < 2) {
        server_redirect_to_error("Usage: ./server <PORT>\n");
    }
    server->port = atoi(argv[1]);

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        server_redirect_to_error("Failed to create socket");
    }

    /* Initializing socket address */
    struct sockaddr_in socket_addr;

    memset(&socket_addr, 0, sizeof(socket_addr));

    socket_addr.sin_family = AF_INET;
    socket_addr.sin_port = htons(server->port);
    socket_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    /* Binding socket to socket address */
    if(do_bind(socket_fd, (struct sockaddr*)&socket_addr) == -1) {
        redirect_to_error(SERVER, "Failed to bind socket");
    }

    /* Listening on socket */
    if (listen(socket_fd, MAX_CLIENTS) == -1) {
        redirect_to_error(SERVER, "Failed to listen on socket");
    }

    server->fd = socket_fd;
}

int init_fds(Fds *fds, Server *server)
{    
    fds->size = START_POLL_SIZE;

    struct pollfd **poll_fds = &fds->fds;
    
    *poll_fds = calloc(fds->size, sizeof(*poll_fds));
    if (*poll_fds == NULL) {
        return -1;
    }

    (*poll_fds)[0].fd = server->fd;
    (*poll_fds)[0].events = POLLIN;
    fds->active_size = 1;

    return 0;
}

int fd_is_ready(struct pollfd *fd) {
    return (fd->revents & POLLIN) == 1 ? 0 : -1;
}

Client* accept_client(Server *server, Fds *fds)
{
    Client *client = calloc(1, sizeof(*client));
    if (client == NULL) {
        return NULL;
    }

    client->fd = accept(server->fd, NULL, NULL);
    if (client->fd == -1) {
        server_redirect_to_error("Failed to accept connection");
    }

    if (fds->active_size >= fds->size) {
        resize_fds(fds);
    }

    append_client(fds, client);
    server_message("New client: %d", client->fd);

    sprintf(client->msg, "Welcome [%d]\n", client->fd);

    int status = send(client->fd, client->msg, strlen(client->msg), 0);
    if (status == -1) {
        server_redirect_to_error("Failed to send message");
    }

    return client;
}

void resize_fds(Fds *fds) {
    fds->size *= 2;
    fds->fds = realloc(fds->fds, sizeof(*(fds->fds)) * fds->size);
}

void append_client(Fds *fds, Client *client)
{
    struct pollfd *fd = &fds->fds[fds->active_size++];

    fd->fd = client->fd;
    fd->events = POLLIN;
}

int read_client_message(Server *server, Fds *fds, struct pollfd *fd)
{
    char buffer[BUF_SIZE_1] = {};
    char msg_to_send[BUF_SIZE_2] = {};
    int read_count, status, sender_fd, dest_fd;

    sender_fd = fd->fd;
    read_count = recv(sender_fd, buffer, sizeof buffer, 0);

    if (read_count <= 0) {
        if (read_count == -1) {
            server_message("Failed to read message from client %d", sender_fd);
        } else {
            server_message("Client %d closed connection", sender_fd);
        }

        close(sender_fd);
        fds->active_size--;

        return -1;
    } else {
        server_message("Got message from client %d: %s", sender_fd, buffer);

        sprintf(msg_to_send, "[%d] says: %s", sender_fd, buffer);

        for(int j = 0; j < fds->active_size; j++) {
            dest_fd = fds->fds[j].fd;

            if (dest_fd != server->fd && dest_fd != sender_fd) {
                status = send(dest_fd, msg_to_send, strlen(msg_to_send), 0);
                if (status == -1) {
                    server_redirect_to_error("Failed to send message");
                }
            }
        }

    }

    return 0;
}
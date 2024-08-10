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

int init_server_socket(const int port)
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        redirect_to_error(SERVER, "Failed to create socket");
    }

    /* Initializing socket address */
    struct sockaddr_in socket_addr;

    memset(&socket_addr, 0, sizeof(socket_addr));

    socket_addr.sin_family = AF_INET;
    socket_addr.sin_port = htons(port);
    socket_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    /* Binding socket to socket address */
    if(do_bind(socket_fd, (struct sockaddr*)&socket_addr) == -1) {
        redirect_to_error(SERVER, "Failed to bind socket");
    }

    /* Listening on socket */
    if (listen(socket_fd, MAX_CLIENTS) == -1) {
        redirect_to_error(SERVER, "Failed to listen on socket");
    }

    return socket_fd;
}
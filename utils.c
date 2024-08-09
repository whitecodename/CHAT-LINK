#include "utils.h"

/**
 * @brief Creates a socket in AF_INET domain using socket stream type with TCP protocol.
 * 
 * @return socket file descriptor
 */
int create_socket()
{
    return socket(AF_INET, SOCK_STREAM, 0);
}

/**
 * @brief Prints the error message preceded with the hostname label and exit with 1 status.
 * 
 * @param host SERVER or CLIENT
 * @param message Error message
 */
void redirect_to_error(HOST host, char *message)
{
    char *hostname = (host == SERVER) ? "SERVER" : "CLIENT" ;

    fprintf(stderr, "%s: %s\n", hostname, message);
    exit(1);
}

int do_connect(int socket_fd, struct sockaddr *socket_addr)
{
    return connect(socket_fd, socket_addr, sizeof(*socket_addr));
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
        redirect_to_error(CLIENT, "Failed to create socket");
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
        redirect_to_error(CLIENT, "Failed to create socket");
    else
        return recv(socket_fd, buffer, BUFFER_SIZE, 0);
}

void clearBuffer()
{
    int c = 0;
    while (c != '\n' && c != EOF) c = getchar();
}
 
/**
 * @brief Read string from stdin and store it in a buffer.
 *
 * @param buffer The buffer to store the read string.
 * @param buffer_size The size of the buffer.
 *
 * @return 1 on success, 0 on error.
 */ 
int readString(char *buffer, int buffer_size)
{
    char *enter_position = NULL;
 
    if (fgets(buffer, buffer_size, stdin) == NULL)
    {
        clearBuffer();
        return 0;
    }

    enter_position = strchr(buffer, '\n');
    
    enter_position != NULL
        ? *enter_position = '\0'
        : clearBuffer();

    return 1;
}
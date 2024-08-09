#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils.h"

int main (int argc, char *argv[])
{
    /* Creating socket */
    int socket_fd = create_socket();

    if(socket_fd == -1)
    {
        redirect_to_error(CLIENT, "Failed to create socket");
    }

    /* Connecting to server */
    struct sockaddr_in socket_addr;

    socket_addr.sin_family = AF_INET;
    socket_addr.sin_port = LISTENING_PORT;

    int inet_return_code = inet_pton(AF_INET, CONNECTION_HOST, &socket_addr.sin_addr);
    
    if(inet_return_code == -1)
    {
        redirect_to_error(CLIENT, "Failed to convert address\n");
    }

    /* Connecting socket to socket address */
    int socket_addr_len = sizeof(socket_addr);
    int connect_return = do_connect(socket_fd, &socket_addr);

    if(connect_return == -1)
    {
        redirect_to_error(CLIENT, "Failed to connect\n");
    }

    while(1)
    {
        char message[BUFFER_SIZE] = {};

        printf("You:> ");
        readString(message, BUFFER_SIZE);

        int send_bytes = send(socket_fd, message, strlen(message), 0);

        if(send_bytes == -1)
        {
            redirect_to_error(CLIENT, "Failed to send data\n");
        }
    }

    close(socket_fd);

    return 0;
}
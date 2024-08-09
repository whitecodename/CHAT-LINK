#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils.h"

int main (int argc, char *argv[])
{
    int socket_fd = create_socket();

    if(socket_fd == -1)
    {
        redirect_to_error(SERVER, "Failed to create socket");
    }

    /* Initializing socket address */
    struct sockaddr_in socket_addr;

    socket_addr.sin_family = AF_INET;
    socket_addr.sin_port = LISTENING_PORT;
    socket_addr.sin_addr.s_addr = INADDR_ANY;

    /* Binding socket to socket address */
    int socket_addr_len = sizeof(socket_addr);
    int bind_return = bind(socket_fd, (struct sockaddr*)&socket_addr, socket_addr_len);

    int do_bind(int socket_fd, struct sockaddr *socket_addr)
    {
        return connect(socket_fd, socket_addr, sizeof(*socket_addr));
    }
    
    if(bind_return == -1)
    {
        redirect_to_error(SERVER, "Failed to bind socket");
    }

    /* Listening on socket */
    int listen_return = listen(socket_fd, MAX_CLIENTS);
    
    if(listen_return == -1)
    {
        redirect_to_error(SERVER, "Failed to listen on socket");
    }
        
    printf("SERVER: Listening on port %d\n", LISTENING_PORT);

    int connected_socked_fd = accept(socket_fd, (struct sockaddr*)&socket_addr, (socklen_t *)&socket_addr_len);

    if(connected_socked_fd == -1)
    {
        redirect_to_error(SERVER, "Failed to accept connection");
    }
        
    while(1)
    {
        char buffer[BUFFER_SIZE] = {0};
        int recvBytes = recv(connected_socked_fd, buffer, BUFFER_SIZE, 0);

        if(recvBytes == -1)
        {
            redirect_to_error(SERVER, "Failed to receive data");
        }
        
        printf("Msg sended: %s\n", buffer);
    }

    close(connected_socked_fd);
    close(socket_fd);

    return 0;
}
#include "defines.h"

int main (int argc, char *argv[])
{
    int status;
    Server server;
    Fds fds;

    /* Get port from command line */
    status = init_server_port(&server, argc, argv);
    if (status == -1) {
        server_redirect_to_error("Failed to get port");
    }

    /* Initialize server socket */
    status = init_server_socket(&server);
    if (server_socket == -1) {
        server_redirect_to_error("Failed to create socket");
    }
        
    server_message("Listening on port %d\n", port);

    /* Initialize fds structure */
    fds_size = START_POLL_SIZE;
    fds = calloc(fds_size, sizeof(*fds));

    if (!fds) {
        server_redirect_to_error("Failed to allocate memory");
    }

    fds[0].fd = server_socket;
    fds[0].events = POLLIN;
    fds_active_size = 1;

    server_message("Set up poll fd array");
        
    while(true)
    {
        status = poll(fds, fds_active_size, 2000); // Blocked option
        server_message("(%d)\n", status);

        if (status == -1) {
            server_redirect_to_error("Failed to poll");
        } else if (status == 0) {
            server_message("...");
            continue;
        }

        for (int i = 0; i < fds_active_size; i++) {
            if ((fds[i].revents & POLLIN) != 1) {
                continue;
            }

            server_message("%d is ready for read", fds[i].fd);

            if (fds[i].fd == server_socket) {
                int client_fd;
                char msg_to_send[BUF_SIZE_2];
                int status;

                client_fd = accept(server_socket, NULL, NULL);
                if (client_fd == -1) {
                    server_redirect_to_error("Failed to accept connection");
                }

                if (fds_active_size == fds_size) {
                    fds_size *= 2;
                    fds = realloc(fds, sizeof(*fds) * fds_size);
                }

                fds[fds_active_size].fd = client_fd;
                fds[fds_active_size].events = POLLIN;
                fds_active_size++;

                server_message("Added client %d to poll fd array", client_fd);

                memset(msg_to_send, '\0', sizeof msg_to_send);
                sprintf(msg_to_send, "Welcome. You are client fd [%d]\n", client_fd);

                status = send(client_fd, msg_to_send, strlen(msg_to_send), 0);
                if (status == -1) {
                    server_redirect_to_error("Failed to send message");
                }
            } else { // Client socket
                char buffer[BUF_SIZE_1];
                char msg_to_send[BUF_SIZE_2];
                int bytes_read, status, sender_fd, dest_fd;

                sender_fd = fds[i].fd;
                memset(buffer, '\0', sizeof buffer);
                bytes_read = recv(sender_fd, buffer, sizeof buffer, 0);

                if (bytes_read <= 0) {
                    if (bytes_read == 0) {
                        server_message("Client %d closed connection", sender_fd);
                    } else {
                        server_redirect_to_error("Failed to read message");
                    }

                    close(sender_fd);
                    
                    fds_active_size--;
                } else {
                    server_message("Got message from client %d: %s", sender_fd, buffer);

                    memset(msg_to_send, '\0', sizeof msg_to_send);
                    sprintf(msg_to_send, "[%d] says: %s", sender_fd, buffer);

                    for(int j = 0; j < fds_active_size; j++) {
                        dest_fd = fds[j].fd;

                        if (dest_fd != server_socket && dest_fd != sender_fd) {
                            status = send(dest_fd, msg_to_send, strlen(msg_to_send), 0);
                            if (status == -1) {
                                server_redirect_to_error("Failed to send message");
                            }
                        }
                    }

                }
            }
        }
    }

    close(server_socket);

    return 0;
}
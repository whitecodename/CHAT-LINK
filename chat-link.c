#include "defines.h"

int main (int argc, char *argv[])
{
    int status;
    Server server;
    Fds fds;

    /* Initialize server socket and port */
    init_server_socket(&server, argc, argv);
    server_message("Listening on port %d\n", server.port);

    /* Initialize fds structure */
    status = init_fds(&fds, &server);
    if (status == -1) {
        server_redirect_to_error("Failed to initialize file descriptors");
    }
        
    while(true) {
        status = poll(fds.fds, fds.active_size, POLL_TIME);
        if (status == -1) {
            server_redirect_to_error("Failed to poll");
        } else if (status == 0) {
            server_message("...");
            continue;
        }

        for (int i = 0; i < fds.active_size; i++) {
            struct pollfd *fd = &fds.fds[i];

            if (fd_is_ready(fd) == -1) {
                continue;
            }

            if (fd->fd == server.fd) {
                accept_client(&server, &fds);
            } else {
                read_client_message(&server, &fds, fd);
            }
        }
    }

    close(server.fd);

    return 0;
}
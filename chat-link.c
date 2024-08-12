#include "defines.h"

int main(int argc, char *argv[])
{
    int status, prog_status = EXIT_FAILURE;
    Server server = {};
    server.fd = ERROR;
    Hosts hosts = {};

    /* Initialize server socket and port */
    if (init_server_socket(&server, argc, argv) == ERROR) {
        goto err;
    }

    server_message("Listening on port %d\n", server.port);

    /* Initialize fds structure */
    status = init_fds(&hosts, &server);
    if (status == ERROR) {
        strcpy(server.err_msg, "Failed to initialize fds");
        goto err;
    }
    
    /* Main loop */
    while(true) {
        /* Polling */
        status = poll(hosts.fds, hosts.active_size, POLL_TIME);
        if (status == ERROR) {
            strcpy(server.err_msg, "Failed to poll");
            goto err;
        } else if (status == 0) {
            server_message("...");
            continue;
        }

        printf("dddd\n");

        /* Check for ready fds */
        for (int i = 0; i < hosts.active_size; i++) {
            struct pollfd *poll_fd = &hosts.fds[i];

            /* Check if fd is ready */
            if (!fd_is_ready(poll_fd)) {
                continue;
            }

            /* Check if fd is server fd */
            if (poll_fd->fd == server.fd) {
                accept_client(&server, &hosts);
            } else {
                fprintf(stderr, "FD (%d)\n", poll_fd->fd);
                read_client_message(&server, &hosts, poll_fd, i);
            }
        }
    }

    prog_status = EXIT_SUCCESS;

err:
    close(server.fd);
    free_hosts(&hosts);

    if (server.err_msg != NULL) {
        server_redirect_to_error(&server);
    }

    return prog_status;
}
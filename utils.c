#include "utils.h"

char* get_hostname(HOST host)
{
    return host == SERVER ? "SERVER" : "CLIENT";
}

void server_redirect_to_error(Server *server)
{
    redirect_to_error(SERVER, server->err_msg);

    if (server->fd != ERROR) {
        close(server->fd);
    }
}

void redirect_to_error(HOST host, char *message)
{
    fprintf(stderr, "%s: %s\n", get_hostname(host), message);
}

void client_redirect_to_error(char *message)
{
    redirect_to_error(CLIENT, message);
}

void clearBuffer()
{
    int c = 0;
    while (c != '\n' && c != EOF) c = getchar();
}
 
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

void server_message(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    printf("[%s]: ", get_hostname(SERVER));
    vprintf(format, args);
    printf("\n");
    
    va_end(args);
}

void client_message(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    
    printf("[%s]: ", get_hostname(CLIENT));
    vprintf(format, args);
    printf("\n");
    
    va_end(args);
}

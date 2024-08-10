#include "utils.h"

char* get_hostname(HOST host)
{
    return host == SERVER ? "SERVER" : "CLIENT";
}

/**
 * @brief Prints the error message preceded with the hostname label and exit with 1 status.
 * 
 * @param host SERVER or CLIENT
 * @param message Error message
 */
void server_redirect_to_error(char *message)
{
    redirect_to_error(SERVER, message);
}

void redirect_to_error(HOST host, char *message)
{
    fprintf(stderr, "%s: %s\n", get_hostname(host), message);
    exit(1);
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

/**
 * @brief Function to print a server message with a given prefix.
 *
 * @param prefix The prefix string for the server message.
 * @param format The format string for the message.
 * @param ... The variable arguments to format the string.
 */
void server_message(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    printf("[%s]: ", get_hostname(SERVER));
    vprintf(format, args);
    printf("\n");
    
    va_end(args);
}

/**
 * @brief Function to print a server message with a given prefix.
 *
 * @param prefix The prefix string for the server message.
 * @param format The format string for the message.
 * @param ... The variable arguments to format the string.
 */
void client_message(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    
    printf("[%s]: ", get_hostname(CLIENT));
    vprintf(format, args);
    printf("\n");
    
    va_end(args);
}

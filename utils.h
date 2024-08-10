#ifndef UTILS_H
#define UTILS_H

#include "defines.h"

void server_redirect_to_error(char *message);
void redirect_to_error(HOST host, char *message);
void client_redirect_to_error(char *message);
void clearBuffer();
int readString(char *buffer, int buffer_size);
void server_message(const char *format, ...);
void client_message(const char *format, ...);

#endif // UTILS_H
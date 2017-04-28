#ifndef SERVER_FUNCTIONS_H
#define SERVER_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

void error (const char*);
void setup_serv_addr (struct sockaddr_in &, const int);
void bind_socket (const int, const struct sockaddr_in);
void dostuff_dgram (const int, struct sockaddr_in &);
void write_to_file (const char*);
int create_dgram_socket ();

#endif

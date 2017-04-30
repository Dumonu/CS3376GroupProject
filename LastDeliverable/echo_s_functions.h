#ifndef ECHO_S_FUNCTIONS_H
#define ECHO_S_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

extern unsigned long LOGIP = INADDR_ANY;
extern int LOGPORT = 9999;

void error (const char*);
void setup_serv_addr (struct sockaddr_in &, const int);
void bind_socket (const int, const struct sockaddr_in);
void dostuff_stream (const int, struct sockaddr_in &, unsigned long, int);
void dostuff_dgram (const int, struct sockaddr_in &, unsigned long, int);
int create_stream_socket ();
int create_dgram_socket ();
int acpt (const int, struct sockaddr_in &);
int logUDP(char*, char*, unsigned long, int);

void interruptHandler(int);

#endif

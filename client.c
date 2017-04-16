// Program: client.c
// author: Henry Tran
// date: 4/13/2017
// Purpose: CS 3376
// Description: The steps involved in establishing a socket on the client side are as follows:
// Create a socket with the socket() system call
// Connect the socket to the address of the server using the connect() system call
// Send and receive data. There are a number of ways to do this, but the simplest is to use the read() and write() system calls.
// The client prompts the user which type of connection they would like to make and establishes the connection accordingly

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "client_functions.h"

// Main function of the client
int main(int argc, char *argv[])
{
	// char to hold the type of connection
	char type = "";
	// Prompt the user what type of connection they would like to make
	printf("Would you like to make a TCP or UDP connection?"
	// Stores the users selection into the variable type
	scanf(%c, &type);
	// establishes a TCP connection
	if(type == "TCP")
	{
		connectTCP(argc, argv);
	}
	// establishes a UDP connection
	else if(type == "UDP")
	{
		connectUDP(argc, argv);
	}
 	   return 0;
}

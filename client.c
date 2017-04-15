// Program: client.c
// author: Henry Tran
// date: 4/13/2017
// Purpose: CS 3376
// Description: The steps involved in establishing a socket on the client side are as follows:
// Create a socket with the socket() system call
// Connect the socket to the address of the server using the connect() system call
// Send and receive data. There are a number of ways to do this, but the simplest is to use the read() and write() system calls.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "client_functions.h"

//Function for error
//void error(const char *msg)
//{
//  perror(msg);
//  exit(0);
//}

// Main function that returns an integer and takes arguments as parameters
int main(int argc, char *argv[])
{
	//Initialize variables
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
// All of this code is the same as that in the server so we are commenting it out.
  
    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
	// Creates the socket using the socket() system call
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	// Make sure the socket has been opened
    if (sockfd < 0) 
        error("ERROR opening socket");
  
	// The variable argv[1] contains the name of a host on the internet
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    // Sets fields in serv_addr. 
	bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
	// Have to use the function bcopy because server->h_addr is a character string 
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
	// Connects the socket to the address of the server using the connect() system call.
	// Takes 3 arguments: the socket file descriptor, the host address that it wants to connect to, and the size of the address
	// Function returns 0 on success, -1 on failure
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
	// Prompts the user to enter a message
    printf("Please enter the message: ");
    bzero(buffer,256);
	// Uses fgets to read the message from stdin
    fgets(buffer,255,stdin);
	// Send data using the write() system call to write to the socket
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
	// Recieve data using the read() system call to read the reply from the socket and display the reply to the screen	
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);
    close(sockfd);
    return 0;
}

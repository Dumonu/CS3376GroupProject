// Program: echo_s_functions.c
// author: Henry Tran
// date: 4/22/2017
// Purpose: CS 3376
// Description: Functions for the echo_s.c 
// Changes were made from server_functions.c to echo inputs from the client
// Continous - implemented
// TCP - implemented
// UDP - implemented
// Based very heavily on server2.c from http://www.linuxhowtos.org/C_C++/socket.htm
// Comments largely based on above article
// select() used to be able to accept both udp/tcp connections
// select() information : http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html#select

#include "echo_s_functions.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void error(const char* msg) // prints an error message and exits the program
{
	perror(msg);
	exit(1);
}


void setup_serv_addr (struct sockaddr_in & serv_addr, const int portno) // sets up the server adress
{
	bzero((char*) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
}

void bind_socket (const int sockfd, const struct sockaddr_in serv_addr) // binds the socket to the port number passed on the command line
{
	if (bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
	{
		error("ERROR on binding");
	}
}

void dostuff_stream (const int newsockfd) // uses a stream socket to read a message from the client and send a confirmation message to the client (TCP)
{
	int err;
	char buffer[256];
	bzero(buffer, 256);
	//while loop to continuously read from the client and write to the client (echo)
	while (1) {
	  // reads from the client
      int read = recv(newsockfd, buffer, 255, 0);

      if (!read) break; // done reading
      if (read < 0) error("Client read failed\n");
      // writes to the client
      err = send(newsockfd, buffer, read, 0);
      if (err < 0) error("Client write failed\n");
    }
	
	
    //CHERRYSCODE:
    //When child is done, send a SIGCHILD signal, with SIG_IGN to make zombie processes disappear.
    signal(SIGCHLD, SIG_IGN);
}

void dostuff_dgram (const int sockfd, struct sockaddr_in & cli_addr) // uses a datagram socket to read a message from the client and send a confirmation message to the client (UDP)
{
	int len, n;
    char buffer[256];
    struct sockaddr_in remote;

    /* need to know how big address struct is, len must be set before the
       call to recvfrom!!! */

    len = sizeof(remote);
    //while loop to continuously read from the client and write to the client (echo)
    while (1) {
      /* read a datagram from the socket (put result in buffer) */
      n = recvfrom(sockfd,buffer,255,0,(struct sockaddr *)&remote,(socklen_t*)&len);

      /* print out the address of the sender 
	  not necessary but extra.
	  */
      printf("Got a datagram from %s port %d\n",
             inet_ntoa(remote.sin_addr), ntohs(remote.sin_port));

      if (n<0) {
        perror("Error receiving data");
      } else {
        printf("GOT %d BYTES\n",n);
        /* Got something, just send it back (echos)*/
        sendto(sockfd,buffer,n,0,(struct sockaddr *)&remote,len);
      }
    }
}

int create_stream_socket () // creates a stream socket
{
	int sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		error("ERROR opening socket");
	}
	return sockfd;
}
	
int create_dgram_socket () // creates a datagram socket
{
	int sockfd;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
	{
		error("ERROR opening socket");
	}
	return sockfd;
}

int acpt (const int sockfd, struct sockaddr_in & cli_addr) // accepts a client's request to connect to the server
{
	int newsockfd;
	socklen_t cli_addr_size = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, &cli_addr_size);
	if (newsockfd < 0)
	{
		error("ERROR on accept");
	}
	return newsockfd;
}
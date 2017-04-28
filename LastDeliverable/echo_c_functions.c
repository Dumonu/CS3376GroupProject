// Program: client_functions.c
// Author: Adrian Rivera-Zayas
// Date: 4/20/2017
// Purpose: SE 3376.502
// Description: this program establishes a connection to a server either through a TCP or UDP connection, to be echoed to a server

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//  header only needed for client in the unix domain
#include <sys/un.h>

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

int connectUDP(int argc, char *argv[])
{
	// Initialize Variables
	int sock, n;
	unsigned int length;
	struct sockaddr_in server, from;
	struct hostent *hp;
	char buffer[256];

	//  Takes only 3 arguments from user
	//  1.socket file descriptor 
	//  2.the host address that it wants to connect to 
	//  3.size of the address)
	if (argc != 3) 
	{
		printf("Usage: server port\n");
		exit(1);
	}

	// Creating the socket
	// Pass internet domain, datagram socket, and 0 for protocol
	sock= socket(AF_INET, SOCK_DGRAM, 0);

	if (sock < 0)// means port number not passed
	{
		error("socket");
	}

	//  Specify that it is an internet domain
	server.sin_family = AF_INET;

	//  argv[1] contains the name of a host on the internet
	hp = gethostbyname(argv[1]);
	if (hp==0) // means incorrect host name
	{
		error("Unknown host");
	}

	// Have to use the function bcopy because server->h_addr is a character string
	bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);
	
	// Get port number
	server.sin_port = htons(atoi(argv[2]));

	// Size of internet address
	length=sizeof(struct sockaddr_in);

	// Ask user to input message to be sent
	write(1, "Please enter the message: ", 26);

	// Receive input
	bzero(buffer,256);
	read(0, buffer, 255);

	// Send to internet server
	n=sendto(sock,buffer, strlen(buffer), 0, (const struct sockaddr *)&server, length);
	if (n < 0)
	{
		error("Sendto");
	}

	// Receive from internet server
	n = recvfrom(sock,buffer,256,0,(struct sockaddr *)&from, &length);
	if (n < 0)
	{
		error("recvfrom");
	}

	// Acknowledge data received successfully
	write(1,"Echo from server: ",18);
	write(1,buffer,n);

	// Close socket
	close(sock);
	return 0;
}

int connectTCP(int argc, char *argv[])
{
	// Initialize Variables
	int sockfd, n;
	unsigned int servlen;
	struct sockaddr_in  serv_addr;
	struct hostent *hp;
	char buffer[82];

	if (argc != 3) 
	{
		printf("Usage: server port\n");
		exit(1);
	}	
	// Set fields in serv_addr
	bzero((char *)&serv_addr,sizeof(serv_addr));

	//  Specify that it is an Internet domain
	serv_addr.sin_family = AF_INET;
	
	// Copy user-inputted path
    	serv_addr.sin_port = atoi(argv[2]);
    	hp = gethostbyname(argv[1]);
    	if (hp == 0)
    	{
        	error("Unknown host");
    	}
    	bcopy((char *)hp->h_addr, (char *)&serv_addr.sin_addr, hp->h_length);

	// Length of server path
        servlen = sizeof(struct sockaddr_in);

	if ((sockfd = socket(AF_INET, SOCK_STREAM,0)) < 0)// means error in creating socket
	{
		error("Creating socket");
	}

	// Connects the socket to the address of the server using the connect() system call.
	// Takes 3 arguments: the socket file descriptor, the host address that it wants to connect to, and the size of the address
	// Function returns 0 on success, -1 on failure
	if (connect(sockfd, (struct sockaddr *)&serv_addr, servlen) < 0)
	{
		error("echo_c: Connecting");
	}

	// Prompts the user to enter a message
	printf("Please enter your message: ");
	bzero(buffer,82);

	// Uses fgets to read the message from stdin
	fgets(buffer,80,stdin);

	// Send data using the write() system call to write to the socket
	write(sockfd,buffer,strlen(buffer));
	if (n < 0) 
	{
		error("ERROR writing to socket");
	}
	// Receive data using the read() system call to read the reply from the socket and display the reply to the screen
	n=read(sockfd,buffer,80);
	if (n < 0) 
	{
		error("ERROR reading from socket");
	}
	// Displaying reply on screen
	printf("Echo from server: \n");
	write(1,buffer,n);

	// Closing socket
	close(sockfd);
	return 0;
}

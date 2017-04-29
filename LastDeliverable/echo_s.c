// Program: echo_s.c
// author: Henry Tran
// date: 4/22/2017
// Purpose: CS 3376
// Description: server file containing main method. Continous server that accepts both udp/tcp sockets 
// This is the same as the first deliverable's server.c
// Continous - implemented
// TCP - implemented
// UDP - implemented
// Based very heavily on server2.c from http://www.linuxhowtos.org/C_C++/socket.htm
// Comments largely based on above article
// select() used to be able to accept both udp/tcp connections
// select() information : http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html#select

/* a server in the unix domain.  The pathname of 
   the socket address is passed as an argument */
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/un.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "echo_s_functions.h"
int main(int argc, char *argv[])
{
    unsigned long logip = INADDR_ANY;
    int logport = 9999;
    // ignore Child signals.
    signal(SIGCHLD, SIG_IGN);
    // send message on interrupt signals
    signal(SIGINT, interruptHandler);

	//if input is not formatted correctly, print correct input formatting and exit
	if(argc < 2)
	{
		std::cout<< "usage: ./server portno\n";
		exit(0);
	}
	for(int i = 2; i < (argc - 1); i += 2)
	{
		if(strcmp(argv[i], "-logip") == 0)
		{
			logip = inet_addr(argv[i + 1]);
		}
		else
		{
			if(strcmp(argv[i], "-logport") == 0)
			{
				logport = atoi(argv[i + 1]);
			}
			else
			{
				std::cout << "usage: ./echo_s portno -logip logipaddr -logport logportno\n";
				exit(0);
			}
		}
	}

    LOGIP = logip;
    LOGPORT = logport;

	int sockfd, newsockfd, servlen, pid, portno; //n will contain the number of characters read/written by the socket
	struct sockaddr_in  cli_addr, serv_addr; //sockaddr_in contains the port for the socket
	portno = atoi(argv[1]);
   
	//variables for select()
	fd_set readfds; //set containing file descriptors used for reading
	fd_set master;	//for select() - reset every loop for select() to be able to detect new connections
	struct timeval tv; //used for select(), sets how long before select times out
	tv.tv_sec = 5; //time to wait in seconds
	tv.tv_usec = 5000000; //time to wait in microseconds
	FD_ZERO(&readfds); //clear everything from sets for select() setup
		
	//TCP setup - create, set server address variables, bind to port
	sockfd = create_stream_socket();	
	setup_serv_addr(serv_addr, portno);					 
        bind_socket(sockfd, serv_addr);									
        	  
	//UDP setup
	struct sockaddr_in server;
	struct sockaddr_in from;   
	int sock=create_dgram_socket();
	int length_UDP = sizeof(server);
	setup_serv_addr(server, portno);
	bind_socket(sock, server);
  
	//main loop, more select setup
	listen(sockfd,5); //listen on socket for connections, second arg is the number of connections that can be waiting while the process handles a connection (backlog list)
	FD_SET(sockfd, &readfds); //add socket to readfds, for use by select()
	FD_SET(sock, &readfds);
	master = readfds; 
	while(1){
		readfds = master; //restore readfds to master so select can check again if a connection is waiting
		//select() must be used because both accept() and recvfrom() block - so we need a nonblocking way to check if connection/data is ready
		int m = select(sock+1, &readfds, NULL, NULL, &tv); 	//args: highest fd+1, fd_set *read_fds, fd_set *write_fds, fd_set *except_fds, struct timeval *timeout
															//timeval set to null - will never timeout 
		if(m < 0)
		{
			error("select");
			exit(0);
		}
		if(FD_ISSET(sockfd, &readfds)) // if TCP connection is detected
		{
			newsockfd = acpt(sockfd, cli_addr); //arguments: 
			pid = fork(); //fork another process for each connection
			if(pid < 0) 
				error("error on fork");
			if(pid == 0){ //call dostuff(), which will handle all communication once a connection has been established (only processes created by fork() go here)
				close(sockfd);
				dostuff_stream(newsockfd, cli_addr, logip, logport); //all communication with client is here
				exit(0);		
			}
			else
				close(newsockfd); //close connection with client
		}		
		//UDP
		if(FD_ISSET(sock, &readfds)) //if there is data to be read on the datagram socket8
		{
			dostuff_dgram(sock, from, logip, logport);
		}
	}//end of while
	close(sockfd);
	return 0; //never get here
}

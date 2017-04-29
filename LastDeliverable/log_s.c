// Program: log_s.c
// author: Ian Llewellyn
// date: 4/11/2017
// Purpose: CS 3376
// Description: server file containing main method. Continous server that accepts info from echo_s server and prints data to file
// Based very heavily on server2.c from http://www.linuxhowtos.org/C_C++/socket.htm
// Comments largely based on above article
// select() used to not block the program when waiting for input
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
#include "log_s_functions.h"
int main(int argc, char *argv[])
{
    //ignore child signals, allow children to die properly.
    signal(SIGCHLD, SIG_IGN);
	
	int portno = 9999;
   	// for the user3 implementation of port argument
   	if(argc >= 2)
	{
		for(int i = 2; i < (argc - 1); i += 2)
		{
			if(strcmp(argv[i], "-port") == 0)
			{
				portno = atoi(argv[i + 1]);
			}
		}
	}
	
	//variables for select()
	fd_set readfds; //set containing file descriptors used for reading
	fd_set master;	//for select() - reset every loop for select() to be able to detect new connections
	struct timeval tv; //used for select(), sets how long before select times out
	tv.tv_sec = 5; //time to wait in seconds
	tv.tv_usec = 5000000; //time to wait in microseconds
	FD_ZERO(&readfds); //clear everything from sets for select() setup
										        	  
	//UDP setup
	struct sockaddr_in server;
	struct sockaddr_in from;   
	int sock=create_dgram_socket();
	int length_UDP = sizeof(server);
	setup_serv_addr(server, portno);
	bind_socket(sock, server);
  
	//main loop, more select setup
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
		//UDP
		if(FD_ISSET(sock, &readfds)) //if there is data to be read on the datagram socket8
		{
			int pid = fork(); //fork another process for each piece of data recieved
			if(pid < 0) 
				error("error on fork");
			if(pid == 0){ 
				dostuff_dgram(sock, from); //all communication with client is here
				exit(0);		
			}
		}
	}//end of while
	return 0; //never get here
}

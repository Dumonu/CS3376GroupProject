// Program: server.c
// author: Ian Llewellyn
// date: 4/11/2017
// Purpose: CS 3376
// Description: server file containing main method. Continous server that accepts both udp/tcp sockets
// Continous - implemented, should be tested
// TCP - implemented, should be tested
// UDP - Not implemented
// Based very heavily on server2.c and U_server.c from http://www.linuxhowtos.org/C_C++/socket.htm
// Comments largely based on rest of above article

/* a server in the unix domain.  The pathname of 
   the socket address is passed as an argument */
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/un.h>
#include <stdio.h>
void error(const char *);
int main(int argc, char *argv[])
{
   int sockfd, newsockfd, servlen, pid; //n will contain the number of characters read/written by the socket
   socklen_t clilen;
   struct sockaddr_un  cli_addr, serv_addr; //sockaddr_un contains the address of the socket - different in unix


   if ((sockfd = socket(AF_UNIX,SOCK_STREAM,0)) < 0) //creates a socket with three arguments : address (unix version), what type (SOCK_STREAM, SOCK_DGRAM), protocol (default 0)
       error("creating socket"); //prints error message if socket creation fails
   bzero((char *) &serv_addr, sizeof(serv_addr)); //sets all values in serv_addr buffer to zero
   serv_addr.sun_family = AF_UNIX; //should always be set to AF_UNIX?
   strcpy(serv_addr.sun_path, argv[1]); //copies first argument (a path name) from command line 
   servlen=strlen(serv_addr.sun_path) + 
                     sizeof(serv_addr.sun_family); //stores the length of the address in servlen, for use in bind()
					 
   if(bind(sockfd,(struct sockaddr *)&serv_addr,servlen)<0) //binds socket to address, needs 3 arguments takes socket file descriptor (sockfd), 
															//the address it is bound to (?) cast to the type "sockaddr", and the length of the address
       error("binding socket"); 

   listen(sockfd,5); //listen on socket for connections, second arg is the number of connections that can be waiting while the process handles a connection (backlog list)
   clilen = sizeof(cli_addr);
   while(1){
		newsockfd = accept( //accept causes process to block
			sockfd,(struct sockaddr *)&cli_addr,&clilen); //arguments: 
		if (newsockfd < 0) 
			error("accepting");
		pid = fork(); //fork another process for each connection
		if(pid < 0) 
			error("error on fork");
		if(pid == 0){ //call dostuff(), which will handle all communication once a connection has been established (only processes created by fork() go here)
			close(sockfd);
			dostuff(newsockfd);
			exit(0);
		}
		else
			close(newsockfd); //close connection with client
   }//end of while
   close(sockfd);
   return 0; //never get here
}

void error(const char *msg) //prints a message when an error occurs and exits the program
{	
    perror(msg);
    exit(0);
}

/******** DOSTUFF() *********************
 There is a separate instance of this function 
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/
void dostuff (int sock)
{
	int n; //number of characters read or written
	char buf[80]; //buffer will be used for read/write
	
	n=read(newsockfd,buf,80); //only get here after successful connection with client. args: socket file descriptor, buffer to write to, max length of message (in chars)
	printf("A connection has been established\n");
	write(1,buf,n); //prints out message recieved by client
	write(newsockfd,"I got your message\n",19); //write to client
}
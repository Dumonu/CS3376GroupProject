// Program: server.c
// author: Ian Llewellyn
// date: 4/11/2017
// Purpose: CS 3376
// Description: server file containing main method. Continous server that accepts both udp/tcp sockets
// Continous - implemented, works with 2 clients
// TCP - implemented, works with 2 clients
// UDP - implemented, need client.c to test
// Based very heavily on server2.c and U_server.c from http://www.linuxhowtos.org/C_C++/socket.htm
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
void error(const char *);
void dostuff(int sock);
int main(int argc, char *argv[])
{
   int sockfd, newsockfd, servlen, pid; //n will contain the number of characters read/written by the socket
   socklen_t clilen;
   struct sockaddr_un  cli_addr, serv_addr; //sockaddr_un contains the address of the socket - different in unix
   
   //variables for select()
   fd_set readfds; //set containing file descriptors used for reading
// fd_set writefds; //set containing file descriptors used for writing
// fd_set exceptfds; //might not end up implementing
   fd_set master;
   struct timeval tv; //used for select(), sets how long before select times out
   tv.tv_sec = 5; //time to wait in seconds
   tv.tv_usec = 5000000; //time to wait in microseconds - not sure why there are two of these
   
	FD_ZERO(&readfds); //clear everything from sets for setup
//	FD_ZERO(writefds);
	
	

   if ((sockfd = socket(AF_UNIX,SOCK_STREAM,0)) < 0) //creates a socket with three arguments : address (unix version), what type (SOCK_STREAM, SOCK_DGRAM), protocol (default 0)
       error("creating socket"); //prints error message if socket creation fails
   bzero((char *) &serv_addr, sizeof(serv_addr)); //sets all values in serv_addr buffer to zero
   serv_addr.sun_family = AF_UNIX; //should always be set to AF_UNIX?
   strcpy(serv_addr.sun_path, argv[1]); //copies first argument (a path name) from command line 
   servlen=strlen(serv_addr.sun_path) + 
                     sizeof(serv_addr.sun_family); //stores the length of the address in servlen, for use in bind()
					 
   if(bind(sockfd,(struct sockaddr *)&serv_addr,servlen)<0) //binds socket to address, needs 3 arguments takes socket file descriptor (sockfd), 
															//the address it is bound to is(?) cast to the type "sockaddr", and the length of the address
       error("binding socket"); 
	  
   //UDP setup
   int sock_UDP, length_UDP, n;
   socklen_t fromlen;
   struct sockaddr_un server;
   struct sockaddr_un from;
   char buf_UDP[1024];

   if (argc < 3) {
      fprintf(stderr, "ERROR, no address provided\n");
      exit(0);
   }
   
   sock_UDP=socket(AF_UNIX, SOCK_DGRAM, 0);
   if (sock_UDP < 0) error("Opening socket");
   length_UDP = sizeof(server);
   bzero(&server,length_UDP);
   server.sun_family=AF_UNIX;
//   server.sun_path=INADDR_ANY;
   strcpy(serv_addr.sun_path, argv[2]);
   if (bind(sock_UDP,(struct sockaddr *)&server,length_UDP)<0) 
       error("binding");
   fromlen = sizeof(struct sockaddr_un);
   
   
   //main loop, more select setup
   listen(sockfd,5); //listen on socket for connections, second arg is the number of connections that can be waiting while the process handles a connection (backlog list)
   FD_SET(sockfd, &readfds); //add socket to readfds, for use by select()
   FD_SET(sock_UDP, &readfds);
   master = readfds; //for select()
   clilen = sizeof(cli_addr);
   while(1){
		readfds = master; //restore readfds to master so select can check again if a connection is waiting
		select(sock_UDP+1, &readfds, NULL, NULL, &tv); //args: highest fd+1, fd_set *read_fds, fd_set *write_fds, fd_set *except_fds, struct timeval *timeout
											  //timeval set to null - will never timeout || might only need to use second arg for sockets?
		
		if(FD_ISSET(sockfd, &readfds)) // if TCP connection is detected
		{
			newsockfd = accept( //accept causes process to block
				sockfd,(struct sockaddr *)&cli_addr,&clilen); //arguments: 
			if (newsockfd < 0) 
				error("accepting");
			pid = fork(); //fork another process for each connection
			if(pid < 0) 
				error("error on fork");
			if(pid == 0){ //call dostuff(), which will handle all communication once a connection has been established (only processes created by fork() go here)
				close(sockfd);
//				std::cout << "child reporting for duty\n";
				dostuff(newsockfd);
				exit(0);		
			}
		else
			close(newsockfd); //close connection with client
		
		//UDP
		if(FD_ISSET(sock_UDP, &readfds)) //if UDP socket can be read
		{
			n = recvfrom(sock_UDP,buf_UDP,1024,0,(struct sockaddr *)&from,&fromlen); //read from udp socket
				if (n < 0) error("recvfrom");
			write(1,"Received a datagram: ",21); //write to stdout
			write(1,buf_UDP,n); //write recieved message to stdout
			n = sendto(sock_UDP,"Got your message\n",17,
					0,(struct sockaddr *)&from,fromlen); //return a message
			if (n  < 0) error("sendto");
		}
	   }
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
	
	n=read(sock,buf,80); //only get here after successful connection with client. args: socket file descriptor, buffer to write to, max length of message (in chars)
	printf("A connection has been established\n");
	write(1,buf,n); //prints out message recieved by client
	write(sock,"I got your message\n",19); //write to client
}
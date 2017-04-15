#include "server_functions.h"

void error(const char* msg)
{
	perror(msg);
	exit(1);
}


void setup_serv_addr (struct sockaddr_in & serv_addr, const int portno)
{
	bzero((char*) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
}

void bind_socket (const int sockfd, const struct sockaddr_in serv_addr)
{
	if (bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
	{
		error("ERROR on binding");
	}
}

void dostuff_stream (const int newsockfd)
{
	int n;
	char buffer[256];
	bzero(buffer, 256);
	n = read(newsockfd, buffer, 255);
	if (n < 0)
	{
		error("ERROR reading from socket");
	}
	printf("Here is the message: %s\n", buffer);
	n = write(newsockfd, "I got your message", 18);
	if (n < 0)
	{
		error("ERROR writing to socket");
	}
    
    //CHERRYSCODE:
    //When child is done, send a SIGCHILD signal, with SIG_IGN to make zombie processes disappear.
    signal(SIGCHLD, SIG_IGN);
}

void dostuff_dgram (const int sockfd, struct sockaddr_in & cli_addr)
{
	int n;
	char buffer[256];
	bzero(buffer, 256);
	socklen_t cli_addr_size = sizeof(cli_addr);
	n = recvfrom(sockfd, buffer, 255, 0, (struct sockaddr*) &cli_addr, &cli_addr_size);
	if (n < 0)
	{
		error("ERROR receiving from socket");
	}
	printf("Here is the message: %s\n", buffer);
	n = sendto(sockfd, "I got your message\n", 20, 0, (struct sockaddr*) &cli_addr, (socklen_t) sizeof(cli_addr));
	if (n < 0)
	{
		error("ERROR sending to socket");
	}
}

int create_stream_socket ()
{
	int sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		error("ERROR opening socket");
	}
	return sockfd;
}
	
int create_dgram_socket ()
{
	int sockfd;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
	{
		error("ERROR opening socket");
	}
	return sockfd;
}

int acpt (const int sockfd, struct sockaddr_in & cli_addr)
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

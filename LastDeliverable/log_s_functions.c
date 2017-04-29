#include "log_s_functions.h"

void error(const char* msg)
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

void bind_socket (const int sockfd, const struct sockaddr_in serv_addr) // binds the socket to the port number 9999
{
	if (bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
	{
		error("ERROR on binding");
	}
}


void dostuff_dgram (const int sockfd, struct sockaddr_in & cli_addr) // uses a datagram socket to read a message from the echo_s and call write_to_file
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


//	printf("Here is the message: %s\n", buffer);
//	n = sendto(sockfd, "I got your message\n", 20, 0, (struct sockaddr*) &cli_addr, (socklen_t) sizeof(cli_addr));
    if(strcmp(buffer, "echo_s is stopping") == 0)
    {
        // stop server
        printf("%s\n", "log_s is stopping");
        kill(getppid(), SIGTERM);
        exit(0);
    }
	write_to_file(buffer);
	
	// There used to be an incorrect call to signal() here. At the time, Cherry did not know what signal() does. He does now.
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
void write_to_file(const char* buffer) //print the recieved message to echo.log
{
	FILE *f = fopen("echo.log", "a");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}

	/* print some text */
	fprintf(f, "Some text: %s", buffer); //print to the file

}

void term(int signum)
{
    kill(0, SIGTERM);
    exit(0);
}

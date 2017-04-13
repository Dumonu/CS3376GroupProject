#A makefile for the CS3376.502 group project
#Group members: Adrian Rivera-Zayas, Devon Hayworth, Ryan Corcoran, Henry Tran, Jacob Cherry, and Ian Llewellyn
#Created on 2017-04-12

CC = g++
FLAGS = 
OBJS = client.o client_functions.o server.o server_functions.o
NAME = prgm

prgm : $(OBJS)
	$(CC) -c $(NAME) $(OBJS)
client.o : client.c
	$(CC) -o client.c
client_functions.o : client_functions.c
	$(CC) -o client_functions.c
server.o : server.c
	$(CC) -o server.c
server_functions.o : server_functions.c
	$(CC) -o server_functions.c
clean :
	rm -f core $(NAME) $(OBJS)

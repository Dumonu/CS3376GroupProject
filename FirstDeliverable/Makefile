#c makefile for the CS3376.502 group project
#Group members: Adrian Rivera-Zayas, Devon Hayworth, Ryan Corcoran, Henry Tran, Jacob Cherry, and Ian Llewellyn
#Created on 2017-04-12

CC = g++
FLAGS = 
SERVOBJS = server.o server_functions.o
CLIENTOBJS = client.o client_functions.o 
SERVNAME = server
CLIENTNAME = client

default: server client

server : $(SERVOBJS)
	$(CC) -o $(SERVNAME) $(SERVOBJS)
client : $(CLIENTOBJS)
	$(CC) -o $(CLIENTNAME) $(CLIENTOBJS)
client.o : client.c
	$(CC) -c client.c
client_functions.o : client_functions.c
	$(CC) -c client_functions.c
server.o : server.c
	$(CC) -c server.c
server_functions.o : server_functions.c server_functions.h
	$(CC) -c server_functions.c
clean :
	rm -f core $(SERVNAME) $(CLIENTNAME) $(SERVOBJS) $(CLIENTOBS)

CC = g++
SERVOBJS = echo_s.o echo_s_functions.o
CLIOBJS = echo_c.o echo_c_functions.o
LOGOBJS = log_s.o log_s_functions.o
PROGS = echo_s echo_c log_s

all : $(PROGS)

echo_s : $(SERVOBJS)
	$(CC) -o echo_s $(SERVOBJS)
echo_c : $(CLIOBJS)
	$(CC) -o echo_c $(CLIOBJS)
log_s : $(LOGOBJS)
	$(CC) -o log_s $(LOGOBJS)
echo_s.o : echo_s.c
	$(CC) -c echo_s.c
echo_s_functions.o : echo_s_functions.c echo_s_functions.h
	$(CC) -c echo_s_functions.c
echo_c.o : echo_c.c
	$(CC) -c echo_c.c
echo_c_functions.o : echo_c_functions.c echo_c_functions.h
	$(CC) -c echo_c_functions.c
log_s.o : log_s.c
	$(CC) -c log_s.c
log_s_functions.o : log_s_functions.c log_s_functions.h
	$(CC) -c log_s_functions.c


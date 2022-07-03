all: server.o client.o
	gcc -o p1 server.o client.o
client.o: client.c server.h
	gcc -c client.c server.h
server.o: server.c server.h
	gcc -c server.c server.h
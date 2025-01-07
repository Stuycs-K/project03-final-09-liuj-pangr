all: client server
compile: client.o server.o handshake.o rps.o
	@gcc -o clientSide client.o handshake.o rps.o
	@gcc -o serverSide server.o handshake.o rps.o

client:
	@./clientSide

server:
	@./serverSide

client.o: client.c handshake.c rps.c
	@gcc -c client.c

server.o: server.c handshake.c rps.c
	@gcc -c server.c

handshake.o: handshake.c
	@gcc -c handshake.c

rps.o: rps.c
	@gcc -c rps.c

clean:
	@rm -f *.o clientSide serverSide

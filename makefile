compile: client.o server.o rps.o handshake.o
	@gcc -o clientSide client.o handshake.o rps.o
	@gcc -o serverSide server.o handshake.o rps.o

client: clientSide
	@./clientSide

server: serverSide
	@./serverSide

client.o: client.c handshake.h rps.h
	@gcc -c -Wall client.c

server.o: server.c handshake.h rps.h
	@gcc -c -Wall server.c

handshake.o: handshake.c handshake.h
	@gcc -c -Wall handshake.c

rps.o: rps.c rps.h
	@gcc -c -Wall rps.c

clean:
	@rm -f *.o clientSide serverSide

compile: client.o server.o rps.o handshake.o
	@rm -rf executables
	@mkdir executables
	@chmod 0777 executables
	@gcc -o clientSide client.o handshake.o rps.o
	@gcc -o serverSide server.o handshake.o rps.o
	@mv clientSide executables
	@mv serverSide executables

client:
	@./executables/clientSide

server:
	@./executables/serverSide

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
	@rm -rf executables

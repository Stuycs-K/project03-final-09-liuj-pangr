#include "handshake.h"
#include "rps.h"
//addr[0] = from client/to server, addr[1] = to client/from server
int handleBracket(int * addr){

  /* Outline
	1. The main server waits until enough clients join.
	2. The server passes one half of clients into a function and the other half to another.
			In this function, the server forks.
			If a subserver receives only one client, this client is the winner.
	3. The server receives one client from both halves.
	4. The server does the game with both clients.
	5. The server passes the winner to the superserver and terminates.
	6. Repeat 2-5 until the main server has a winner.

	Think of how merge sort is implemented
  */

  return 0;
}
int main(){
  int adr[2];
  int * addr = adr;
  addr = server_handshake(addr);
  int check = 300;
  write(addr[0], &check, 4);
  read(addr[1], &check, 4);
  printf("%d\n", check);

  handleBracket(addr);
  return 0;
}

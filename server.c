#include "handshake.h"
#include "rps.h"
#define TEMP_MAX 4
//addr[0] = from client/to server, addr[1] = to client/from server
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
int handleBracket(int addr[][2]){

  return 0;
}
int main(){
  // TODO: Make the server wait until enough clients join
  int addrs[TEMP_MAX][2];
  int adr[2];
  int * addr = adr;
  
  for (int i = 0; i < TEMP_MAX; i++) { // assuming that the server needs TEMP_MAX to continue, for testing
    addr = server_handshake(addr);
    addrs[i][0] = adr[0];
    addrs[i][1] = adr[1];
  }

  for (int i = 0; i < TEMP_MAX; i++) {
    int check = 300;
    write(addrs[i][0], &check, 4);
    read(addrs[i][1], &check, 4);
    printf("%d\n", check);
  }
  // handleBracket(addr);

  return 0;
}

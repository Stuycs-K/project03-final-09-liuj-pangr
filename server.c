#include "handshake.h"
#include "rps.h"
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

int main(){
  int MYWKP = -1;
  struct player * list = malloc(sizeof(struct player) * 8);
  char buff[LINE_SIZE];
  int current = 0;
  printf("Looking for clients? input y/n\n");
  int connectCode = CONNECTED;
  while (fgets(buff, 511, stdin)){
    printf("%s\n", buff);
    if (buff[0] == 'y'){
      list[current].downstream = server_handshake(&MYWKP);
      write(list[current].downstream, &connectCode, 4);
      current++;
      printf("%d\n", current);
    }
    if (buff[0] == 'n'){
      break;
    }
    printf("Looking for clients? input y/n\n");
  }
  connectCode = READY;
  for (int i = 0; i < current; i ++){
    printf("%d\n",list[i].downstream);
    write(list[i].downstream, &connectCode, 4);
  }

  char p1;
  char p2;
  for (int i = 0; i < current-1; i++) { // TODO: Manage multiple pairs
    read(MYWKP, buff, 511);
    p1 = buff[0];
    read(MYWKP, buff, 511);
    p2 = buff[0];
    printf("P1 chose %c. P2 chose %c.\n", p1, p2);
    printf("Result of fight is %c.\n", fight(p1,p2));
  }

  free(list);
  return 0;
}

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

int main(){
  int MYWKP = -1;
  struct player * list = malloc(sizeof(struct player) * 8);
  char buff[512];
  int current = 0;
  printf("Looking for clients? input y/n\n");
  while (fgets(buff, 511, stdin)){
    printf("%s\n", buff);
    if (buff[0] == 'y'){
      list[current].downstream = server_handshake(&MYWKP);
      current++;
      printf("%d\n", current);
    }
    if (buff[0] == 'n'){
      break;
    }
    printf("Looking for clients? input y/n\n");
  }
  int message = 200;
  for (int i = 0; i < current; i ++){
    printf("%d\n",list[i].downstream);
    write(list[i].downstream, &message, 4);
  }
  free(list);
  return 0;
}

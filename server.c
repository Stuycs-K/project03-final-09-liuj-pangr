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
#define ALIVE 1
#define DEAD 0
int main(){
  int MYWKP = -1;
  struct player * list = malloc(sizeof(struct player) * 8);
  char buff[LINE_SIZE];
  int current = 0;
  printf("Looking for clients? input y/n\n");
  int connectCode = CONNECTED;
  while (fgets(buff, 511, stdin)){
    if (buff[0] == 'y'){
      list[current].downstream = server_handshake(&MYWKP);
      write(list[current].downstream, &connectCode, 4);
      current++;
      list[current].status = ALIVE;
    }
    if (buff[0] == 'n'){
      break;
    }
    printf("Looking for clients? input y/n\n");
  }
  connectCode = READY;
  int alive = current;
  char buffplayers[current][20];
  for (int i = 0; i < current; i ++){
    if (list[i].status == ALIVE){
      write(list[i].downstream, &connectCode, 4);
      read(MYWKP, buffplayers[i], 19);
    }
  }
  for (int i = 0; i < current; i +=2){
    fight(buffplayers[i], buffplayers[i+1]);
  }

  char p1;
  char p2;
  char win;
  for (int i = 0; i < current-1; i++) { // TODO: Manage multiple pairs
    read(MYWKP, buff, 511);
    p1 = buff[0];
    read(MYWKP, buff, 511);
    p2 = buff[0];
    snprintf(buff, 512, "P1 chose %c. P2 chose %c.", p1, p2);
    printf("%s\n", buff);
    win = fight(p1, p2);
    printf("Result of fight is %c.\n", win);
  }

  free(list);
  return 0;
}

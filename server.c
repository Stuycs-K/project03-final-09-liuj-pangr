#include "handshake.h"
#include "rps.h"

//outline
/*
The server first sets up the piping for WKP and waits for client pipes.

The server will ask for connections, when available, the client will be connected.
  - input y for adding new clients, n to start the game.

The server then loops through the alive players and sends them information about
the game status, and when receiving the type acknowledgement the clinets then can
input their controls.

The server will pair up the clients, and handles a bracket and loops through
until there is one suriver left, which is the winner of the game.
*/
#define ALIVE 1
#define DEAD 0
int main(){
  signal(SIGPIPE, SIGHANDLER);
  signal(SIGINT, SIGHANDLER);
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
      list[current].status = ALIVE;
      current++;
    }
    if (buff[0] == 'n'){
      printf("GAME START!\n");
      break;
    }
    printf("Looking for clients? input y/n\n");
  }
  connectCode = READY;
  int doneCode = DONE;
  int winCode = WIN;
  int loseCode = LOSE;
  int tieCode = TIE;
  int alive = current;
  char buffplayers[current][20];
  while (alive > 1){
    printf("%d players alive\n", alive);
    printf("total %d\n", current);
    for (int i = 0; i < current; i ++){
      if (list[i].status == ALIVE){
        memset(buffplayers[i], '\0', sizeof(buffplayers[i]));
        write(list[i].downstream, &connectCode, 4);
        int bytes = read(MYWKP, buffplayers[i], 19);
        if (bytes < 0) err();
      }
    }
    for (int i = 0; i < current; i ++){
      if (list[i].status != DEAD){
        for (int j = i+1; j < current; j ++){
          if (list[j].status != DEAD){
            // printf("p1 index:%d, p2 index:%d\n", i, j);
            char win = fight(buffplayers[i][0], buffplayers[j][0]);
            while(win == 't'){
              write(list[i].downstream, &tieCode, 4);
              write(list[j].downstream, &tieCode, 4);

              write(list[i].downstream, &connectCode, 4);
              int bytes = read(MYWKP, buffplayers[i], 19);
              if (bytes < 0) err();

              write(list[j].downstream, &connectCode, 4);
              bytes = read(MYWKP, buffplayers[j], 19);
              if (bytes < 0) err();

              win = fight(buffplayers[i][0], buffplayers[j][0]);
            }
            if (win == '1') {
              list[j].status = DEAD;
              write(list[j].downstream, &loseCode, 4);
              write(list[i].downstream, &winCode, 4);
              alive --;
            }
            else {
              list[i].status = DEAD;
              write(list[j].downstream, &winCode, 4);
              write(list[i].downstream, &loseCode, 4);
              alive --;
            }
            // printf("Result of fight is %c.\n", win);
            i = j;
            j = current;
          }
        }
      }
    }
  }

  printf("Game finished.\n");
  for (int i = 0; i < current; i ++){
    if (list[i].status == ALIVE){
      write(list[i].downstream, &doneCode, 4);
    }
  }
  free(list);
  remove(WKP);
  return 0;
}

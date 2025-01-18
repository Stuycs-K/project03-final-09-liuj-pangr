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
  fd_set playerFds;
  FD_ZERO(&playerFds);
  int MYWKP = -1;
  struct player * list = malloc(sizeof(struct player) * 8);
  char buff[LINE_SIZE];
  int current = 0;
  printf("Looking for clients? input y/n\n");
  int connectCode = CONNECTED;
  while (fgets(buff, 511, stdin)){
    if (buff[0] == 'y'){
      list[current].downstream = server_handshake(&MYWKP);
      list[current].upstream = MYWKP;
      write(list[current].downstream, &connectCode, 4);
      list[current].status = ALIVE;
      list[current].paired = 0;
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
  int passCode = PASS;
  int alive = current;
  char buffplayers[current][20];
  
  while (alive > 1){
    printf("%d players alive\n", alive);
    printf("total %d\n", current);
    int passed = -1;
    if (alive % 2 != 0){
      for (int i = 0; i < current; i ++){
        if (list[i].status == ALIVE){
          write(list[i].downstream, &passCode, 4);
          passed = i;
          break;
        }
      }
    }
    
    for (int i = 0; i < current; i ++){
      if (list[i].status == ALIVE && i != passed){
        memset(buffplayers[i], '\0', sizeof(buffplayers[i]));
        write(list[i].downstream, &connectCode, 4);
      }
    }
    passed = -1;
    
    int activePlayers[2];
    int ready = 0;
    int AWAIT = 0;
    int grouped = 0;
    while(grouped < alive){
      while (ready < 2){
        FD_ZERO(&playerFds);
        if (AWAIT == 0){
          for (int i = 0; i < current; i ++){
            if (list[i].status == ALIVE && list[i].paired == 0) FD_SET(list[i].upstream, &playerFds);
          }
          AWAIT = 1;
          int selectStatus = select(list[current-1].upstream + 1, &playerFds, NULL, NULL, NULL);
          if (selectStatus < 0) err();
        }

        for (int i = 0; i < current; i ++){
          if (list[i].paired == 0 && FD_ISSET(list[i].upstream, &playerFds)){
            AWAIT = 0;
            read(list[i].upstream, buffplayers[i], 19);
            activePlayers[ready] = i;
            ready++;
            break;
          }
        }
      }

      list[activePlayers[0]].paired = 1;
      list[activePlayers[1]].paired = 1;
      grouped += 2;
      printf("%d fighting %d\n", activePlayers[0], activePlayers[1]);
      char win = fight(buffplayers[activePlayers[0]][0], buffplayers[activePlayers[1]][0]);
      while(win == 't'){
        fd_set tieSet;
        FD_ZERO(&tieSet);
        write(list[activePlayers[0]].downstream, &tieCode, 4);
        write(list[activePlayers[1]].downstream, &tieCode, 4);
        write(list[activePlayers[0]].downstream, &connectCode, 4);
        write(list[activePlayers[1]].downstream, &connectCode, 4);

        int tieReady = 0;
        while(tieReady < 2){
          FD_SET(list[activePlayers[0]].upstream, &tieSet);
          FD_SET(list[activePlayers[1]].upstream, &tieSet);
          int selectStatus = select(list[current-1].upstream + 1, &tieSet, NULL, NULL, NULL);
          if (selectStatus < 0) err();
          for (int i = 0; i < current; i ++){
            if (FD_ISSET(list[i].upstream, &tieSet)){
              read(list[i].upstream, buffplayers[i], 19);
              tieReady++;
              break;
            }
          }
        }
        win = fight(buffplayers[activePlayers[0]][0], buffplayers[activePlayers[1]][0]);
      }
      if (win == '1') {
        list[activePlayers[1]].status = DEAD;
        write(list[activePlayers[1]].downstream, &loseCode, 4);
        write(list[activePlayers[0]].downstream, &winCode, 4);
        alive --;
      }
      else {
        list[activePlayers[0]].status = DEAD;
        write(list[activePlayers[1]].downstream, &winCode, 4);
        write(list[activePlayers[0]].downstream, &loseCode, 4);
        alive --;
      }
      if (alive < 2) break;
      ready = 0;
      activePlayers[1] = -1;
      activePlayers[0] = -1;
      for (int i = 0; i < current; i ++){
        if (list[i].status == ALIVE){
          memset(buffplayers[i], '\0', sizeof(buffplayers[i]));
        }
      }
      printf("fight complete.\n");
    }
    grouped = 0;
    for (int i = 0; i < current; i ++){
      if (list[i].status == ALIVE) list[i].paired = 0;
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

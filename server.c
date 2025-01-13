#include "handshake.h"
#include "rps.h"

	Think of how merge sort is implemented
 */
#define ALIVE 1
#define DEAD 0
int main(){
  signal(SIGPIPE, SIGHANDLER);
  signal(SIGINT, SIGHANDLER);
  int MYWKP = -1;
  struct player * list = malloc(sizeof(struct player) * 8);
  fd_set active_fds;
  fd_set backup_fds;
  char buff[LINE_SIZE];
  int current = 0;

  FD_ZERO(&active_fds);

  printf("Looking for clients? input y/n\n");
  int connectCode = CONNECTED;
  while (fgets(buff, 511, stdin)){
    if (buff[0] == 'y'){
      MYWKP = server_setup();
      FD_SET(MYWKP, &backup_fds);
      list[current].downstream = server_handshake(&MYWKP);
      list[current].upstream = MYWKP;
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
        if (bytes < 0){
          printf("read err");
          exit(0);
        }
      }
    }
    for (int i = 0; i < current; i ++){
      if (list[i].status == DEAD){
        //nothing
      }
      else {
        for (int j = i+1; j < current; j ++){
          if (list[j].status == DEAD){
            //nothing
          }
          else {
            printf("p1 index:%d, p2 index:%d\n", i, j);
            char win = fight(buffplayers[i][0], buffplayers[j][0]);
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
            printf("Result of fight is %c.\n", win);
            i = j;
            j = current;
          }
        }
      }
    }
  }
  for (int i = 0; i < current; i ++){
    printf("%d\n", list[i].status);
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

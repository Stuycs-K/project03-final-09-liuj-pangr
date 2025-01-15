#include "handshake.h"
#include "rps.h"
#define ALIVE 1
#define DEAD 0

int getPlayer(fd_set* active_fds, fd_set* backup_fds, int maxFD) {
  FD_ZERO(active_fds);

  // https://stackoverflow.com/questions/3661285/how-to-iterate-through-a-fd-set
  // *active_fds = *backup_fds;
  memcpy(active_fds, backup_fds, sizeof(*backup_fds));

  int selID = select(maxFD+1, active_fds, NULL, NULL, NULL); // add timeval later
  if (selID < 0) {
    printf("%s\n", strerror(errno));
    exit(errno);
  }
  for (int i = 3; i <= maxFD; i++) {
    if (FD_ISSET(i, active_fds)) {
      int player_fd = i;
      // *backup_fds = *active_fds;
      // FD_ZERO(active_fds);
      // *active_fds = *backup_fds;
      return player_fd;
    }
  }
  return -1;
}

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
  int maxFD = 2;
  while (fgets(buff, 511, stdin)){
    if (buff[0] == 'y'){
      list[current].downstream = server_handshake(&MYWKP);
      list[current].upstream = MYWKP;
      printf("WKPFD: %d\n", MYWKP);
      FD_SET(list[current].upstream, &active_fds);
      FD_SET(list[current].upstream, &backup_fds);
      if (write(list[current].downstream, &connectCode, 4) < 0) {
        printf("read err");
        exit(0);
      }
      list[current].status = ALIVE;
      maxFD += 2;
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
    for (int i = 0; i < current; i++) {
      memset(buffplayers[i], '\0', sizeof(buffplayers[i]));
      write(list[i].downstream, &connectCode, 4);
    }
    for (int i = 0; i < alive/2; i ++){
      // if (list[i].status == ALIVE){
      //   memset(buffplayers[i], '\0', sizeof(buffplayers[i]));
      //   write(list[i].downstream, &connectCode, 4);
      //   int bytes = read(MYWKP, buffplayers[i], 19);
      //   if (bytes < 0){
      //     printf("read err");
      //     exit(0);
      //   }
      // }
      int player1FD = getPlayer(&active_fds, &backup_fds, maxFD);
      printf("%d\n", player1FD);
      int player1I = 0;
      for (int j = 0; j < current; j++) {
        if (list[j].upstream == player1FD && list[j].status == ALIVE){
          int bytes = read(list[j].upstream, buffplayers[j], 19);
          if (bytes < 0){
            printf("read err");
            exit(0);
          }
          printf("P1 received %s\n", buffplayers[j]);
          player1I = j;
          break;
        }
      }

      int player2FD = getPlayer(&active_fds, &backup_fds, maxFD);
      printf("%d\n", player2FD);
      int player2I = 0;
      for (int j = 0; j < current; j++) {
        if (list[j].upstream == player2FD && list[j].status == ALIVE){
          int bytes = read(list[j].upstream, buffplayers[j], 19);
          if (bytes < 0){
            printf("read err");
            exit(0);
          }
          printf("P2 received %s\n", buffplayers[j]);
          player2I = j;
          break;
        }
      }

      int i = player1FD;
      int j = player2FD;
      printf("p1 index:%d, p2 index:%d\n", i, j);

      char win = fight(buffplayers[i][0], buffplayers[j][0]); // FIXME: Fight doesn't work!
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
    }
    // for (int i = 0; i < current; i ++){
    //   if (list[i].status == DEAD){
    //     //nothing
    //   }
    //   else {
    //     for (int j = i+1; j < current; j ++){
    //       if (list[j].status == DEAD){
    //         //nothing
    //       }
    //       else {
    //         printf("p1 index:%d, p2 index:%d\n", i, j);
    //         char win = fight(buffplayers[i][0], buffplayers[j][0]);
    //         if (win == '1') {
    //           list[j].status = DEAD;
    //           write(list[j].downstream, &loseCode, 4);
    //           write(list[i].downstream, &winCode, 4);
    //           alive --;
    //         }
    //         else {
    //           list[i].status = DEAD;
    //           write(list[j].downstream, &winCode, 4);
    //           write(list[i].downstream, &loseCode, 4);
    //           alive --;
    //         }
    //         printf("Result of fight is %c.\n", win);
    //         i = j;
    //         j = current;
    //       }
    //     }
    //   }
    // }
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

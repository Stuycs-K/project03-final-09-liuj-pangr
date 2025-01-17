// TODO: group 13
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
int getPlayer(fd_set* active_fds, int maxFD) {
  // for (int i = 0; i < maxFD+1; i++) {
  //   if (FD_ISSET(i, active_fds)) {
  //     printf("???: %d\n", i);
  //   }
  // }


  int selID = select(maxFD+1, active_fds, NULL, NULL, NULL); // add timeval later
  if (selID < 0) err();
  for (int i = 3; i <= maxFD; i++) {
    if (FD_ISSET(i, active_fds)) {
      int player_fd = i;
      // *backup_fds = *active_fds;
      // FD_ZERO(active_fds);
      // *active_fds = *backup_fds;
      printf("SELECTED: %d\n", i);
      return player_fd;
    }
  }
  return -1;
}

int main(){
  // signal(SIGPIPE, SIGHANDLER);
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
      if (write(list[current].downstream, &connectCode, 4) < 0) err();
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
  int tieCode = TIE;
  int alive = current;
  char buffplayers[current][20];
  while (alive > 1){
  	printf("========== ROUND START ==========\n");
    printf("%d players alive\n", alive);
    printf("total %d\n", current);
    // for (int i = 0; i < current; i++) {
    //   memset(buffplayers[i], '\0', sizeof(buffplayers[i]));
    //   write(list[i].downstream, &connectCode, 4);
    // }

    for (int i = 0; i < current; i++) {
    	if(list[i].status == SENT) {
    		list[i].status = ALIVE;
        printf("%d\n", i);
    	}
      if(list[i].status == ALIVE) {
        memset(buffplayers[i], '\0', sizeof(buffplayers[i]));
        write(list[i].downstream, &connectCode, 4);
      }
    }
    int notSent = alive;
    while(notSent > 0){
      // if (list[i].status == ALIVE){
      //   memset(buffplayers[i], '\0', sizeof(buffplayers[i]));
      //   write(list[i].downstream, &connectCode, 4);
      //   int bytes = read(MYWKP, buffplayers[i], 19);
      //   if (bytes < 0){
      //     printf("read err");
      //     exit(0);
      //   }
      // }
      int i = 0;
      int j = 0;

      FD_ZERO(&active_fds);
      for (int q = 0; q < current; q++) {
        if(list[q].status == DEAD) {
          printf("THIS IS DEAD: %d\n", q);
        }
        if(list[q].status == SENT) {
          printf("THIS IS SENT: %d\n", q);
        }
        if(list[q].status == ALIVE) {
          FD_SET(list[q].upstream, &active_fds);
          printf("ADDED %d\n", list[q].upstream);
        }
      }

      //
      int player1FD = getPlayer(&active_fds, maxFD);
      printf("%d\n", player1FD);
      for (int x = 0; x < current; x++) {
        if (list[x].upstream == player1FD && list[x].status == ALIVE){
          list[x].status = SENT;
          int bytes = read(list[x].upstream, buffplayers[x], 19);
          if (bytes < 0) err();
          printf("P1 received %s\n", buffplayers[x]);
          i = x;
          // printf("i: %d\n", i);
          // printf("CLEARED %d\n", player1FD);
          break;
        }
      }

      FD_ZERO(&active_fds);
      for (int q = 0; q < current; q++) {
        if(list[q].status == ALIVE) {
          FD_SET(list[q].upstream, &active_fds);
          printf("ADDED %d\n", list[q].upstream);
        }
      }

      int player2FD = getPlayer(&active_fds, maxFD);
      printf("%d\n", player2FD);
      for (int x = 0; x < current; x++) {
        if (list[x].upstream == player2FD && list[x].status == ALIVE){
          list[x].status = SENT;
          int bytes = read(list[x].upstream, buffplayers[x], 19);
          if (bytes < 0) err();
          printf("P2 received %s\n", buffplayers[x]);
          j = x;
          // printf("j: %d\n", j);
          break;
        }
      }
      printf("p1 index:%d, p2 index:%d\n", i, j);

      char win = fight(buffplayers[i][0], buffplayers[j][0]);
      while(win == 't'){
        write(list[i].downstream, &tieCode, 4);
        write(list[j].downstream, &tieCode, 4);

        write(list[i].downstream, &connectCode, 4);
        // int bytes = read(list[i].upstream, buffplayers[i], 19);
        // if (bytes < 0) err();

        write(list[j].downstream, &connectCode, 4);
        // bytes = read(list[j].upstream, buffplayers[j], 19);
        // if (bytes < 0) err();

        // win = fight(buffplayers[i][0], buffplayers[j][0]);

        FD_ZERO(&active_fds);
        FD_SET(player1FD, &active_fds);
        FD_SET(player2FD, &active_fds);
        int player = getPlayer(&active_fds, maxFD);
        // player = first person
        for (int x = 0; x < current; x++) {
          if(list[x].upstream == player && list[x].status == ALIVE) {
            list[x].status = SENT;
            int bytes = read(list[x].upstream, buffplayers[x], 19);
            if (bytes < 0) err();
          }
        }
        if(player == player1FD) {

        }
        break;
      }
      if(win == 't') {
        continue;
      }
      if (win == '1') {
        list[j].status = DEAD;
        write(list[j].downstream, &loseCode, 4);
        write(list[i].downstream, &winCode, 4);
        alive --;
        notSent-=2;
      }
      else {
        list[i].status = DEAD;
        write(list[j].downstream, &winCode, 4);
        write(list[i].downstream, &loseCode, 4);
        alive --;
        notSent-=2;
      }
      printf("===== Result of fight is %c. =====\n", win);
    }
    printf("========== ROUND COMPLETE ==========\n");
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
    if (list[i].status == SENT){
      write(list[i].downstream, &doneCode, 4);
    }
  }
  free(list);
  remove(WKP);
  return 0;
}

#include "handshake.h"
#include "rps.h"

int getPlayer(fd_set active_fds, fd_set backup_fds) {
  FD_ZERO(&active_fds);

  // https://stackoverflow.com/questions/3661285/how-to-iterate-through-a-fd-set
  for (int i = 0; i < sizeof(backup_fds); i++) {
    active_fds.fd_array[i] = backup_fds.fd_array[i];
  }

  selID = select(sizeof(backup_fds)+1, &active_fds, NULL, NULL, 30);
  for (int i = 0; i < sizeof(backup_fds); i++) {
    if (FD_ISSET(backup_fds.fd_array[i], &active_fds)) {
      // client_socket = server_connect(listen_socket);
      int player_fd = backup_fds.fd_array[i];
      FD_ZERO(&backup_fds);
      for (int i = 0; i < sizeof(backup_fds); i++) {
        backup_fds.fd_array[i] = active_fds.fd_array[i];
      }
      return player_fd;
    }
  }
  return -1;
}

int main(){
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
    printf("%s\n", buff);
    if (buff[0] == 'y'){
      &MYWKP = server_setup();
      FD_SET(&MYWKP, &backup_fds);
      list[current].downstream = server_handshake(&MYWKP);
      list[current].upstream = MYWKP;
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

  int selID;
  while(1) {
    int player1FD = getPlayer(active_fds, backup_fds);
    int player2FD = getPlayer(active_fds, backup_fds);

    char play1;
    char play2;

    for (int i = 0; i < 8; i++) {
      if(list[i].upstream == player1FD) {
        read(player1FD, buff, 511);
        play1 = buff[0];
        break;
      }
    }
    for (int i = 0; i < 8; i++) {
      if(list[i].upstream == player2FD) {
        read(player2FD, buff, 511);
        play2 = buff[0];
        break;
      }
    }

    char win;
    win = fight(p1, p2);
    printf("Result of fight is %c.\n", win);
  }

  // char p1;
  // char p2;
  // char win;
  // for (int i = 0; i < current-1; i++) { // TODO: Manage multiple pairs
  //   read(MYWKP, buff, 511);
  //   p1 = buff[0];
  //   read(MYWKP, buff, 511);
  //   p2 = buff[0];
  //   snprintf(buff, 512, "P1 chose %c. P2 chose %c.", p1, p2);
  //   printf("%s\n", buff);
  //   win = fight(p1, p2);
  //   printf("Result of fight is %c.\n", win);
  // }

  free(list);
  return 0;
}

#include "handshake.h"

void SIGHANDLER(int signo){//sighandler
  if (signo == SIGINT){
    remove(WKP);
    exit(0);
  }
  if (signo == SIGPIPE){
    printf("client left.");
  }
}


int * client_handshake(int * addr){ //client handshake
  char str[512];
  int pid = getpid();
  sprintf(str, "%d", pid);
  mkfifo(str, 0666);
  addr[0] = open(WKP, O_WRONLY);
  write(addr[0], &pid, 4);
  addr[1] = open(str, O_RDONLY);
  remove(str);
  int ack;
  return addr;
}
//addr[0] = from client/to server, addr[1] = to client/from server
int * server_handshake(int * addr){ //server handshaking
  mkfifo(WKP, 0666);
  addr[0] = open(WKP, O_RDONLY);
  remove(WKP);
  int childID = fork();
  if (childID == 0){
    signal(SIGPIPE, SIGHANDLER);
    read(addr[0], &addr[1], 4);
  }
  return addr;
}

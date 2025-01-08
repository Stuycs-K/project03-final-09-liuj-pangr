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
  char str[LINE_SIZE];
  int pid = getpid();
  sprintf(str, "%d", pid);
  mkfifo(str, 0666);
  printf("made pipe %s\n", str);
  addr[0] = open(WKP, O_WRONLY);
  printf("server pipe online\n");
  write(addr[0], &pid, 4);
  addr[1] = open(str, O_RDONLY);
  remove(str);
  int ack;
  if (read(addr[1], &ack, 4) < 0) printf("%s\n", strerror(errno));
  if (ack == pid + 1){
    pid -= 1;
    write(addr[0], &pid, 4);
  }
  return addr;
}
//addr[0] = from client/to server, addr[1] = to client/from server
int * server_handshake(int * addr){ //server handshaking
  mkfifo(WKP, 0666);
  addr[0] = open(WKP, O_RDONLY);
  remove(WKP);
  int childID = fork();
  if (childID != 0){
    signal(SIGPIPE, SIGHANDLER);
    read(addr[0], &addr[1], 4);
    char str[LINE_SIZE];
    sprintf(str, "%d", addr[1]);
    printf("%s received\n", str);
    int ack = addr[1] + 1;
    printf("server sent %d\n", ack);
    addr[1] = open(str, O_WRONLY);
    write(addr[1], &ack, 4);
    int finalAck;
    read(addr[0], &finalAck, 4);
    if (finalAck != ack - 2){
      printf("Ack incorrect.");
      exit(0);
    }
  }
  return addr;
}

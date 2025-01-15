#include "handshake.h"
#include "rps.h"

void err(){ //err code
  printf("error %d\n", errno);
  printf("%s\n", strerror(errno));
  exit(1);
}

void SIGHANDLER(int signo){ //sighandler
  if (signo == SIGINT){
    remove(WKP);
    exit(0);
  }
  if (signo == SIGPIPE){
    printf("client left.");
  }
}


int client_handshake(int * myPipe){ //client handshake
  int addr = -1;
  addr = open(WKP, O_WRONLY);
  if (addr < 0) {
    printf("Server pipe offline.\n");
    printf("%s\n", strerror(errno));
    exit(0);
  }
  printf("Server pipe online.\n");
  char str[LINE_SIZE];
  int pid = getpid();
  sprintf(str, "%d", pid);
  mkfifo(str, 0777);
  chmod(str, 0777);
  printf("Pipe %s created.\n", str);
  if (write(addr, &pid, 4) < 0) {
    printf("%s\n", strerror(errno));
    exit(errno);
  }
  * myPipe = open(str, O_RDONLY);
  if (*myPipe < 0) {
    printf("%s\n", strerror(errno));
    exit(errno);
  }
  remove(str);
  int ack;
  if (read(* myPipe, &ack, 4) < 0) {
    printf("%s\n", strerror(errno));
    exit(errno);
  }
  if (ack == pid + 1){
    pid -= 1;
    if (write(addr, &pid, 4) < 0) {
      printf("%s\n", strerror(errno));
      exit(errno);
    }
  }
  return addr;
}

int server_setup() { //sets up the WKP
  int addr = -1;
  mkfifo(WKP, 0777);
  chmod(WKP, 0777);

  addr = open(WKP, O_RDONLY);
  if (addr < 0){
    printf("line 39 error\n");
    exit(errno);
  }
  return addr;
}

//THEWKP = from client/to server, addr = to client/from server
int server_handshake(int * THEWKP){ //server handshaking
  int addr = -1;
  // signal(SIGPIPE, SIGHANDLER);
  *THEWKP = server_setup();
  int bytes = read(*THEWKP, &addr, 4);
  if (bytes < 0) {
    printf("%s\n", strerror(errno));
    exit(errno);
  }
  unlink(WKP);
  char str[LINE_SIZE];
  sprintf(str, "%d", addr);
  printf("client %s received\n", str);
  int ack = addr + 1;
  printf("acknowledgement sent\n");
  addr = open(str, O_WRONLY);
  if (addr < 0) {
    printf("%s\n", strerror(errno));
    exit(errno);
  }
  bytes = write(addr, &ack, 4);
  if (bytes < 0) {
    printf("%s\n", strerror(errno));
    exit(errno);
  }
  int finalAck;
  bytes = read(*THEWKP, &finalAck, 4);
  if (bytes < 0) {
    printf("%s\n", strerror(errno));
    exit(errno);
  }
  if (finalAck != ack - 2){
    printf("Ack incorrect.\n");
    exit(0);
  }
  if (addr < 0){
    printf("line 65 error\n");
    exit(0);
  }
  return addr;
}

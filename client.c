#include "handshake.h"
#include "rps.h"
//addr[0] = from client/to server, addr[1] = to client/from server
int main(){
  int myPipe = -1;
  int serverPipe = -1;
  serverPipe = client_handshake(&myPipe);
  if (serverPipe < 0){
    printf("serverpipe error\n");
    exit(0);
  }
  int message = -1;
  while(1){
    read(myPipe, &message, 4);
    if (message == CONNECTED){
      printf("You have been connected! Please wait.\n");
    }
    else if (message == READY) {
      char p = takeInput();
      write(serverPipe, &p, sizeof(char));
    }
    else if (message == DONE) {
      // read player 1
      // read player 2
      // read message
      if (message == WIN) {
        // win condition
      }
      else {
        // loss condition
      }
    }
    message = -1;
  }

  // char p1 = takeInput();
  // char p2 = takeInput();
  // printf("P1 chose %c. P2 chose %c.\n", p1, p2);
  // printf("Result of fight is %c.\n", fight(p1,p2));
  // return 0;
}

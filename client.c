#include "handshake.h"
#include "rps.h"
//addr[0] = from client/to server, addr[1] = to client/from server
int main(){
  int adr[2];
  int * addr = adr;
  addr = client_handshake(addr);
  int check = 100;
  write(addr[0], &check, 4);
  read(addr[1], &check, 4);
  printf("%d\n", check);

  char p1 = takeInput();
  char p2 = takeInput();
  printf("P1 chose %c. P2 chose %c.\n", p1, p2);
  printf("Result of fight is %c.\n", fight(p1,p2));
  return 0;
}

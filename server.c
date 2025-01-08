#include "handshake.h"
#include "rps.h"
//addr[0] = from client/to server, addr[1] = to client/from server
int handleBracket(){
  return 0;
}
int main(){
  int adr[2];
  int * addr = adr;
  addr = server_handshake(addr);
  int check = 300;
  write(addr[0], &check, 4);
  read(addr[1], &check, 4);
  printf("%d\n", check);
  return 0;
}

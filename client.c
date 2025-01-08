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
  return 0;
}

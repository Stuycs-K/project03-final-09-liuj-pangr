#include "handshake.h"
#include "rps.h"
//addr[0] = from client/to server, addr[1] = to client/from server
int handleBracket(int * addr){

  /* Outline
  The server waits until two clients join.
  The server forks.
  The server will receive single client.
  If the server doesn't receive another connection, this client is the winner.
  If the server receives a connection, repeat steps.

  The subserver waits until its subservers terminate and two clients have a connection.
  The subserver blocks until both clients have submitted their results.
  The subserver compares the two results, writing something to the loser client for the client to terminate.
  The subserver returns the fd of the winner client to the higher server, before terminating.

  */

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

  handleBracket(addr);
  return 0;
}

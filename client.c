#include "handshake.h"
#include "rps.h"

int main(){
  int to_server;
  int from_server = client_handshake( &to_server );

  char choice = takeInput()l
  int bytes = write(to_server, &choice, sizeof(char));
  // write to WKP

  //

  return 0;
}

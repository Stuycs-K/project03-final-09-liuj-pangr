#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#ifndef HANDSHAKE_H
#define WKP "serverPipe"

int server_handshake(int * addr);
int client_handshake(int * addr);
void SIGHANDLER(int signo);

#endif

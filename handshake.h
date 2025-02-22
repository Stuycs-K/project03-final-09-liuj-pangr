#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#define WKP "serverPipe"
#define LINE_SIZE 512

#ifndef HANDSHAKE_H
#define HANDSHAKE_H

void err();
int server_setup();
int server_handshake(int * THEWKP);
int client_handshake(int * myPipe);
void SIGHANDLER(int signo);

#endif

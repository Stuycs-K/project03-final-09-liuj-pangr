#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>

#define CONNECTED 0
#define READY 200
#define DONE 100
#define WIN 300
#define TIE 400
#define LOSE 500
#define PAIRED 1
#define UNPAIRED 0
#define ALIVE 1
#define DEAD 0
#define SENT 2

#ifndef RPS_H
#define RPS_H

struct player { int downstream; int upstream; int status; int paired;};

char takeInput();
char fight(char p1, char p2);

#endif

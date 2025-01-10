#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>

#define CONNECTED 0
#define READY 200
#define DONE 100
#define WIN 300

#ifndef RPS_H
#define RPS_H

struct player { int downstream; int upstream; int status; };

char takeInput();
char fight(char p1, char p2);

#endif

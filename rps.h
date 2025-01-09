#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#ifndef RPS_H
#define RPS_H

struct player { int downstream; int status; };

char takeInput();
char fight(char p1, char p2);

#endif

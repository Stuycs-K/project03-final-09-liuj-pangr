#include "rps.h"

char takeInput(){ //When called asks used for input
  while(1){
    printf("Please enter either 'r'(rock), 'p'(paper), or 's'(scissor)\n");
    char input[256];
    fgets(input, 255, stdin);
    int length = strlen(sentence);
    if(length > 1){
      printf("Invalid arguement: too long\n");
    }
    char attack = input[0];
    if(strcmp("r", attack) == 0 || strcmp("p", attack) == 0 || strcmp("s", attack) == 0){
      return attack;
    }
    else{
      printf("Invalid arguement: not 'r'(rock), 'p'(paper), or 's'(scissor)\n")
    }
  }
  return NULL;
}

char fight(char p1, char p2){ //Finds winner of RPS game

  return NULL;
}

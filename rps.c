#include "rps.h"

char takeInput(){ //When called asks used for input
  while(1){
    printf("Please enter either 'r'(rock), 'p'(paper), or 's'(scissor)\n");
    char input[256];
    fgets(input, 255, stdin);
    int length = strlen(input);
    if(length > 2){
      printf("Invalid arguement: too long\n");
    }
    else{
      char attack = input[0];
      if(attack == 'r' || attack == 'p' || attack == 's'){
        return attack;
      }
      else{
        printf("Invalid arguement: not 'r'(rock), 'p'(paper), or 's'(scissor)\n");
      }
    }
  }
  return 'f';
}

char fight(char p1, char p2){ //Finds winner of RPS game
  if('r' == p1){
    if('r' == p2){
      return 't';
    }
    if('p' == p2){
      return '2';
    }else{
      return '1';
    }
  }
  if('p' == p1){
    if('p' == p2){
      return 't';
    }
    if('s' == p2){
      return '2';
    }else{
      return '1';
    }
  }
  if('s' == p1){
    if('s' == p2){
      return 't';
    }
    if('r' == p2){
      return '2';
    }else{
      return '1';
    }
  }
  return 'f';
}

int main(){ // testing temp
  char p1 = takeInput();
  char p2 = takeInput();
  printf("P1 chose %c. P2 chose %c.\n", p1, p2);
  printf("Result of fight is %c.\n", fight(p1,p2));
}
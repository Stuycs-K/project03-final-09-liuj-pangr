[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Vh67aNdh)
# ABOVEGROUND SPR TOURNAMENT

### Senatus PopulusQue Romanus (Scissor Paper Rock)

Jason Liu, Renxuan Pang, Steven Luo

### Project Description:

A server that creates a tournament between joining clients, which will
initiate a thrilling tournament of rock paper scissors.

### Instructions:

How does the user install/compile/run the program.

The server machine will clone the repo inside their /tmp folder,
  Then make -> make server
Client machines ssh onto server machine, find the project folder, make client

How does the user interact with this program?

The server will type y if a new client wants to join, and n to start the game
(on the server terminal).

When prompted to type their choices for RPS, do so as prompt says.

## Some Behaviors of note:
DEFAULT MAX PLAYERS: 8
You can start a tournament with any amount of players less than/equal to max. 

You should be able to add more players to the game by changing the size of list in line 26 of server.c

If the game ends up resulting in an odd number of players, the last player to participate will get a free pass, skipping to the next phase after inputting their response.

NOTE: AS LONG AS CLIENTS FOLLOW INSTRUCTIONS ON THE TERMINAL, THERE SHOULDN'T BE ISSUES.



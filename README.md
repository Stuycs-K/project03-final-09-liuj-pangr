[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Vh67aNdh)
# ABOVEGROUND SPR TOURNAMENT

### Senatus PopulusQue Romanus (Scissor Paper Rock)

Jason Liu, Renxuan Pang, Steven Luo

### Project Description:

A server that creates a tournament between joining clients, which will initiate a thrilling tournament of rock paper scissors (RPS).

## Link to Presentation
[https://youtu.be/G2dp3XVbBdk](https://youtu.be/G2dp3XVbBdk)

## How does the user install/compile/run the program.

Server Machine:
1) Clone the repo inside their /tmp folder.
2) Run "make".
3) Run "make server".

Client Machines:
1) SSH onto the server machine.
2) CD into the project folder in /tmp.
3) Run "make client".
   * If the output says that the server pipe is offline, that may mean the server is not taking clients yet.
   * Wait until the person hosting the server tells you to join or, if you really want to, spam "make client" until you join.

## How does the user interact with this program?

Server Machine:
* Type y to let a new client join, and n to start the game.

Client Machine:
* When prompted to type their choices for RPS, do so as prompt says.

## Important Behaviors:
DEFAULT MAX PLAYERS: 8
* You can start a tournament with any amount of players less than/equal to max.
* You should be able to add more players to the game by changing the number in Line 2 of server.c

If the game ends up resulting in an odd number of players, the last player to participate will get a free pass, skipping to the next phase after inputting their response.

NOTE: AS LONG AS CLIENTS FOLLOW INSTRUCTIONS ON THE TERMINAL, THERE SHOULDN'T BE ISSUES.

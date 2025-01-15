# Final Project Proposal

## Group Members:

Jason Liu, Renxuan Pang, Steven Luo

# Intentions:

Rock Paper Scissors 1v1 tournament (client-server)
- A tournament bracket made with a set of pipes, with the server listening to client inputs and sending them instructions about the game.
- The game would start through a command in the server confirming the players' readiness.
- The server would malloc a list of players (structs), which stores all the player data. The results and losses of the game are calculated as the fights happen. Dead players are ignored and winners continue.

# Intended usage:

It will be text based, with the terminal taking inputs and outputting instructions to the client from the server.

- the inputs will be letters to represent the actions that the client could take.
- in the very end the tournament results will be displayed.

# Technical Details:

Processes, mallocing, Signaling, Allocating memory, pipes.
We will break this down into server-sided handling and client-sided handling.
Bracket handling, Arrays to store data, server-client communication managing, and handshake.

# Intended pacing:

We should be done with basic communications in the first 3 days, then 2 more for algorithms regarding game mechanics and bracketing, the the rest for select, documentations, optimizations, and debugging.

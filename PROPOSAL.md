# Final Project Proposal

## Group Members:

Jason Liu, Renxuan Pang

# Intentions:

Rock Paper Scissors 2-player tournament (client-server)
- A tournament bracket made with forking servers where players can input options to decide the victory and move up the bracket (managed by the server, closing the loser’s pipe).
- The game would start through a command in the server confirming the players' readiness.
- The server would malloc a list of results, first place to last place and print the results when the game is finished.

# Intended usage:

It will be text based, with the terminal taking inputs and outputting instructions to the client from the server.

- the inputs will be letters to represent the actions that the client could take.
- in the very end the tournament results will be displayed.

# Technical Details:

Processes, Piping, Signaling, Allocating memory.
We will break this down into server-sided handling and client-sided handling.
Bracket handling, Arrays to store data, server-client communication managing, and handshake.

# Intended pacing:

We should be done with basic communications in the first 3 days, then 2 more for algorithms regarding game mechanics and bracketing, the the rest for DEBUGGING

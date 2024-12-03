# Tulbat
Project of Foundamentals of Artificial Intelligence course at University of Bologna.
The goal of this repository is to implement a player for the Tablut game using heuristic evaluation functions. The agent is implemented in c++ for improved performance and deepening of the search tree.

## **Repository Structure:**

The repository is organized as follows:

- **heuristic_cpp**: contains the source code of the agent implemented in c++.
- **other**: contains the old files of the project, including the python agent and the neural network.

## **Heuristic Agent:**

The agent uses the Minimax algorithm with Alpha-Beta pruning to search the game tree. Using a heuristic evaluation function the player is capable to evaluate the game state for every possible board configuration.

## **How to run the agent:**

To compile the agent, the user must run the following commands in the terminal:

```./heuristic_cpp/start.sh```

To run the agent, the user must provide the color (white or black), the time in seconds and the ip address of the server.

```./heuristic_cpp/runmyplayer.sh <color> <time> <ip>```

> **Note:** Before running the agent, the user must start the server.

For example, to run the agent as white with 60 seconds of thinking time and the server at localhost:

```./heuristic_cpp/runmyplayer.sh white 60 127.0.0.1```
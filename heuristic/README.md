# Tablut Heuristic Strategy

## Overview

This project implements an **heuristic-based strategy** for playing the game of Tablut, a variant of the ancient Scandinavian game Hnefatafl. The game has a 9x9 board, and this strategy evaluates possible moves for a player (either **White** or **Black**) using a set of heuristic rules and a **breadth-first exploration** of game states.

The project structure is modular, allowing for flexible extension with other strategies, such as neural networks. Below is a detailed breakdown of the files and their roles in implementing the heuristic strategy.

## Heuristic Strategy

The core of the heuristic strategy is implemented in the `game_utils.py` module. The heuristic approach evaluates game moves based on predefined rules that estimate how advantageous each move is for the player. These evaluations aim to lead the game toward favorable positions while avoiding obvious pitfalls.

## Project Structure

```
tablut_project/ 
│ 
├── driver.py  
├── game_utils.py 
├── logger.py 
├── main.py 
├── player.py 
└── logs/ 
    └── game.log # Log file storing information about game events and errors.
```


## Files Descriptions

- **driver.py :** Manages the socket communication between the player and the Tablut server. It handles sending and receiving messages, ensuring that data is transmitted correctly to facilitate seamless gameplay. This script is essential for connecting the player to the server and maintaining the flow of the game.

- **game_utils.py :** Provides various utility functions that are critical for the game. It includes functionalities for generating all valid moves based on the current board state and evaluating those moves using heuristic rules. This script encapsulates the logic necessary to assess the consequences of player actions and determine optimal strategies.

- **logger.py :** Implements a centralized logging mechanism that captures important events and errors during gameplay. The logger tracks player moves, game state changes, and any errors that may occur, writing this information to a log file. This is useful for debugging and understanding the game's flow, making it easier to analyze performance and issues.

- **main.py :** Serves as the main entry point for the application. This script initializes the game environment, prompts the user for input, and manages the overall game loop. It coordinates interactions between the player, the server, and the other components, ensuring that the game progresses smoothly.

- **player.py :** Contains the Player class that represents the game participant. This script manages the player's interactions with the server, including decision-making processes for selecting moves. It encapsulates the behavior of the player, allowing for different strategies to be implemented while interacting with the game.

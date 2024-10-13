# Tablut Heuristic Strategy

## Overview

This project implements an **heuristic-based strategy** for playing the game of Tablut, a variant of the ancient Scandinavian game Hnefatafl. The game has a 9x9 board, and this strategy evaluates possible moves for a player (either **White** or **Black**) using a set of heuristic rules and a **breadth-first exploration** of game states.

The project structure is modular, allowing for flexible extension with other strategies, such as neural networks. Below is a detailed breakdown of the files and their roles in implementing the heuristic strategy.

## Project Structure

tablut_project/ │ ├── driver.py # Handles communication between the client and the Tablut server. ├── game_utils.py # Contains functions for generating and evaluating game moves based on heuristic rules. ├── logger.py # Implements logging functionalities to track game events and errors. ├── main.py # The main entry point of the application that orchestrates game execution. ├── player.py # Contains the Player class that interacts with the server and manages game flow. └── logs/ └── game.log # Log file storing information about game events and errors.

tablut_project/ │ ├── driver.py # Handles communication with the Tablut server. ├── game_utils.py # Contains functions for generating and evaluating game moves. ├── logger.py # Centralized logging configuration for the project. ├── main.py # Entry point of the application. ├── player.py # Implements the Player class and game interaction logic. └── logs/ └── game.log # Stores logs generated during game execution.


## Heuristic Strategy

The core of the heuristic strategy is implemented in the `game_utils.py` module. The heuristic approach evaluates game moves based on predefined rules that estimate how advantageous each move is for the player. These evaluations aim to lead the game toward favorable positions while avoiding obvious pitfalls.


## Function Descriptions

### `driver.py`

- **Functionality**: Manages socket communication between the player and the server.

### `game_utils.py`

- **Functionality**: Provides functions for generating valid moves and evaluating them based on heuristic strategies.

### `logger.py`

- **Functionality**: Centralized logging mechanism to record game events and errors.

### `main.py`

- **Functionality**: Initializes the game, prompting user input and controlling the game loop.

### `player.py`

- **Functionality**: Encapsulates player behavior, including decision-making and interaction with the server.
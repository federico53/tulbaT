from player import Player
from logger import logger

def main():
    try:
        player_name = input("Enter your player name: ")
        color = input("Choose your color (white/black): ").lower()

        if color not in ['white', 'black']:
            raise ValueError("You must choose 'white' or 'black' as your color.")

        if color == 'white':
            server_address = ('localhost', 5800)
        else:
            server_address = ('localhost', 5801)

        logger.info(f"Starting game for player {player_name} as {color} connecting to {server_address}")

        player = Player(player_name, color, server_address)
        player.play()

    except Exception as e:
        logger.critical(f"Failed to start the game: {e}")
        print(f"Failed to start the game: {e}")

if __name__ == "__main__":
    main()

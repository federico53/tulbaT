from driver import GameDriver
from game_utils import generate_all_possible_moves, select_best_move, convert_move_for_server
from logger import logger

class Player:
    def __init__(self, name, color, server_address):
        self.name = name
        self.color = color
        self.driver = GameDriver(color, server_address)
        logger.info(f"Player {self.name} initialized as {self.color}")

    def play(self):
        try:
            self.driver.connect(self.name)
        except Exception as e:
            logger.critical(f"Failed to connect: {e}")
            return

        while True:
            try:
                # Ricevi lo stato di gioco
                game_state = self.driver.receive_game_state()
                board = game_state['board']
                turn = game_state['turn'].lower()

                logger.info(f"Current turn: {turn}")
                logger.debug(f"Current board state: {board}")

                # Controlla se il gioco è finito
                if turn in ['whitewin', 'blackwin', 'draw']:
                    logger.info(f"Game over! Result: {turn.upper()}")
                    print(f"Game over! Result: {turn.upper()}")
                    break

                # Aspetta il turno
                if turn != self.color:
                    logger.info(f"Waiting for the opponent's turn... (Current turn: {turn})")
                    print(f"Waiting for the opponent's turn... (Current turn: {turn})")
                    continue

                # Genera tutte le mosse valide
                valid_moves = generate_all_possible_moves(board, self.color)
                logger.info(f"Valid moves generated: {len(valid_moves)}")

                if not valid_moves:
                    logger.warning("No valid moves available.")
                    print("No valid moves available.")
                    break

                # Seleziona la miglior mossa
                best_move = select_best_move(valid_moves, board, self.color)
                logger.info(f"Best move selected: {best_move}")

                move_for_server = convert_move_for_server(best_move, self.color)

                # Invia la mossa al server
                logger.info(f"Sending move: {move_for_server}")
                self.driver.send_move(move_for_server)
                print(f"Sending move: {move_for_server}")

            except Exception as e:
                logger.error(f"An error occurred during the game loop: {e}")
                break

        self.driver.close()

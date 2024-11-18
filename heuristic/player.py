from driver import GameDriver
from game_utils import minimax_alpha_beta, format_board
from logger import logger
from stats import stats_of_the_board

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
                logger.debug(f"Current board state: \n{format_board(board)}")

                # Controlla se il gioco è finito
                if turn in ['whitewin', 'blackwin', 'draw']:
                    logger.info(f"Game over! Result: {turn.upper()}")
                    break

                # Aspetta il turno
                if turn != self.color:
                    logger.info(f"Waiting for the opponent's turn... (Current turn: {turn})")
                    continue

                # Printing the board stats
                logger.debug(f"Stats of the board: {stats_of_the_board(board, self.color)}")

                # Find the best move using minmax algorithm
                best_score, best_move = minimax_alpha_beta(board, depth=1, alpha=float('-inf'), beta=float('inf'), turn=turn, player=self.color)
                logger.info(f"Best move for black: {best_move} with score: {best_score}")

                # Invia la mossa al server
                logger.info(f"Sending move: {best_move}")
                self.driver.send_move(best_move, self.color)

            except Exception as e:
                logger.error(f"An error occurred during the game loop: {e}")
                break

        self.driver.close()

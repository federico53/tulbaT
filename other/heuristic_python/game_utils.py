from logger import logger
import stats
import random
from tensorflow.keras.models import load_model


enemies = {'BLACK': ['WHITE'], 'WHITE': ['BLACK'], 'KING': ['BLACK']}
allies = {'BLACK': ['BLACK', 'THRONE'], 'WHITE': ['WHITE', 'KING', 'THRONE'], 'KING': ['WHITE', 'THRONE']}
empty = [['EMPTY' for _ in range(9)] for _ in range(9)]
empty[4][4] = 'THRONE'
citadels = [(0, 3), (0, 4), (0, 5), (1, 4), (3, 0), (3, 1), (3, 2), (4, 0), (4, 2), (4, 6), (4, 8), (5, 0)]
winning_positions = [(0, 1), (0, 2), (0, 6), (0, 7), (1, 0), (1, 8), (2, 0), (2, 8), (6, 0), (6, 8), (7, 0), (7, 8), (8, 1), (8, 2), (8, 6), (8, 7)]

# Obtaining the model from the neural network
print("Loading the model...")
model = load_model('../neuralNetwork/tablut_model_NN.keras')
print("Model loaded successfully!")

### MinMax ### 

def minimax_alpha_beta(board, depth, mode, alpha, beta, turn, player):
    '''
    Minimax algorithm with alpha-beta pruning.
    
    Args:
        board: Current board state.
        depth: Depth limit for the search tree.
        alpha: Alpha value for alpha-beta pruning.
        beta: Beta value for alpha-beta pruning.
        turn: Current turn (e.g., 'black' or 'white').
        player: Player color (e.g., 'black' or 'white').
        
    Returns:
        tuple: A tuple containing:
            - best_score: Best score for the current player.
            - best_move: Best move for the current player.
    '''
    try:
        

        # Base case: depth limit reached or game over
        if depth == 0 or is_game_over(board):
            if mode == 1:
                return heuristic_evaluation(board, turn, player), None  # Return score and no move using the heuristic
            elif mode == 2:
                return neural_network_evaluation(board, turn, player, model), None  # Return score and no move using the neural network
            #return heuristic_evaluation(board, turn, player), None  # Return score and no move

        best_move = None

        is_max = (turn == player)

        if is_max:
            max_eval = float('-inf')
            for move in generate_all_possible_moves(board, player):
                # Apply move for max
                new_board = apply_move(board, move)
                eval, _ = minimax_alpha_beta(new_board, depth - 1, mode, alpha, beta, get_opposite_turn(turn), player)
                if eval > max_eval:
                    max_eval = eval
                    best_move = move
                alpha = max(alpha, eval)
                if beta <= alpha:
                    break  # Beta cut-off
            return max_eval, best_move
        else:
            min_eval = float('inf')
            for move in generate_all_possible_moves(board, player):
                # Apply move for min
                new_board = apply_move(board, move)
                eval, _ = minimax_alpha_beta(new_board, depth - 1, mode, alpha, beta, get_opposite_turn(turn), player)
                if eval < min_eval:
                    min_eval = eval
                    best_move = move
                beta = min(beta, eval)
                if beta <= alpha:
                    break  # Alpha cut-off
            return min_eval, best_move
    except Exception as e:
        logger.error(f"Error in minimax_alpha_beta: {e}")
        raise

def is_valid_move(move, board, color):
    
    try:

        from_pos, to_pos = move
        from_row, from_col = from_pos
        to_row, to_col = to_pos

        # Controlla se la mossa è sul trono o in una citadel
        if board[to_row][to_col] != 'EMPTY':
            return False  # Non puoi muoverti su una casella già occupata
        
        if board[to_row][to_col] == 'THRONE':
            return False  # Non puoi muoverti sul trono

        # Citadel positions (as in GameAshtonTablut.java)
        citadels = ['a4', 'a5', 'a6', 'b5', 'd1', 'e1', 'f1', 'e2', 'i4', 'i5', 'i6', 'h5', 'd9', 'e9', 'f9', 'e8']
        columns = 'abcdefghi'
        to_box = columns[to_col] + str(9 - to_row)
        from_box = columns[from_col] + str(9 - from_row)

        if to_box in citadels and from_box not in citadels:
            return False  # Non puoi muoverti in una citadel da fuori

        # Controlla che non ci siano ostacoli nel percorso (le mosse non possono scavalcare altre pedine)
        if from_row == to_row:  # Movimento orizzontale
            step = 1 if to_col > from_col else -1
            for col in range(from_col + step, to_col, step):
                if board[from_row][col] != 'EMPTY':
                    return False
        elif from_col == to_col:  # Movimento verticale
            step = 1 if to_row > from_row else -1
            for row in range(from_row + step, to_row, step):
                if board[row][from_col] != 'EMPTY':
                    return False
        else:
            return False  # Il movimento in diagonale non è consentito

        # Se tutti i controlli sono passati, la mossa è valida
        return True
    
    except Exception as e:
        logger.error(f"Error in is_valid_move: {e}")
        raise

def generate_all_possible_moves(board, color):
    try:

        moves = []
        for row in range(len(board)):
            for col in range(len(board[row])):
                if (color == 'white' and board[row][col] in ['WHITE', 'KING']) or (color == 'black' and board[row][col] == 'BLACK'):
                    # Genera mosse in ogni direzione (es: muovi orizzontalmente e verticalmente)
                    directions = [(1, 0), (-1, 0), (0, 1), (0, -1)]
                    for dr, dc in directions:
                        for i in range(1, len(board)):  # Massima distanza fino al bordo
                            new_row, new_col = row + dr * i, col + dc * i
                            if 0 <= new_row < len(board) and 0 <= new_col < len(board[0]) and board[new_row][new_col] == 'EMPTY':
                                move = ((row, col), (new_row, new_col))
                                if is_valid_move(move, board, color):  # Verifica se la mossa è valida
                                    moves.append(move)
                            else:
                                break
        return moves
    
    except Exception as e:
        logger.error(f"Error in generate_all_possible_moves: {e}")
        raise




### HEURISTIC ###
def heuristic_evaluation(board, turn, player):
    '''
    Heuristic evaluation function for the current board state
    Args:
        board: current board state
        turn: current turn (black/white)
        player: player color (black/white)
    Returns:
        score: evaluation score for the current board state, negative if it's a better move for min, positive if it's a better move for max
    '''
    # Since i have already changed the turn in the minimax function, in order to choose the right heuristic i have to reason as i was in the next turn
    if turn!=player:
        return heuristic(board, turn)
    else:
        return -heuristic(board, turn)
    
def heuristic(board, turn):
    # Same as before, i have to reason as if i was in the next turn
    if turn != 'white':
        return heuristic_white(board)
    else:
        return heuristic_black(board)
    

def heuristic_white(board):
    try:
        #1. TODO
        #king position. in base a dov'è il king e alle sides del king,

        #3. black in checkmate
        points_king_checkmate = 0
        if stats.black_checkmate(board):
            points_king_checkmate = 100

        #4. difference between black eaten and white lost
        white_lost = 8 - stats.count_pieces(board, 'WHITE')
        black_eaten = 16 - stats.count_pieces(board, 'BLACK')
        diff_between_white_black = 1600*(black_eaten - white_lost)*200
        
        #5. eaten black
        score_black_eaten = 5000*black_eaten

        #6. king not blocked in quadrant 10000  
        score_king_not_blocked_quadrant = 0
        if not stats.king_block_in_quadrant(board):
            score_king_not_blocked_quadrant = 10000
        
        #7. black can't make checkmate in one move
        score_king_not_checkmated_infuture = 0
        if not stats.black_can_checkmate_in_future(board):
            score_king_not_checkmated_infuture = 20000
        
        #8. king not in checkmate from black
        score_king_not_checkmated = 0
        if not stats.white_checkmate(board):
            score_king_not_checkmated = 40000

        # 9. If black has not won
        white_didnt_win_points = 0
        if not is_game_over(board) == 'black':
            white_didnt_win_points = 80000

        # 10. If white has won
        black_won_points = 0
        if is_game_over(board) == 'white':
            black_won_points = 160000

        #TODO AGGIUNGERE I PUNTEGGI NUOVI CHE PRIMA ERANO COMMENTATI
        return (points_king_checkmate + diff_between_white_black + score_black_eaten + score_king_not_blocked_quadrant + score_king_not_checkmated_infuture + score_king_not_checkmated + white_didnt_win_points + black_won_points)


    except Exception as e:
        logger.error(f"Error in heuristic_white: {e}")

#obsolete
""" def heuristic_white(board):
    try:
        # Points for each eaten black piece
        eaten_blacks_points = (16 - stats.count_pieces(board, 'BLACK')) * 10

        # Points for the sides of the king
        free_sides, blocked_sides, black_blockers, white_blockers, castle_blockers = stats.king_free_sides(board)
        white_blockers_points = white_blockers * 20
        free_sides_points = free_sides * 10
        castle_blockers_points = castle_blockers * -5

        # Points for the position of the king
        winning_positioning_points = stats.king_winning_direction_score(board) * 15

        # Points for the checkmate
        black_in_check_mate_points = 0
        if stats.black_checkmate(board):
            black_in_check_mate_points = 50

        return eaten_blacks_points + white_blockers_points + free_sides_points + castle_blockers_points + winning_positioning_points + black_in_check_mate_points
    except Exception as e:
        logger.error(f"Error in heuristic_white: {e}")
        raise """

#support function
def calculate_black_starting_positions_points(board):
    """
    Calculate the points for black pieces based on their starting positions on the board.

    Returns:
        int: The total points for black pieces based on their starting positions.
    Raises:
        Exception: If an error occurs during the calculation.
    """
    
    try:
        points = 0

        top_positions = [(0,4), (8,4), (4,0), (4,8)]
        medium_positions = [(1,4), (7,4), (4,1), (4,7)]
        low_positions = [(0,3),(0,5),(3,0),(5,0),(3,8),(5,8),(8,3),(8,5)]

        # Assign points based on the position of the black pieces
        for row, col in top_positions:
            if board[row][col] == 'BLACK':
                points += 3

        for row, col in medium_positions:
            if board[row][col] == 'BLACK':
                points += 2

        for row, col in low_positions:
            if board[row][col] == 'BLACK':
                points += 1

        return points
    except Exception as e:
        logger.error(f"Error in calculate_black_starting_positions_points: {e}")
        raise

def heuristic_black(board):
    try:
        
        # 1. Points for the starting positions of the black pieces
        black_starting_positions_points = calculate_black_starting_positions_points(board)

        # 2. Difference between white pieces eaten and black pieces lost
        white_eaten = 8 - stats.count_pieces(board, 'WHITE')
        black_lost = 16 - stats.count_pieces(board, 'BLACK')
        diff_between_white_black = 480 + (white_eaten - black_lost) * 30

        # 3. Points for each white piece eaten
        points_white_eaten = white_eaten * 750

        # 4. If the king cannot checkmate in one move
        king_not_checkamate_inonemove = 0
        if not stats.king_can_checkmate_in_future(board):
            king_not_checkamate_inonemove = 6750

        # 5. If the king is in checkmate
        king_not_checkmate_points = 0
        if stats.white_checkmate(board):
            king_not_checkmate_points = 13500

        # 6. If black is not in checkmate
        black_checkmate_points = 0
        if not stats.black_checkmate(board):
            black_checkmate_points = 27000

        # 7. If white has not won
        white_didnt_win_points = 0
        if not is_game_over(board) == 'white':
            white_didnt_win_points = 54000

        # 8. If black has won
        black_won_points = 0
        if is_game_over(board) == 'black':
            black_won_points = 108000

        return black_starting_positions_points + diff_between_white_black + points_white_eaten + king_not_checkamate_inonemove + king_not_checkmate_points + black_checkmate_points + white_didnt_win_points + black_won_points
    except Exception as e:
        logger.error(f"Error in heuristic_black: {e}")
        raise


#OBSOLETE!!! can be deleted when we are sure that the new heuristics works
""" def heuristic_black(board):
    try:
        score = 0

        white_eaten = 8 - stats.count_pieces(board, 'WHITE')
        black_eaten = 16 - stats.count_pieces(board, 'BLACK')
        diff_between_white_black = 480 + (white_eaten - black_eaten)*30

        points_white_eaten = 750 * white_eaten

        # Points for each eaten black piece
        eaten_whites_points = (8 - stats.count_pieces(board, 'WHITE')) * 15

        # Points for the position of the king
        winning_positioning_points = stats.king_winning_direction_score(board) * 15

        # Points for the sides of the king
        free_sides, blocked_sides, black_blockers, white_blockers, castle_blockers = stats.king_free_sides(board)
        black_blockers_points = black_blockers * 10 * (1 + winning_positioning_points)
        castle_blockers_points = castle_blockers * 5

        # Points for the checkmate
        white_in_check_mate_points = 0
        if stats.white_checkmate(board):
            white_in_check_mate_points = 100

        return eaten_whites_points + black_blockers_points + castle_blockers_points + winning_positioning_points + white_in_check_mate_points
    except Exception as e:
        logger.error(f"Error in heuristic_black: {e}")
        raise
    """

### HEURISTIC WITH NEURAL NETWORK ###
def neural_network_evaluation(board, turn, player, model):
    '''
    Heuristic evaluation function for the current board state
    Args:
        board: current board state
        turn: current turn (black/white)
        player: player color (black/white)
    Returns:
        score: evaluation score for the current board state, negative if it's a better move for min, positive if it's a better move for max
    '''
    try:
        white_winning_prob = model.predict(prepare_input(board, turn))
        black_winning_prob = 1 - white_winning_prob

        # Since i have already changed the turn in the minimax function, in order to choose the right heuristic i have to reason as i was in the next turn
        if turn!=player:
            if player == 'white':
                #print(f"Turn:{turn}. Player: {player}. White winning prob: {white_winning_prob}")
                return white_winning_prob
            else:
                #print(f"Turn:{turn}. Player: {player}. Black winning prob: {black_winning_prob}")
                return black_winning_prob
        else:
            if player == 'white':
                #print(f"Turn:{turn}. Player: {player}. White winning prob: {white_winning_prob}")
                return -white_winning_prob
            else:
                #print(f"Turn:{turn}. Player: {player}. Black winning prob: {black_winning_prob}")
                return -black_winning_prob
    except Exception as e:
        logger.error(f"Error in neural_network_evaluation: {e}")
        raise

### UTILS NEURAL NETWORK ###
import numpy as np
# Funzione per convertire la board in formato numerico
def convert_board_state_v2(board_state):
    # Definisco una mappatura dei valori
    mapping = {
        "EMPTY": 0,   # Cella vuota
        "BLACK": 1,   # Pezzo nero
        "WHITE": 2,   # Pezzo bianco
        "KING": 3,    # Re
        "THRONE": 4   # Trono
    }
    
    # Converto ogni cella della board nel corrispondente valore numerico
    board_numeric = []
    for row in board_state:
        for el in row:
            row_values = [mapping[el]]
            board_numeric.extend(row_values)
    
    return np.array(board_numeric)

# Funzione per preparare l'input per la rete neurale
def prepare_input(board, turn):
    # Converti la board in formato numerico
    board_numeric = convert_board_state_v2(board)
    
    # Mappa il turno in un valore numerico (0 per white, 1 per black)
    turn_numeric = 0 if turn == "white" else 1
    
    # Concateno la board numerica con il turno
    input_data = np.concatenate([board_numeric, [turn_numeric]])  # Risultato finale (81 + 1 = 82 elementi)
    
    # Reshapo per avere un input compatibile con la rete neurale (1, 82)
    return input_data.reshape(1, -1)

### APPLY MOVE ###
def apply_move(board, move):
    '''
    Apply the move to the board
    Args:
        board: current board state
        move: move to apply
    Returns:
        new_board: new board state after applying the move
    '''
    try:
        from_pos, to_pos = move
        from_row, from_col = from_pos
        to_row, to_col = to_pos

        new_board = [row.copy() for row in board]
        new_board[to_row][to_col] = new_board[from_row][from_col]
        new_board[from_row][from_col] = 'EMPTY'

        # Check if a piece is captured
        directions = [(1, 0), (-1, 0), (0, 1), (0, -1)]
        for dr, dc in directions:
            if is_within_bounds(to_row + dr*2, to_col + dc*2, new_board) and new_board[to_row + dr][to_col + dc] in enemies[new_board[to_row][to_col]] and (new_board[to_row + dr*2][to_col + dc*2] in allies[new_board[to_row][to_col]] or ((to_row + dr*2, to_col + dc*2) in citadels and (to_row + dr, to_col + dc) not in citadels)):
                if new_board[to_row + dr][to_col + dc] != 'KING':
                    new_board[to_row + dr][to_col + dc] = empty[to_row + dr][to_col + dc]

        return new_board
    except Exception as e:
        logger.error(f"Error in apply_move: {e}")
        raise


def get_opposite_turn(turn):
    '''
    Return the opposite turn
    '''
    return 'white' if turn == 'black' else 'black'




### CHECK GAME OVER ###
def is_game_over(board):
    try:
        # 1. Check if the king is in an escape cell (white wins)
        king_position = stats.get_king_position(board)
        if king_position and king_position in winning_positions:
            return 'white'  # White player wins
        # 2. Check if the king is captured (black wins)
        if is_king_captured(board):
            return 'black'  # Black player wins
        # 3. Check if a player has no possible moves (the other player wins)
        if not generate_all_possible_moves(board, 'white'):
            return 'black'  # Black player wins
        if not generate_all_possible_moves(board, 'black'):
            return 'white'  # White player wins
    except Exception as e:
        logger.error(f"Error in is_game_over: {e}")
        raise

"""     # 4. Check for a repeating board state (draw)
    if board in previous_states:
        return 'draw'  # Game is a draw due to repetition
    previous_states.append(board)  # Add current board to previous states """


def is_castle(row, col):
    """Verifica se la posizione è il castello (trono)."""
    castle_row, castle_col = 4, 4  # Il castello si trova al centro della scacchiera
    return row == castle_row and col == castle_col

def is_adjacent_to_castle(row, col):
    """Verifica se la posizione è adiacente al castello."""
    castle_row, castle_col = 4, 4  # Il castello si trova nella posizione (4, 4)
    adjacent_positions = [
        (castle_row + 1, castle_col),  # Sotto
        (castle_row - 1, castle_col),  # Sopra
        (castle_row, castle_col + 1),  # Destra
        (castle_row, castle_col - 1)   # Sinistra
    ]
    return (row, col) in adjacent_positions

def is_king_captured(board):
    """
    Check if the king is captured in the current board state.
    
    Args:
        board: Current board state.
    
    Returns:
        bool: True if the king is captured, False otherwise.
    """
    king_pos = stats.get_king_position(board)
    if not king_pos:
        return False  # No king found, so it's not captured

    king_row, king_col = king_pos
    directions = [(1, 0), (-1, 0), (0, 1), (0, -1)]  # Down, Up, Right, Left

    # Case 1: Check if the king is in the castle
    if is_castle(king_row, king_col):
        # King must be surrounded on all four sides by black pieces to be captured
        return all(is_within_bounds(king_row + dr, king_col + dc, board) and
                   board[king_row + dr][king_col + dc] == 'BLACK'
                   for dr, dc in directions)

    # Case 2: Check if the king is adjacent to the castle
    elif is_adjacent_to_castle(king_row, king_col):
        # King is captured if surrounded on three sides by black pieces
        black_count = sum(1 for dr, dc in directions
                          if is_within_bounds(king_row + dr, king_col + dc, board) and
                          board[king_row + dr][king_col + dc] == 'BLACK')
        return black_count >= 3

    # Case 3: General capture condition for non-castle, non-adjacent cells
    else:
        # King must be surrounded on all four sides by black pieces or blocking cells
        return all(is_within_bounds(king_row + dr, king_col + dc, board) and
                   (board[king_row + dr][king_col + dc] == 'BLACK' or
                    is_blocking_cell(king_row + dr, king_col + dc, board))
                   for dr, dc in directions)

def is_blocking_cell(row, col, board):
    """
    Check if a cell is a blocking cell (like a castle or citadel).
    
    Args:
        row, col: Coordinates of the cell.
        board: Current board state.
    
    Returns:
        bool: True if the cell is a blocking cell, False otherwise.
    """
    return board[row][col] in ['CASTLE', 'CITADEL']

def  is_within_bounds(row, col, board):
    '''
    Check if the given position is within the bounds of the board
    '''
    return 0 <= row < len(board) and 0 <= col < len(board[0])




### BOARD REPRESENTATION ###
def format_board(board):
    """Restituisce una rappresentazione formattata della scacchiera 9x9 come stringa."""
    # Dizionario per sostituire i valori con le iniziali
    symbols = {
        'WHITE': 'W',
        'BLACK': 'B',
        'KING': 'K',
        'THRONE': 'T',
        'EMPTY': ' '
    }

    # Intestazione delle colonne
    column_labels = "A B C D E F G H I"
    board_str = "    " + "   ".join(column_labels.split()) + "\n"
    board_str += "  +" + "---+" * 9 + "\n"

    # Aggiungi le righe della scacchiera
    for i, row in enumerate(board):
        row_str = f"{i+1} | " + " | ".join(f"{symbols[cell]}" for cell in row) + " |"
        board_str += row_str + "\n"
        board_str += "  +" + "---+" * 9 + "\n"

    return board_str

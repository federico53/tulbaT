from logger import logger
import random  


enemies = {'BLACK': ['WHITE'], 'WHITE': ['BLACK'], 'KING': ['BLACK']}
allies = {'BLACK': ['BLACK', 'CAMP', 'THRONE'], 'WHITE': ['WHITE', 'KING', 'CAMP', 'THRONE'], 'KING': ['WHITE', 'CAMP', 'THRONE']}
empty = [['EMPTY' for _ in range(9)] for _ in range(9)]
empty[0][3] = 'CAMP'
empty[0][4] = 'CAMP'
empty[0][5] = 'CAMP'
empty[1][4] = 'CAMP'
empty[3][0] = 'CAMP'
empty[3][8] = 'CAMP'
empty[4][0] = 'CAMP'
empty[4][1] = 'CAMP'
empty[4][7] = 'CAMP'
empty[4][8] = 'CAMP'
empty[5][0] = 'CAMP'
empty[5][8] = 'CAMP'
empty[7][4] = 'CAMP'
empty[8][3] = 'CAMP'
empty[8][4] = 'CAMP'
empty[8][5] = 'CAMP'
empty[4][4] = 'THRONE'

print(empty)


### MinMax ### 

def minimax_alpha_beta(board, depth, alpha, beta, turn, player):
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
    # Base case: depth limit reached or game over
    if depth == 0 or is_game_over(board):
        return heuristic_evaluation(board, turn, player), None  # Return score and no move

    best_move = None

    is_max = (turn == player)

    if is_max:
        max_eval = float('-inf')
        for move in generate_all_possible_moves(board, player):
            # Apply move for max
            new_board = apply_move(board, move)
            eval, _ = minimax_alpha_beta(new_board, depth - 1, alpha, beta, get_opposite_turn(turn), player)
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
            eval, _ = minimax_alpha_beta(new_board, depth - 1, alpha, beta, get_opposite_turn(turn), player)
            if eval < min_eval:
                min_eval = eval
                best_move = move
            beta = min(beta, eval)
            if beta <= alpha:
                break  # Alpha cut-off
        return min_eval, best_move


def heuristic_evaluation(board, turn, player):
    '''
    Heuristic evaluation function for the current board state
    Args:
        board: current board state
        turn: current turn (black/white)
        player: player color (black/white)
    Returns:
        score: evaluation score for the current board state
    '''
    return NotImplemented


def apply_move(board, move):
    '''
    Apply the move to the board
    Args:
        board: current board state
        move: move to apply
    Returns:
        new_board: new board state after applying the move
    '''
    from_pos, to_pos = move
    from_row, from_col = from_pos
    to_row, to_col = to_pos

    new_board = [row.copy() for row in board]
    new_board[to_row][to_col] = new_board[from_row][from_col]
    new_board[from_row][from_col] = 'EMPTY'

    # Check if a piece is captured
    directions = [(1, 0), (-1, 0), (0, 1), (0, -1)]
    for dr, dc in directions:
        if is_within_bounds(to_row + dr*2, to_col + dc*2, new_board) and new_board[to_row + dr][to_col + dc] in enemies[new_board[to_row][to_col]] and new_board[to_row + dr*2][to_col + dc*2] in allies[new_board[to_row][to_col]]:
            if new_board[to_row + dr][to_col + dc] != 'KING':
                new_board[to_row + dr][to_col + dc] = empty[to_row + dr][to_col + dc]

    return new_board


def get_opposite_turn(turn):
    '''
    Return the opposite turn
    '''
    return 'white' if turn == 'black' else 'black'


def is_game_over(board):
    '''
    Determine if the current board state represents a terminal position
    '''
    return NotImplemented

def  is_within_bounds(row, col, board):
    '''
    Check if the given position is within the bounds of the board
    '''
    return 0 <= row < len(board) and 0 <= col < len(board[0])


### Move validation function ###

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

### Moves generation function ###

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

### Evaluation functions ###

def evaluate_move(move, board, color):
    try:
        from_pos, to_pos = move
        from_row, from_col = from_pos
        to_row, to_col = to_pos
        score = 0

        if color == 'black':
            score += evaluate_black_move(to_row, to_col, board)
        elif color == 'white':
            score += evaluate_white_move(from_row, from_col, to_row, to_col, board)

        return score

    except Exception as e:
        logger.error(f"Error in evaluate_move: {e}")
        raise

def evaluate_black_move(to_row, to_col, board):
    score = 0
    winning_positions = get_winning_positions()
    directions = [(1, 0), (-1, 0), (0, 1), (0, -1)]

    for dr, dc in directions:
        adj_row, adj_col = to_row + dr, to_col + dc

        if is_within_bounds(adj_row, adj_col, board):
            score += check_king_threat(adj_row, adj_col, to_row, to_col, winning_positions, directions, board)
            score += check_capture_king(adj_row, adj_col, dr, dc, board)
            score += check_surrounding_king(adj_row, adj_col, directions, board)
            score += check_adjacent_to_castle(adj_row, adj_col, directions, board)

            if board[adj_row][adj_col] == 'WHITE':
                score += 50  # Black threatening a white piece

        score += 5  # Neutral move

    return score

def evaluate_white_move(from_row, from_col, to_row, to_col, board):
    score = 0

    if board[from_row][from_col] == 'KING':
        if to_row in [0, 8] and to_col in [0, 8]:
            score += 100  # King moving to a winning position
        score += check_black_adjacent(to_row, to_col, board)

    elif board[from_row][from_col] == 'WHITE':
        score += check_black_adjacent(to_row, to_col, board)
        
        if is_protecting_king(to_row, to_col, board):
            score += 80
            
        # Bonus per mantenere una formazione difensiva
        if is_defensive_formation(to_row, to_col, board):
            score += 60
            
        # Bonus per aprire percorsi di fuga per il re
        if is_opening_escape_path(to_row, to_col, board):
            score += 70

    score += 5  # Neutral move
    return score

### Move selection function ###

def select_best_move(valid_moves, board, color):
    try:
        best_score = float('-inf')
        best_moves = []  # Lista di mosse con il punteggio massimo

        for move in valid_moves:
            score = evaluate_move(move, board, color)
            if score > best_score:
                best_score = score
                best_moves = [move]  # Resetta la lista con la nuova best move
            elif score == best_score:
                best_moves.append(move)  # Aggiungi alla lista le mosse con lo stesso punteggio

        # Seleziona randomicamente una delle mosse migliori se ce ne sono più di una
        if best_moves:
            return random.choice(best_moves)
        else:
            return None  # Se non ci sono mosse valide, restituisci None
    except Exception as e:
        logger.error(f"Error in select_best_move: {e}")
        raise

### Check functions for moves ###

def check_king_threat(adj_row, adj_col, to_row, to_col, winning_positions, directions, board):
    score = 0
    if board[adj_row][adj_col] == 'KING':
        score += 100  # Black threatening the king
        for win_row, win_col in winning_positions:
            if (adj_row, adj_col) == (win_row, win_col):
                block_row, block_col = adj_row + directions[0][0], adj_col + directions[0][1]  # Using the first direction for blocking
                if (to_row == block_row and to_col == block_col):
                    score += 200  # Black blocking the king from reaching a winning position
                else:
                    score += 100  # Black threatening the king
    return score

def check_capture_king(adj_row, adj_col, dr, dc, board):
    score = 0
    capture_row, capture_col = adj_row + dr, adj_col + dc
    if is_within_bounds(capture_row, capture_col, board):
        if board[adj_row][adj_col] == 'KING':
            if (board[capture_row][capture_col] == 'BLACK' or is_gray_tile(capture_row, capture_col)):
                score += 1000  # King captured
    return score

def check_surrounding_king(adj_row, adj_col, directions, board):
    score = 0
    if is_castle(adj_row, adj_col):
        surround_count = sum(1 for dr, dc in directions if is_within_bounds(adj_row + dr, adj_col + dc, board) and board[adj_row + dr][adj_col + dc] == 'BLACK')
        if surround_count == 4:  # King surrounded on all four sides in the castle
            score += 1000
    return score

def check_adjacent_to_castle(adj_row, adj_col, directions, board):
    score = 0
    if is_adjacent_to_castle(adj_row, adj_col):
        open_sides = sum(1 for dr, dc in directions if is_within_bounds(adj_row + dr, adj_col + dc, board) and (adj_row + dr, adj_col + dc) != (4, 4) and board[adj_row + dr][adj_col + dc] != 'BLACK')
        if open_sides == 1:  # King captured with three sides surrounded near the castle
            score += 1000
    return score

def check_black_adjacent(to_row, to_col, board):
    score = 0
    directions = [(1, 0), (-1, 0), (0, 1), (0, -1)]
    for dr, dc in directions:
        adj_row, adj_col = to_row + dr, to_col + dc
        if is_within_bounds(adj_row, adj_col, board) and board[adj_row][adj_col] == 'BLACK':
            score -= 50  # King or white piece moving near a black piece
    return score

### Utility functions ###

def is_within_bounds(row, col, board):
    return 0 <= row < len(board) and 0 <= col < len(board[0])

def get_winning_positions():
    return [
        (0, 1), (0, 2), (0, 6), (0, 7),  # Top edge
        (8, 1), (8, 2), (8, 6), (8, 7),  # Bottom edge
        (1, 0), (2, 0), (6, 0), (7, 0),  # Left edge
        (1, 8), (2, 8), (6, 8), (7, 8)   # Right edge
    ]

def is_gray_tile(row, col):
    try:

        gray_tiles = [(0, 3), (0, 4), (0, 5), (1, 4), (8, 3), (8, 4), (8, 5), (7, 4), (3, 0), (4, 0), (5, 0), (4, 1), (3, 8), (4, 8), (5, 8), (4, 7)]
        return (row, col) in gray_tiles
    
    except Exception as e:
        logger.error(f"Error in is_gray_tile: {e}")
        raise

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

def is_protecting_king(to_row, to_col, board):
    """Verifica se un pezzo bianco sta proteggendo il re."""
    king_pos = find_king_position(board)
    if king_pos:
        king_row, king_col = king_pos
        # Verifica se il pezzo è adiacente al re
        return abs(to_row - king_row) + abs(to_col - king_col) == 1
    return False

def is_defensive_formation(to_row, to_col, board):
    """Verifica se la mossa contribuisce a una formazione difensiva."""
    white_pieces = 0
    directions = [(1,0), (-1,0), (0,1), (0,-1)]
    for dr, dc in directions:
        new_row, new_col = to_row + dr, to_col + dc
        if is_within_bounds(new_row, new_col, board):
            if board[new_row][new_col] in ['WHITE', 'KING']:
                white_pieces += 1
    return white_pieces >= 2

def is_opening_escape_path(to_row, to_col, board):
    """Verifica se la mossa apre un percorso di fuga per il re."""
    king_pos = find_king_position(board)
    if not king_pos:
        return False
        
    king_row, king_col = king_pos
    
    # Verifica se la mossa libera un percorso verso gli angoli
    corners = [(0,0), (0,8), (8,0), (8,8)]
    for corner_row, corner_col in corners:
        # Se il pezzo si sposta dalla linea diretta tra il re e l'angolo
        if (min(king_row, corner_row) <= to_row <= max(king_row, corner_row) and 
            min(king_col, corner_col) <= to_col <= max(king_col, corner_col)):
            # E la posizione precedente bloccava il percorso
            return True
    return False

def find_king_position(board):
    """Trova la posizione del re sulla scacchiera."""
    for row in range(len(board)):
        for col in range(len(board[row])):
            if board[row][col] == 'KING':
                return (row, col)
    return None
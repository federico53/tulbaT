from logger import logger
import random  

### Move validation function ###

def is_valid_move(move, board, color):
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
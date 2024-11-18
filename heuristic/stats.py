from logger import logger
# In this file are contained all the function useful to understand the state of the board and the game situation

# Function that returns the number of pieces of a player on the board
def count_pieces(board, player):
    try:
        count = 0
        for i in range(9):
            for j in range(9):
                if board[i][j] == player:
                    count += 1
        return count
    except Exception as e:
        logger.error(f"An error occurred in count_pieces: {e}")
        raise

# Function that returns the number of pieces which are still in the starting position for the white player
def count_white_starting_position(board):
    try:
        count = 0
        for i in range(2, 7):
            for j in range(2, 7):
                if board[i][j] == 'WHITE':
                    count += 1
        return count
    except Exception as e:
        logger.error(f"An error occurred in count_white_starting_position: {e}")
        raise

# Function that returns the number of pieces which are still in the starting position for the black player
def count_black_starting_position(board):
    try:
        count = 0
        for i in range(3, 6):
            if board[0][i] == 'BLACK':
                count += 1
        for i in range(3, 6):
            if board[8][i] == 'BLACK':
                count += 1
        for i in range(3, 6):
            if board[i][0] == 'BLACK':
                count += 1
        for i in range(3, 6):
            if board[i][8] == 'BLACK':
                count += 1
        if board[1][4] == 'BLACK':
            count += 1
        if board[7][4] == 'BLACK':
            count += 1
        if board[4][1] == 'BLACK':
            count += 1
        if board[4][7] == 'BLACK':
            count += 1
        return count
    except Exception as e:
        logger.error(f"An error occurred in count_black_starting_position: {e}")
        raise

# Function that return the king position
def get_king_position(board):
    try:
        for i in range(9):
            for j in range(9):
                if board[i][j] == 'KING':
                    return (i, j)
    except Exception as e:
        logger.error(f"An error occurred in get_king_position: {e}")
        raise

# Function that return the score of the position of the king related to the "King Winning Direction Heatmap"

king_winning_direction_heatmap = [[0, 0, 0, 0, 0, 0, 0, 0, 0],
                                  [0, 2, 3, 1, 0, 1, 3, 2, 0],
                                  [0, 3, 4, 2, 2, 2, 4, 3, 0],
                                  [0, 1, 2, 0, 0, 0, 2, 1, 0],
                                  [0, 0, 2, 0, 0, 0, 2, 0, 0],
                                  [0, 1, 2, 0, 0, 0, 2, 1, 0],
                                  [0, 3, 4, 2, 2, 2, 4, 3, 0],
                                  [0, 2, 3, 1, 0, 1, 3, 2, 0],
                                  [0, 0, 0, 0, 0, 0, 0, 0, 0]]

def king_winning_direction_score(board):
    try:
        king_position = get_king_position(board)
        return king_winning_direction_heatmap[king_position[0]][king_position[1]]
    except Exception as e:
        logger.error(f"An error occurred in king_winning_direction_score: {e}")
        raise

# Function that return how many side of the king are free and how many are blocked, and also which pieces are blocking the king

    # Positions of the black camps
black_camps_positions = [(0, 3), (0, 4), (0, 5), (1, 4), (3, 0), (3, 1), (3, 2), (4, 0), (4, 2), (4, 6), (4, 8), (5, 0)]

    # Position of the castle
castle_position = (4, 4)

def king_free_sides(board):
    try:
        king_position = get_king_position(board)

        free_sides = 0
        blocked_sides = 0
        black_blockers = 0
        white_blockers = 0
        castle_blockers = 0

        if king_position[0] == 0 or king_position[0] == 8 or king_position[1] == 0 or king_position[1] == 8:
            return 0, 0, 0, 0, 0

        if board[king_position[0] - 1][king_position[1]] == 'EMPTY':
            free_sides += 1
        else:
            blocked_sides += 1
            if (king_position[0] - 1, king_position[1]) in black_camps_positions or board[king_position[0] - 1][king_position[1]] == 'BLACK':
                black_blockers += 1
            elif board[king_position[0] - 1][king_position[1]] == 'WHITE':
                white_blockers += 1
            elif (king_position[0] - 1, king_position[1]) == castle_position:
                castle_blockers += 1

        if board[king_position[0] + 1][king_position[1]] == 'EMPTY':
            free_sides += 1
        else:
            blocked_sides += 1
            if (king_position[0] + 1, king_position[1]) in black_camps_positions or board[king_position[0] + 1][king_position[1]] == 'BLACK':
                black_blockers += 1
            elif board[king_position[0] + 1][king_position[1]] == 'WHITE':
                white_blockers += 1
            elif (king_position[0] + 1, king_position[1]) == castle_position:
                castle_blockers += 1

        if board[king_position[0]][king_position[1] - 1] == 'EMPTY':
            free_sides += 1
        else:
            blocked_sides += 1
            if (king_position[0], king_position[1] - 1) in black_camps_positions or board[king_position[0]][king_position[1] - 1] == 'BLACK':
                black_blockers += 1
            elif board[king_position[0]][king_position[1] - 1] == 'WHITE':
                white_blockers += 1
            elif (king_position[0], king_position[1] - 1) == castle_position:
                castle_blockers += 1

        if board[king_position[0]][king_position[1] + 1] == 'EMPTY':
            free_sides += 1
        else:
            blocked_sides += 1
            if (king_position[0], king_position[1] + 1) in black_camps_positions or board[king_position[0]][king_position[1] + 1] == 'BLACK':
                black_blockers += 1
            elif board[king_position[0]][king_position[1] + 1] == 'WHITE':
                white_blockers += 1
            elif (king_position[0], king_position[1] + 1) == castle_position:
                castle_blockers += 1

        return free_sides, blocked_sides, black_blockers, white_blockers, castle_blockers
    except Exception as e:
        logger.error(f"An error occurred in king_free_sides: {e}")
        raise


# Function that return if the black is in checkmate (the king can move to a winning position)
    # The king can move in any direction until it reaches a position non empty or the border of the board or a black camp or the castle

    # Winning positions for the king
winning_positions = [(0, 1), (0, 2), (0, 6), (0, 7), (1, 0), (2, 0), (6, 0), (7, 0), (8, 1), (8, 2), (8, 6), (8, 7), (1, 8), (2, 8), (6, 8), (7, 8)]

def black_checkmate(board):
    try:
        king_position = get_king_position(board)
        if king_position[0] == 0 or king_position[0] == 8 or king_position[1] == 0 or king_position[1] == 8:
            return False
        move_directions = [(0, 1), (0, -1), (1, 0), (-1, 0)]
        for direction in move_directions:
            new_position = (king_position[0] + direction[0], king_position[1] + direction[1])
            while new_position[0] >= 0 and new_position[0] <= 8 and new_position[1] >= 0 and new_position[1] <= 8:
                if new_position in winning_positions:
                    return True
                if board[new_position[0]][new_position[1]] != 'EMPTY' or new_position in black_camps_positions or new_position == castle_position:
                    break
                new_position = (new_position[0] + direction[0], new_position[1] + direction[1])
        return False
    except Exception as e:
        logger.error(f"An error occurred in black_checkmate: {e}")
        raise

# Function that given a certain position of the board and a color check if is empty 
# and then check if any piece of the color can reach that position with one move
# A move can be done in every direction until:
    # a non empty position is reached
    # the border of the board is reached
    # a black camp is reached
    # the castle is reached
    # Special rule: if a black piece is in a black camp it can move to another black camp if it pass trought cells that are always black camps

def is_empty_and_reachable(board, position, color):
    try:
        if board[position[0]][position[1]] != 'EMPTY':
            return False
        move_directions = [(0, 1), (0, -1), (1, 0), (-1, 0)]

        # Check for the special rule
        if position in black_camps_positions:
            for direction in move_directions:
                new_position = (position[0] + direction[0], position[1] + direction[1])
                while new_position[0] >= 0 and new_position[0] <= 8 and new_position[1] >= 0 and new_position[1] <= 8:
                    if new_position in black_camps_positions:
                        if board[new_position[0]][new_position[1]] == color:
                            return True
                        new_position = (new_position[0] + direction[0], new_position[1] + direction[1])
                    else:
                        break
                    if board[new_position[0]][new_position[1]] != 'EMPTY':
                        break

        for direction in move_directions:
            new_position = (position[0] + direction[0], position[1] + direction[1])
            while new_position[0] >= 0 and new_position[0] <= 8 and new_position[1] >= 0 and new_position[1] <= 8:
                if board[new_position[0]][new_position[1]] == color: # if the position is a piece of the color
                    return True
                if board[new_position[0]][new_position[1]] != 'EMPTY': # if the position is not empty
                    break
                if new_position in black_camps_positions or new_position == castle_position: # if the position is a black camp or the castle
                    break
                new_position = (new_position[0] + direction[0], new_position[1] + direction[1])
        return False
    except Exception as e:
        logger.error(f"An error occurred in is_empty_and_reachable: {e}")
        raise

# Function that return if the king is in the castle

def king_in_the_castle(king_position):
    try:
        return king_position == castle_position
    except Exception as e:
        logger.error(f"An error occurred in king_in_the_castle: {e}")
        raise

# Function that return if the king is adjacent to the castle

def king_adjacent_to_castle(king_position):
    try:
        return king_position == (3, 4) or king_position == (5, 4) or king_position == (4, 3) or king_position == (4, 5)
    except Exception as e:
        logger.error(f"An error occurred in king_adjacent_to_castle: {e}")
        raise

# Function that return if the king can be captured by two black pieces or by a black piece and a black camp
    # The king can be capture if the king has on one side a black piece or a black camp and on the opposite side
    # another black piece can with one move capture the king

def king_can_be_captured_between_two_blacks(board):
    try:
        king_position = get_king_position(board)
        if king_position[0] == 0 or king_position[0] == 8 or king_position[1] == 0 or king_position[1] == 8:
            return False
        if king_in_the_castle(king_position) or king_adjacent_to_castle(king_position):
            return False
        if (board[king_position[0] - 1][king_position[1]] == 'BLACK' or board[king_position[0] - 1][king_position[1]] in black_camps_positions) and is_empty_and_reachable(board, (king_position[0] + 1, king_position[1]), 'BLACK'):
            return True
        if (board[king_position[0] + 1][king_position[1]] == 'BLACK' or board[king_position[0] + 1][king_position[1]] in black_camps_positions) and is_empty_and_reachable(board, (king_position[0] - 1, king_position[1]), 'BLACK'):
            return True
        if (board[king_position[0]][king_position[1] - 1] == 'BLACK' or board[king_position[0]][king_position[1] - 1] in black_camps_positions) and is_empty_and_reachable(board, (king_position[0], king_position[1] + 1), 'BLACK'):
            return True
        if (board[king_position[0]][king_position[1] + 1] == 'BLACK' or board[king_position[0]][king_position[1] + 1] in black_camps_positions) and is_empty_and_reachable(board, (king_position[0], king_position[1] - 1), 'BLACK'):
            return True
        return False
    except Exception as e:
        logger.error(f"An error occurred in king_can_be_captured_between_two_blacks: {e}")
        raise

# Function that return if the king is adjacent to the castle and can be captured
# The king can be captured if:
    # the king is adjacent to the castle
    # on two of the other three sides there are black pieces
    # for the remaining side there is a black piece that with one move can capture the king

def king_adjacent_to_castle_can_be_captured(board):
    try:
        king_position = get_king_position(board)
        if king_position[0] == 0 or king_position[0] == 8 or king_position[1] == 0 or king_position[1] == 8:
            return False
        if not king_adjacent_to_castle(king_position):
            return False
        if king_position == (3, 4):
            if (board[3][3] == 'BLACK' or board[3][3] in black_camps_positions) and is_empty_and_reachable(board, (3, 5), 'BLACK') and is_empty_and_reachable(board, (4, 4), 'BLACK'):
                return True
            if (board[4][4] == 'BLACK' or board[4][4] in black_camps_positions) and is_empty_and_reachable(board, (3, 3), 'BLACK') and is_empty_and_reachable(board, (3, 5), 'BLACK'):
                return True
            if (board[3][5] == 'BLACK' or board[3][5] in black_camps_positions) and is_empty_and_reachable(board, (3, 3), 'BLACK') and is_empty_and_reachable(board, (4, 4), 'BLACK'):
                return True
        if king_position == (5, 4):
            if (board[5][3] == 'BLACK' or board[5][3] in black_camps_positions) and is_empty_and_reachable(board, (5, 5), 'BLACK') and is_empty_and_reachable(board, (4, 4), 'BLACK'):
                return True
            if (board[4][4] == 'BLACK' or board[4][4] in black_camps_positions) and is_empty_and_reachable(board, (5, 3), 'BLACK') and is_empty_and_reachable(board, (5, 5), 'BLACK'):
                return True
            if (board[5][5] == 'BLACK' or board[5][5] in black_camps_positions) and is_empty_and_reachable(board, (5, 3), 'BLACK') and is_empty_and_reachable(board, (4, 4), 'BLACK'):
                return True
        if king_position == (4, 3):
            if (board[3][3] == 'BLACK' or board[3][3] in black_camps_positions) and is_empty_and_reachable(board, (5, 3), 'BLACK') and is_empty_and_reachable(board, (4, 4), 'BLACK'):
                return True
            if (board[4][4] == 'BLACK' or board[4][4] in black_camps_positions) and is_empty_and_reachable(board, (3, 3), 'BLACK') and is_empty_and_reachable(board, (5, 3), 'BLACK'):
                return True
            if (board[5][3] == 'BLACK' or board[5][3] in black_camps_positions) and is_empty_and_reachable(board, (3, 3), 'BLACK') and is_empty_and_reachable(board, (4, 4), 'BLACK'):
                return True
        if king_position == (4, 5):
            if (board[3][5] == 'BLACK' or board[3][5] in black_camps_positions) and is_empty_and_reachable(board, (5, 5), 'BLACK') and is_empty_and_reachable(board, (4, 4), 'BLACK'):
                return True
            if (board[4][4] == 'BLACK' or board[4][4] in black_camps_positions) and is_empty_and_reachable(board, (3, 5), 'BLACK') and is_empty_and_reachable(board, (5, 5), 'BLACK'):
                return True
            if (board[5][5] == 'BLACK' or board[5][5] in black_camps_positions) and is_empty_and_reachable(board, (3, 5), 'BLACK') and is_empty_and_reachable(board, (4, 4), 'BLACK'):
                return True
        return False
    except Exception as e:
        logger.error(f"An error occurred in king_adjacent_to_castle_can_be_captured: {e}")
        raise

# Function that returns if the king is in the castle and can be captured
# The king can be captured if:
    # the king is in the castle
    # on three sides there are black pieces
    # for the remaining side there is a black piece that with one move can capture the king

def king_in_the_castle_can_be_captured(board):
    try:
        king_position = get_king_position(board)
        if king_position[0] == 0 or king_position[0] == 8 or king_position[1] == 0 or king_position[1] == 8:
            return False
        if not king_in_the_castle(king_position):
            return False
        if board[3][4] == 'BLACK' and board[4][3] == 'BLACK' and board[4][5] == 'BLACK' and is_empty_and_reachable(board, (5, 4), 'BLACK'):
            return True
        if board[5][4] == 'BLACK' and board[4][3] == 'BLACK' and board[4][5] == 'BLACK' and is_empty_and_reachable(board, (3, 4), 'BLACK'):
            return True
        if board[4][3] == 'BLACK' and board[3][4] == 'BLACK' and board[5][4] == 'BLACK' and is_empty_and_reachable(board, (4, 5), 'BLACK'):
            return True
        if board[4][5] == 'BLACK' and board[3][4] == 'BLACK' and board[5][4] == 'BLACK' and is_empty_and_reachable(board, (4, 3), 'BLACK'):
            return True
        return False
    except Exception as e:
        logger.error(f"An error occurred in king_in_the_castle_can_be_captured: {e}")
        raise

# Function that return if the white is in checkmate
# The white is in checkmate when the king can be captured by the black pieces (so when with one move the black wins the game)
# The king is captured when
    # The king is between two black pieces or a black camp and a black piece (and is not in the castle and not adjacent to the castle)
    # The king is in the castle and on each side there is a black piece
    # The king is adjacent to the castle and on the other three sides there are black pieces

def white_checkmate(board):
    try:
        return king_can_be_captured_between_two_blacks(board) or king_adjacent_to_castle_can_be_captured(board) or king_in_the_castle_can_be_captured(board)
    except Exception as e:
        logger.error(f"An error occurred in white_checkmate: {e}")
        raise


# function that returns if the white is not yet in checkmate, but the blacks could kill the king in a couple of turns
#the situations are the same of white_checkmate
    #king is not in the castle nor adjacent to it, but king can be approached on one side and you have another black pawn which can go to the other side
    #king is in the castle and there are already two black pawns adjacent to him, and two more that could reach him
    #king is adjacent to the castle and there is one black pawn and two more that can reach him
#in these two last cases, it's necessary that those two black pawns that can get to the king aren't the same.
#maybe change is_empty_and_reachable so that it returns also the starting position, and check whether the positions of the two is different.

def black_can_checkmate_in_future(board):
    try:
        return king_can_be_captured_between_two_blacks_infuture(board) or king_adjacent_to_castle_can_be_captured_infuture(board) or king_in_the_castle_can_be_captured_infuture(board)
    except Exception as e:
        logger.error(f"An error occurred in white_checkmate: {e}")
        raise

def is_empty_and_reachable_version2(board, position, color, exceptional_start=None):
    """
    Check if a position is empty and reachable by a piece of the specified color, excluding an exceptional start position.
    Args:
        board: Current board state.
        position: Target position to check.
        color: Color of the piece to check for reachability.
        exceptional_start: Position to exclude from the check (default is None).
    Returns:
        tuple: (bool, tuple) - True if the position is reachable, and the starting position of the piece that can reach it.
               False if the position is not reachable, and None.
    """
    try:
        if board[position[0]][position[1]] != 'EMPTY':
            return False, None

        move_directions = [(0, 1), (0, -1), (1, 0), (-1, 0)]

        # Check for the special rule
        if position in black_camps_positions:
            for direction in move_directions:
                new_position = (position[0] + direction[0], position[1] + direction[1])
                while new_position[0] >= 0 and new_position[0] <= 8 and new_position[1] >= 0 and new_position[1] <= 8:
                    if new_position in black_camps_positions:
                        if board[new_position[0]][new_position[1]] == color and new_position != exceptional_start:
                            return True, new_position
                        new_position = (new_position[0] + direction[0], new_position[1] + direction[1])
                    else:
                        break
                    if board[new_position[0]][new_position[1]] != 'EMPTY':
                        break

        for direction in move_directions:
            new_position = (position[0] + direction[0], position[1] + direction[1])
            while new_position[0] >= 0 and new_position[0] <= 8 and new_position[1] >= 0 and new_position[1] <= 8:
                if board[new_position[0]][new_position[1]] == color and new_position != exceptional_start:  # if the position is a piece of the color and not the exceptional start
                    return True, new_position
                if board[new_position[0]][new_position[1]] != 'EMPTY':  # if the position is not empty
                    break
                if new_position in black_camps_positions or new_position == castle_position:  # if the position is a black camp or the castle
                    break
                new_position = (new_position[0] + direction[0], new_position[1] + direction[1])
        return False, None
    except Exception as e:
        logger.error(f"An error occurred in is_empty_and_reachable_version2: {e}")
        raise

def king_can_be_captured_between_two_blacks_infuture(board):
    try:
        king_position = get_king_position(board)

        if king_position[0] == 0 or king_position[0] == 8 or king_position[1] == 0 or king_position[1] == 8:
            return False

        if king_in_the_castle(king_position) or king_adjacent_to_castle(king_position):
            return False
        
        reachable, starting_position = is_empty_and_reachable_version2(board, (king_position[0] - 1, king_position[1]),'BLACK')
        if reachable and is_empty_and_reachable_version2(board, (king_position[0] + 1, king_position[1]), 'BLACK', starting_position)[0]:
            return True
        
        reachable, starting_position = is_empty_and_reachable_version2(board, (king_position[0] + 1, king_position[1]),'BLACK')
        if reachable and is_empty_and_reachable_version2(board, (king_position[0] - 1, king_position[1]), 'BLACK', starting_position)[0]:
            return True
        
        reachable, starting_position = is_empty_and_reachable_version2(board, (king_position[0], king_position[1]-1),'BLACK')
        if reachable and is_empty_and_reachable_version2(board, (king_position[0], king_position[1]+1), 'BLACK', starting_position)[0]:
            return True
        
        reachable, starting_position = is_empty_and_reachable_version2(board, (king_position[0], king_position[1]+1),'BLACK')
        if reachable and is_empty_and_reachable_version2(board, (king_position[0], king_position[1]-1), 'BLACK', starting_position)[0]:
            return True
        
        return False
    except Exception as e:
        logger.error(f"An error occurred in king_can_be_captured_between_two_blacks_infuture: {e}")
        raise

def king_in_the_castle_can_be_captured_infuture(board):
    """
    Check if the king in the castle can be captured by two black pieces on two sides and two more that can fill the remaining gaps.
    Args:
        board: Current board state.
    Returns:
        bool: True if the king can be captured, False otherwise.
    """
    try:
        king_position = get_king_position(board)
        if not king_in_the_castle(king_position):
            return False


        # Check if there are two black pieces on two sides and two more that can fill the remaining gaps
        if board[3][4] == 'BLACK' and board[4][3] == 'BLACK':
            reachable1, start_pos1 = is_empty_and_reachable_version2(board, (4, 5), 'BLACK')
            reachable2, _ = is_empty_and_reachable_version2(board, (5, 4), 'BLACK', exceptional_start=start_pos1)
            if reachable1 and reachable2:
                return True

        if board[3][4] == 'BLACK' and board[4][5] == 'BLACK':
            reachable1, start_pos1 = is_empty_and_reachable_version2(board, (4, 3), 'BLACK')
            reachable2, _ = is_empty_and_reachable_version2(board, (5, 4), 'BLACK', exceptional_start=start_pos1)
            if reachable1 and reachable2:
                return True

        if board[4][3] == 'BLACK' and board[5][4] == 'BLACK':
            reachable1, start_pos1 = is_empty_and_reachable_version2(board, (4, 5), 'BLACK')
            reachable2, _ = is_empty_and_reachable_version2(board, (3, 4), 'BLACK', exceptional_start=start_pos1)
            if reachable1 and reachable2:
                return True

        if board[4][5] == 'BLACK' and board[5][4] == 'BLACK':
            reachable1, start_pos1 = is_empty_and_reachable_version2(board, (4, 3), 'BLACK')
            reachable2, _ = is_empty_and_reachable_version2(board, (3, 4), 'BLACK', exceptional_start=start_pos1)
            if reachable1 and reachable2:
                return True

        if board[3][4] == 'BLACK' and board[5][4] == 'BLACK':
            reachable1, start_pos1 = is_empty_and_reachable_version2(board, (4, 3), 'BLACK')
            reachable2, _ = is_empty_and_reachable_version2(board, (4, 5), 'BLACK', exceptional_start=start_pos1)
            if reachable1 and reachable2:
                return True

        if board[4][3] == 'BLACK' and board[4][5] == 'BLACK':
            reachable1, start_pos1 = is_empty_and_reachable_version2(board, (3, 4), 'BLACK')
            reachable2, _ = is_empty_and_reachable_version2(board, (5, 4), 'BLACK', exceptional_start=start_pos1)
            if reachable1 and reachable2:
                return True

        return False
    except Exception as e:
        logger.error(f"An error occurred in king_in_the_castle_can_be_captured_infuture: {e}")
        raise

def king_adjacent_to_castle_can_be_captured_infuture(board):
    """
    Check if the king adjacent to the castle can be captured by one black piece and two more that can fill the remaining gaps.
    Args:
        board: Current board state.
    Returns:
        bool: True if the king can be captured, False otherwise.
    """
    try:
        king_position = get_king_position(board)
        if not king_adjacent_to_castle(king_position):
            return False

        # Check if there is one black piece and two more that can fill the remaining gaps
        if king_position == (3, 4):
            if (board[3][3] == 'BLACK' or board[3][3] in black_camps_positions):
                reachable1, start_pos1 = is_empty_and_reachable_version2(board, (3, 5), 'BLACK')
                reachable2, _ = is_empty_and_reachable_version2(board, (4, 4), 'BLACK', exceptional_start=start_pos1)
                if reachable1 and reachable2:
                    return True
            if (board[3][5] == 'BLACK' or board[3][5] in black_camps_positions):
                reachable1, start_pos1 = is_empty_and_reachable_version2(board, (3, 3), 'BLACK')
                reachable2, _ = is_empty_and_reachable_version2(board, (4, 4), 'BLACK', exceptional_start=start_pos1)
                if reachable1 and reachable2:
                    return True
            if (board[4][4] == 'BLACK' or board[4][4] in black_camps_positions):
                reachable1, start_pos1 = is_empty_and_reachable_version2(board, (3, 3), 'BLACK')
                reachable2, _ = is_empty_and_reachable_version2(board, (3, 5), 'BLACK', exceptional_start=start_pos1)
                if reachable1 and reachable2:
                    return True

        if king_position == (5, 4):
            if (board[5][3] == 'BLACK' or board[5][3] in black_camps_positions):
                reachable1, start_pos1 = is_empty_and_reachable_version2(board, (5, 5), 'BLACK')
                reachable2, _ = is_empty_and_reachable_version2(board, (4, 4), 'BLACK', exceptional_start=start_pos1)
                if reachable1 and reachable2:
                    return True
            if (board[5][5] == 'BLACK' or board[5][5] in black_camps_positions):
                reachable1, start_pos1 = is_empty_and_reachable_version2(board, (5, 3), 'BLACK')
                reachable2, _ = is_empty_and_reachable_version2(board, (4, 4), 'BLACK', exceptional_start=start_pos1)
                if reachable1 and reachable2:
                    return True
            if (board[4][4] == 'BLACK' or board[4][4] in black_camps_positions):
                reachable1, start_pos1 = is_empty_and_reachable_version2(board, (5, 3), 'BLACK')
                reachable2, _ = is_empty_and_reachable_version2(board, (5, 5), 'BLACK', exceptional_start=start_pos1)
                if reachable1 and reachable2:
                    return True

        if king_position == (4, 3):
            if (board[3][3] == 'BLACK' or board[3][3] in black_camps_positions):
                reachable1, start_pos1 = is_empty_and_reachable_version2(board, (5, 3), 'BLACK')
                reachable2, _ = is_empty_and_reachable_version2(board, (4, 4), 'BLACK', exceptional_start=start_pos1)
                if reachable1 and reachable2:
                    return True
            if (board[5][3] == 'BLACK' or board[5][3] in black_camps_positions):
                reachable1, start_pos1 = is_empty_and_reachable_version2(board, (3, 3), 'BLACK')
                reachable2, _ = is_empty_and_reachable_version2(board, (4, 4), 'BLACK', exceptional_start=start_pos1)
                if reachable1 and reachable2:
                    return True
            if (board[4][4] == 'BLACK' or board[4][4] in black_camps_positions):
                reachable1, start_pos1 = is_empty_and_reachable_version2(board, (3, 3), 'BLACK')
                reachable2, _ = is_empty_and_reachable_version2(board, (5, 3), 'BLACK', exceptional_start=start_pos1)
                if reachable1 and reachable2:
                    return True

        if king_position == (4, 5):
            if (board[3][5] == 'BLACK' or board[3][5] in black_camps_positions):
                reachable1, start_pos1 = is_empty_and_reachable_version2(board, (5, 5), 'BLACK')
                reachable2, _ = is_empty_and_reachable_version2(board, (4, 4), 'BLACK', exceptional_start=start_pos1)
                if reachable1 and reachable2:
                    return True
            if (board[5][5] == 'BLACK' or board[5][5] in black_camps_positions):
                reachable1, start_pos1 = is_empty_and_reachable_version2(board, (3, 5), 'BLACK')
                reachable2, _ = is_empty_and_reachable_version2(board, (4, 4), 'BLACK', exceptional_start=start_pos1)
                if reachable1 and reachable2:
                    return True
            if (board[4][4] == 'BLACK' or board[4][4] in black_camps_positions):
                reachable1, start_pos1 = is_empty_and_reachable_version2(board, (3, 5), 'BLACK')
                reachable2, _ = is_empty_and_reachable_version2(board, (5, 5), 'BLACK', exceptional_start=start_pos1)
                if reachable1 and reachable2:
                    return True

        return False
    except Exception as e:
        logger.error(f"An error occurred in king_adjacent_to_castle_can_be_captured: {e}")
        raise

def king_can_checkmate_in_future(board):
    """
    Check if the king can win in two moves.
    Args:
        board: Current board state.
    Returns:
        bool: True if the king can win in two moves, False otherwise.
    """
    try:
        king_position = get_king_position(board)
        move_directions = [(0, 1), (0, -1), (1, 0), (-1, 0)]

        # Check if the king can win in two moves
        for direction in move_directions:
            first_move = (king_position[0] + direction[0], king_position[1] + direction[1])
            while first_move[0] >= 0 and first_move[0] <= 8 and first_move[1] >= 0 and first_move[1] <= 8:
                if board[first_move[0]][first_move[1]] == 'EMPTY':
                    for second_direction in move_directions:
                        second_move = (first_move[0] + second_direction[0], first_move[1] + second_direction[1])
                        while second_move[0] >= 0 and second_move[0] <= 8 and second_move[1] >= 0 and second_move[1] <= 8:
                            if second_move in winning_positions:
                                return True
                            if board[second_move[0]][second_move[1]] != 'EMPTY' or second_move in black_camps_positions or second_move == castle_position:
                                break
                            second_move = (second_move[0] + second_direction[0], second_move[1] + second_direction[1])
                if board[first_move[0]][first_move[1]] != 'EMPTY' or first_move in black_camps_positions or first_move == castle_position:
                    break
                first_move = (first_move[0] + direction[0], first_move[1] + direction[1])

        return False
    except Exception as e:
        logger.error(f"An error occurred in king_can_checkmate_in_future: {e}")
        raise

def king_block_in_quadrant(board):
    '''
    For a given board return True if the king is blocked in a quadrant
    '''
    try:

        row, col = get_king_position(board)

        # If the king is on a corridor then is not blocked
        if row == 4 or col == 4:
            return False

        # Based on the quadrant i get the areas to check
        orizontal = []
        vertical = []
        blocked_orizontal = False
        blocked_vertical = False

        if row < 4:
            orizontal = [(2, 3), (2, 4), (2, 5), (3, 3), (3, 4), (3, 5)]
        else:
            orizontal = [(5, 3), (5, 4), (5, 5), (6, 3), (6, 4), (6, 5)]

        if col < 4:
            vertical = [(3, 2), (3, 3), (4, 2), (4, 3), (5, 2), (5, 3)]
        else:
            vertical = [(3, 5), (3, 6), (4, 5), (4, 6), (5, 5), (5, 6)]

        # If the king is in one of the area then i delete that position from the list
        if (row, col) in orizontal:
            orizontal.remove((row, col))
        
        if (row, col) in vertical:
            vertical.remove((row, col))

        # For the two areas i check if are they blocked
        for t1 in orizontal:
            for t2 in orizontal:
                if board[t1[0]][t1[1]] == 'BLACK' and board[t2[0]][t2[1]] == 'BLACK' and t1[0] != t2[0] and abs(t1[1] - t2[1]) < 2:
                    blocked_orizontal = True

        for t1 in vertical:
            for t2 in vertical:
                if board[t1[0]][t1[1]] == 'BLACK' and board[t2[0]][t2[1]] == 'BLACK' and t1[1] != t2[1] and abs(t1[0] - t2[0]) < 2:
                    blocked_vertical = True

        return blocked_orizontal and blocked_vertical

    
    except Exception as e:
        logger.error(f"An error occurred in king_block_in_quadrant: {e}")
        raise


# Function stats that returns an explanation of the situation of the board 
# This function uses all the function above in this file

def stats_of_the_board(board, turn):
    result = []

    result.append("\n----------NUMBER OF PIECES-----------\n")
    result.append(f"White pieces: {count_pieces(board, 'WHITE')}/8 + the KING")
    result.append(f"Black pieces: {count_pieces(board, 'BLACK')}/16")
    result.append("\n-------------------------------------\n")

    result.append("-----PIECES ON STARTING POSITION-----\n")
    result.append(f"White pieces on starting position: {count_white_starting_position(board)}/8")
    result.append(f"Black pieces on starting position: {count_black_starting_position(board)}/16")
    result.append("\n-------------------------------------\n")

    result.append("-----------KING SITUATION------------\n")
    result.append(f"The KING is in a position with a direction winning score of: {king_winning_direction_score(board)}")
    result.append(f"The KING is in the castle: {king_in_the_castle(get_king_position(board))}")
    result.append(f"The KING is adjacent to the castle: {king_adjacent_to_castle(get_king_position(board))}")
    free_sides, blocked_sides, black_blockers, white_blockers, castle_blockers = king_free_sides(board)
    result.append(f"The KING has free sides: {free_sides}/4")
    result.append(f"The KING has blocked sides: {blocked_sides}/4")
    result.append(f"The KING has black blockers: {black_blockers}/{blocked_sides}")
    result.append(f"The KING has white blockers: {white_blockers}/{blocked_sides}")
    result.append(f"The KING has castle blockers: {castle_blockers}/{blocked_sides}")
    result.append(f"The KING is blocked in a quadrant: {king_block_in_quadrant(board)}")
    result.append("\n-------------------------------------\n")

    if turn == "black":
        result.append("---------BLACK CHECKMATE-------------\n")
        result.append(f"The BLACK is in checkmate: {black_checkmate(board)}")
        result.append("\n-------------------------------------\n")
    
    if turn == "white":
        result.append("---------WHITE CHECKMATE-------------\n")
        result.append(f"The WHITE is in checkmate: {white_checkmate(board)}")
        result.append("\n-------------------------------------\n")

    return "\n".join(result)

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
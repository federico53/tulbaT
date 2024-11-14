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
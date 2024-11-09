import socket
import struct
import json
import random
from heuristic.stats import stats_of_the_board

def recvall(sock, n):
    data = b''
    while len(data) < n:
        packet = sock.recv(n - len(data))
        if not packet:
            return None
        data += packet
    return data

def convert_move_for_server(move, color):
    def coordinate_to_algebraic(row, col):
        columns = 'abcdefghi'  # Colonne dalla 'a' alla 'i'
        return columns[col] + str(row + 1)  # Le righe vanno da 1 a 9

    from_pos = coordinate_to_algebraic(move[0][0], move[0][1])
    to_pos = coordinate_to_algebraic(move[1][0], move[1][1])
    return json.dumps({"from": from_pos, "to": to_pos, "turn": color.upper()})

def is_gray_tile(row, col, board):
    gray_tiles = [(0, 3), (0, 4), (0, 5), (1, 4), (8, 3), (8, 4), (8, 5), (7, 4), (3, 0), (4, 0), (5, 0), 
                  (4, 1), (3, 8), (4, 8), (5, 8), (4, 7)]
    return (row, col) in gray_tiles

# Lista delle 16 caselle di vittoria del re
winning_positions = [
    (0, 1), (0, 2), (0, 6), (0, 7),  # Bordo superiore
    (8, 1), (8, 2), (8, 6), (8, 7),  # Bordo inferiore
    (1, 0), (2, 0), (6, 0), (7, 0),  # Bordo sinistro
    (1, 8), (2, 8), (6, 8), (7, 8)   # Bordo destro
]

def evaluate_move(move, board, color):
    """ Function to evaluate the score of a move based on the heuristic. """
    from_pos, to_pos = move
    from_row, from_col = from_pos
    to_row, to_col = to_pos
    score = 0

    if color == 'black':
        # Heuristic for black (trap the king, attack white pieces, capture white, block king)
        directions = [(1, 0), (-1, 0), (0, 1), (0, -1)]
        for dr, dc in directions:
            adj_row, adj_col = to_row + dr, to_col + dc
            if 0 <= adj_row < len(board) and 0 <= adj_col < len(board[0]):
                if board[adj_row][adj_col] == 'KING':
                    score += 100  # Black threatening the king
                    
                    # Controlla se il re è vicino a una posizione di vittoria
                    for win_row, win_col in winning_positions:
                        if (adj_row, adj_col) == (win_row, win_col):
                            # Controlla se la mossa del nero blocca il re dal raggiungere la vittoria
                            block_row, block_col = adj_row + dr, adj_col + dc
                            if (to_row == block_row and to_col == block_col):
                                score += 200  # Black blocking the king from reaching a winning position
                            else:
                                score += 100  # Black threatening the king

                elif board[adj_row][adj_col] == 'WHITE':
                    score += 50  # Black threatening a white piece

            # Cattura: verifica se un bianco è intrappolato tra due neri o tra un nero e una casella grigia
            capture_row, capture_col = adj_row + dr, adj_col + dc
            if 0 <= capture_row < len(board) and 0 <= capture_col < len(board[0]):
                if board[adj_row][adj_col] == 'WHITE' and (board[capture_row][capture_col] == 'BLACK' or is_gray_tile(capture_row, capture_col, board)):
                    score += 120  # Black capturing a white piece (between blacks or black and gray)

        score += 5  # Neutral move

    elif color == 'white':
        # Heuristic for white (protect the king, avoid adjacent black pieces)
        if board[from_row][from_col] == 'KING':
            if to_row in [0, 8] and to_col in [0, 8]:
                score += 100  # King moving to a winning position
            directions = [(1, 0), (-1, 0), (0, 1), (0, -1)]
            for dr, dc in directions:
                adj_row, adj_col = to_row + dr, to_col + dc
                if 0 <= adj_row < len(board) and 0 <= adj_col < len(board[0]):
                    if board[adj_row][adj_col] == 'BLACK':
                        score -= 50  # King moving near a black piece

        elif board[from_row][from_col] == 'WHITE':
            directions = [(1, 0), (-1, 0), (0, 1), (0, -1)]
            for dr, dc in directions:
                adj_row, adj_col = to_row + dr, to_col + dc
                if 0 <= adj_row < len(board) and 0 <= adj_col < len(board[0]):
                    if board[adj_row][adj_col] == 'BLACK':
                        score -= 50  # White piece moving near a black piece

        score += 5  # Neutral move

    return score


def select_best_move(valid_moves, board, color):
    """ Select the best move based on the heuristic evaluation. """
    best_move = None
    best_score = float('-inf')
    
    for move in valid_moves:
        score = evaluate_move(move, board, color)
        if score > best_score:
            best_move = move
            best_score = score
    
    return best_move

def is_valid_move(move, board, color):
    """ Verifica se una mossa è valida basata sulle regole del gioco di Tablut. """
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

def generate_all_possible_moves(board, color):
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


def main():
    player_name = input("Enter your player name: ")
    color = input("Choose your color (white/black): ").lower()

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        if color == 'white':
            server_address = ('localhost', 5800)
        elif color == 'black':
            server_address = ('localhost', 5801)
        else:
            raise Exception("You must play as white or black")

        print(f"Connecting to {server_address}")
        sock.connect(server_address)

        # Send player name to server
        sock.send(struct.pack('>i', len(player_name)))
        sock.send(player_name.encode())

        while True:
            # Receive game state from server
            len_bytes = struct.unpack('>i', recvall(sock, 4))[0]
            current_state_server_bytes = recvall(sock, len_bytes)
            json_current_state_server = json.loads(current_state_server_bytes.decode())

            board = json_current_state_server['board']
            turn = json_current_state_server['turn'].lower()

            #print("Current state: ", json.dumps(json_current_state_server, indent=2))

            # Check if the game is over
            if turn in ['whitewin', 'blackwin', 'draw']:
                print(f"Game over! Result: {turn.upper()}")
                break

            # Check if it's our turn
            if turn != color:
                print(f"Waiting for the opponent's turn... (Current turn: {turn})")
                continue  # Wait for our turn

            # Stampare lo stato di gioco
            stats_of_the_board(board, turn)

            # Generate all possible moves based on the updated board
            all_possible_moves = generate_all_possible_moves(board, color)

            # Select the best move using the heuristic function
            if not all_possible_moves:
                print("No valid moves available.")
                break

            best_move = select_best_move(all_possible_moves, board, color)

            # Convert move for server and send it
            move_for_server = convert_move_for_server(best_move, color)
            print(f"Sending move: {move_for_server}")
            sock.send(struct.pack('>i', len(move_for_server)))
            sock.send(move_for_server.encode())

            # Wait for the server's response or opponent's move
            print(f"Move made: {best_move}, waiting for the opponent's move...")

if __name__ == "__main__":
    main()

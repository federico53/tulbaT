import socket
import struct
import json
from logger import logger

class GameDriver:
    def __init__(self, color, server_address):
        self.color = color
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_address = server_address
        logger.info(f"GameDriver initialized with color {self.color} and server {self.server_address}")

    def connect(self, player_name):
        try:
            self.sock.connect(self.server_address)
            logger.info(f"Connected to server at {self.server_address}")
            self.sock.send(struct.pack('>i', len(player_name)))
            self.sock.send(player_name.encode())
            logger.info(f"Sent player name: {player_name}")
        except Exception as e:
            logger.error(f"Failed to connect or send player name: {e}")
            raise

    def send_move(self, move, color):
        try:
            move_for_server = self.convert_move_for_server(move, color)
            self.sock.send(struct.pack('>i', len(move_for_server)))
            self.sock.send(move_for_server.encode())
            logger.info(f"Sent move: {move_for_server}")
        except Exception as e:
            logger.error(f"Failed to send move: {e}")
            raise

    def convert_move_for_server(self, move, color):
        try:

            from_pos = self.coordinate_to_algebraic(move[0][0], move[0][1])
            to_pos = self.coordinate_to_algebraic(move[1][0], move[1][1])
            return json.dumps({"from": from_pos, "to": to_pos, "turn": color.upper()})
        
        except Exception as e:
            logger.error(f"Error in convert_move_for_server: {e}")
            raise

    def coordinate_to_algebraic(self, row, col):
        try:

            columns = 'abcdefghi'  # Colonne dalla 'a' alla 'i'
            return columns[col] + str(row + 1)  # Le righe vanno da 1 a 9
        
        except Exception as e:
            logger.error(f"Error in coordinate_to_algebraic: {e}")
            raise

    def receive_game_state(self):
        try:
            len_bytes = struct.unpack('>i', self.recvall(4))[0]
            current_state_server_bytes = self.recvall(len_bytes)
            game_state = json.loads(current_state_server_bytes.decode())
            return game_state
        except Exception as e:
            logger.error(f"Failed to receive game state: {e}")
            raise

    def recvall(self, n):
        data = b''
        while len(data) < n:
            packet = self.sock.recv(n - len(data))
            if not packet:
                logger.warning("Socket connection closed unexpectedly.")
                return None
            data += packet
        return data

    def close(self):
        self.sock.close()
        logger.info("Socket connection closed.")

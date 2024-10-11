import socket
import struct
import json
import torch
import torch.nn as nn

class TablutNN(nn.Module):
    def __init__(self):
        super(TablutNN, self).__init__()
        self.fc1 = nn.Linear(81, 128)  # 9x9 board
        self.fc2 = nn.Linear(128, 128)
        self.fc3 = nn.Linear(128, 81)  # 9x9 output

    def forward(self, x):
        x = torch.relu(self.fc1(x))
        x = torch.relu(self.fc2(x))
        x = self.fc3(x)
        return x

def get_reward(game_result):
    if game_result == "win":
        return 1
    elif game_result == "loss":
        return -1
    else:
        return 0

def check_valid_move(state, move):
    # Inserisci qui le funzioni di cattura e controllo della validità
    return True  # Placeholder: deve essere implementato con la logica delle mosse

def recvall(sock, n):
    data = b''
    while len(data) < n:
        packet = sock.recv(n - len(data))
        if not packet:
            return None
        data += packet
    return data

def convert_move_for_server(move, color):
    # Funzione di conversione della mossa per il server (da adattare)
    return json.dumps(move)

def play_game(color, player_name):
    model = TablutNN()
    optimizer = torch.optim.Adam(model.parameters(), lr=0.001)
    criterion = nn.MSELoss()

    # Connessione al server
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        if color == 'white':
            server_address = ('localhost', 5800)
        elif color == 'black':
            server_address = ('localhost', 5801)
        else:
            raise Exception("Se giochi o sei bianco oppure sei nero")
        
        sock.connect(server_address)
        sock.send(struct.pack('>i', len(player_name)))
        sock.send(player_name.encode())

        while True:
            # Ricevi lo stato corrente dal server
            len_bytes = struct.unpack('>i', recvall(sock, 4))[0]
            current_state_server_bytes = sock.recv(len_bytes)
            json_current_state_server = json.loads(current_state_server_bytes)

            # Converti lo stato JSON in una rappresentazione compatibile con il modello
            current_state = ...  # Converti json_current_state_server in array 9x9
            input_tensor = torch.tensor(current_state.flatten(), dtype=torch.float32)
            
            # Passa lo stato attraverso la rete neurale
            output = model(input_tensor)

            # Filtra le mosse valide
            valid_moves = []
            for i, move in enumerate(output):
                if check_valid_move(current_state, move):  # Usa la logica di cattura
                    valid_moves.append((i, move))
            
            if not valid_moves:
                print("Nessuna mossa valida trovata.")
                break

            # Seleziona la migliore mossa tra quelle valide
            best_move_index, _ = max(valid_moves, key=lambda x: x[1])
            selected_move = best_move_index  # Il miglior indice tra le mosse valide

            # Converti la mossa per il server e inviala
            move_for_server = convert_move_for_server(selected_move, color)
            sock.send(struct.pack('>i', len(move_for_server)))
            sock.send(move_for_server.encode())

            # Ricevi il risultato della mossa (win/loss/draw) dal server
            game_result = ...  # Deve essere ricevuto dal server (es. win/loss/draw)
            reward = get_reward(game_result)

            # Aggiorna la rete in base alla ricompensa
            target = torch.tensor([reward], dtype=torch.float32)
            loss = criterion(output[best_move_index], target)

            optimizer.zero_grad()
            loss.backward()
            optimizer.step()

            # Termina il ciclo se il gioco è finito
            if game_result in ["win", "loss", "draw"]:
                break

if __name__ == "__main__":
    color = 'white'  # O 'black', a seconda del giocatore
    player_name = 'Player1'  # Nome del giocatore
    play_game(color, player_name)

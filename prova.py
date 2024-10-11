import torch
import torch.nn as nn
import random

class TablutNN(nn.Module):
    def __init__(self):
        super(TablutNN, self).__init__()
        self.fc1 = nn.Linear(81, 128)  # Cambiato a 81 per il campo di 9x9
        self.fc2 = nn.Linear(128, 128)
        self.fc3 = nn.Linear(128, 81)  # Output anche 81 per rappresentare tutte le possibili celle di destinazione

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
    # Ad esempio:
    # state = checkCaptureWhite(state, move)
    # state = checkCaptureBlackKingLeft(state, move)
    # state = checkCaptureBlackKingUp(state, move)
    # state = checkCaptureBlackKingRight(state, move)
    # state = checkCaptureBlackKingDown(state, move)
    return state

def play_game():
    model = TablutNN()
    optimizer = torch.optim.Adam(model.parameters(), lr=0.001)
    criterion = nn.MSELoss()  # Funzione di perdita

    # Inizializza il gioco
    while True:
        current_state = ...  # Ottieni lo stato attuale dal gioco (9x9 array)
        input_tensor = torch.tensor(current_state.flatten(), dtype=torch.float32)
        
        # Passa lo stato corrente attraverso la rete neurale
        output = model(input_tensor)
        
        # Filtra le mosse per ottenere solo quelle valide
        valid_moves = []
        for i, move in enumerate(output):
            if check_valid_move(current_state, move):  # Usa la logica di cattura qui
                valid_moves.append((i, move))
        
        if not valid_moves:
            print("Nessuna mossa valida trovata.")
            break
        
        # Seleziona la mossa con il valore più alto tra quelle valide
        best_move_index, _ = max(valid_moves, key=lambda x: x[1])
        selected_move = best_move_index  # Il miglior indice tra le mosse valide
        
        # Invia la mossa al server e ottieni il risultato del gioco
        game_result = ...  # Ottieni il risultato dal server (win/loss/draw)
        reward = get_reward(game_result)
        
        # Calcola la perdita e aggiorna la rete
        target = torch.tensor([reward], dtype=torch.float32)  # Obiettivo per la ricompensa
        loss = criterion(output[best_move_index], target)  # Confronta con l'output del modello
        
        optimizer.zero_grad()
        loss.backward()  # Calcola i gradienti
        optimizer.step()  # Aggiorna i pesi della rete
        
        # Termina il ciclo se il gioco è finito
        if game_result in ["win", "loss", "draw"]:
            break

if __name__ == "__main__":
    play_game()

import torch
import torch.nn as nn
import random

class TablutNN(nn.Module):
    def __init__(self):
        super(TablutNN, self).__init__()
        self.fc1 = nn.Linear(64, 128)
        self.fc2 = nn.Linear(128, 128)
        self.fc3 = nn.Linear(128, 64)

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

def play_game():
    model = TablutNN()
    # Implementa il tuo codice per giocare a Tablut
    while True:
        current_state = ...  # Ottieni lo stato attuale
        input_tensor = torch.tensor(current_state.flatten(), dtype=torch.float32)
        
        output = model(input_tensor)
        predicted_move_index = torch.argmax(output).item()
        # Supponiamo di avere un array di mosse valide
        selected_move = valid_moves[predicted_move_index]
        
        # Invia la mossa al server e ricevi il risultato
        game_result = ...  # Ottieni il risultato dal server (win/loss/draw)
        reward = get_reward(game_result)

        # Aggiorna la rete qui in base alla ricompensa
        # ... (implementa l'aggiornamento dei pesi della rete)

if __name__ == "__main__":
    play_game()

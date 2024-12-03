import os
import re
import pandas as pd
import numpy as np

def parse_tablut_log(file_path):
    with open(file_path, 'r') as file:
        log_data = file.readlines()

    dataset = []
    current_state = None
    winner = None

    for line in log_data:
        line = line.strip()
        
        # Controlla se la linea indica il vincitore alla fine del file
        if line in ["WW", "BW"]:
            print("Vincitore:", line)
            winner = line  # Se 'WW', il bianco ha vinto; se 'BB', il nero ha vinto

        # Match stato della board
        if "Stato:" in line:
            current_state = []
        elif re.match(r'^[OBWKT]{9}$', line):  # Righe della board
            current_state.append(line)
        elif current_state and line == '':  # Linea vuota che separa i turni
            current_state = "\n".join(current_state).strip()  # Unisci board in un'unica stringa
            current_state = current_state.replace('-','')  # Rimuovi trattini
            
        elif "Turn:" in line:  # Turno e mossa
            turn_match = re.search(r'Turn: ([WB]) .* from (\w\d) to (\w\d)', line)
            if turn_match:
                turn = turn_match.group(1)
                #start_pos = turn_match.group(2)
                #end_pos = turn_match.group(3)
               
                boards_numeric = convert_board_state(current_state)
                #print(boards_numeric)

                for b in boards_numeric:
                    if len(b) != 81:
                        #Error
                        continue

                    # Aggiungi al dataset
                    dataset.append({
                        "turn": turn,
                        #"start_pos": start_pos,
                        #"end_pos": end_pos,
                        "board_state": b.tolist(),
                        "winner": winner
                    })

                current_state = []  # Reset per la prossima partita

    # Creazione del DataFrame
    df = pd.DataFrame(dataset)
    df['winner'] = winner
    return df



S_symmetry_matrix = np.array([[0, 0, 0, 0, 0, 0, 0, 0, 1],
                              [0, 0, 0, 0, 0, 0, 0, 1, 0],
                              [0, 0, 0, 0, 0, 0, 1, 0, 0],
                              [0, 0, 0, 0, 0, 1, 0, 0, 0],
                              [0, 0, 0, 0, 1, 0, 0, 0, 0],
                              [0, 0, 0, 1, 0, 0, 0, 0, 0],
                              [0, 0, 1, 0, 0, 0, 0, 0, 0],
                              [0, 1, 0, 0, 0, 0, 0, 0, 0],
                              [1, 0, 0, 0, 0, 0, 0, 0, 0]])


def generate_8_identical_boards(board):
    #Append of the original board
    boards=[]
    if len(board)!=9:
        return boards

    boards.append(board.flatten())

    #Append ot the other 3 boards rotated 90° each time counterclockwise
    for i in range(3):
        #initial_board = np.rot90(initial_board)
        board = (board @ S_symmetry_matrix).T
        boards.append(board.flatten())

    #Append of the board mirrored
    board = board[:, ::-1]
    boards.append(board.flatten())

    #Append ot the other 3 boards rotated 90° each time counterclockwise
    for i in range(3):
        #symmetric_board = np.rot90(symmetric_board)
        board = (board @ S_symmetry_matrix).T
        boards.append(board.flatten())
    
    return boards



def convert_board_state(board_state):
    converted_boards = []
    board_matrix = []
    for row in board_state:
        row_values = []
        for cell in row:
            if cell == 'O':
                row_values.append(0)  # Cella vuota
            elif cell == 'B':
                row_values.append(1)  # Pezzo nero
            elif cell == 'W':
                row_values.append(2)  # Pezzo bianco
            elif cell == 'K':
                row_values.append(3)  # Re
            elif cell == 'T':
                row_values.append(4)  # Trono
        board_matrix.append(row_values)

    return generate_8_identical_boards(np.array(board_matrix)) # Appiattiamo la matrice in un array 1D









def process_directory(directory_path):
    all_data = []

    # Esamina tutti i file nella directory
    for filename in os.listdir(directory_path):
        if filename.endswith('.txt'):  # Solo i file .txt
            file_path = os.path.join(directory_path, filename)
            print(f"Processing file: {file_path}")
            
            # Esegui il parsing e aggiungi i dati al DataFrame globale
            df = parse_tablut_log(file_path)
            all_data.append(df)
    
    # Unisci tutti i DataFrame in uno solo
    if all_data:
        final_df = pd.concat(all_data, ignore_index=True)
        # Salva il DataFrame finale nel file CSV
        final_df.to_csv('tablut_dataset.csv', index=False)
        print("File CSV salvato come 'tablut_dataset.csv'")
    else:
        print("Nessun file .txt trovato nella directory.")

# Esegui il parsing dei file nella directory
directory_path = '../../games/'  # Sostituisci con il percorso della tua directory
process_directory(directory_path)

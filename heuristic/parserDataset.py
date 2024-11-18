import os
import re
import pandas as pd

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
                
                # Aggiungi al dataset
                dataset.append({
                    "turn": turn,
                    #"start_pos": start_pos,
                    #"end_pos": end_pos,
                    "board_state": current_state,
                    "winner": winner
                })
                current_state = []  # Reset per la prossima partita

    # Creazione del DataFrame
    df = pd.DataFrame(dataset)
    df['winner'] = winner
    return df

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
        print("File CSV salvato come 'tablut_dataset_with_winner.csv'")
    else:
        print("Nessun file .txt trovato nella directory.")

# Esegui il parsing dei file nella directory
directory_path = 'games/'  # Sostituisci con il percorso della tua directory
process_directory(directory_path)

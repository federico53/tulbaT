import numpy as np
import pandas as pd
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
from tensorflow.keras.optimizers import Adam

# Funzione per convertire lo stato della board in formato numerico
def convert_board_state(board_state):
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
    return np.array(board_matrix).flatten()  # Appiattiamo la matrice in un array 1D

# Funzione per preprocessare i dati
def preprocess_data(df):
    X = []
    y = []

    # Filtro delle righe senza il campo 'winner'
    df = df.dropna(subset=['winner'])

    for _, row in df.iterrows():
        # Converti la board state in un array numerico
        board_state = eval(row['board_state'])  # Converti la stringa della lista in un oggetto Python
        board_numeric = convert_board_state(board_state)

        if len(board_numeric) != 81:
            #print(f"Errore: board state non ha 81 celle: {len(board_numeric)}")
            continue

        # Ottieni il turno (0 per W, 1 per B)
        turn = 0 if row['turn'] == 'W' else 1

        # Ottieni il vincitore (1 per WW, 0 per BB)
        winner = 1 if row['winner'] == 'WW' else 0

        # Aggiungi i dati
        X.append(np.concatenate([board_numeric, [turn]]))  # Aggiungi il turno alla board
        y.append(winner)

    print(f"Shape of X: {len(X)}")
    print(f"Shape of y: {len(y)}")
    print(f"Sample of X[0]: {X[0]}")

    return np.array(X), np.array(y)

# Funzione per creare e allenare il modello
def create_model(input_dim):
    model = Sequential()
    model.add(Dense(128, input_dim=input_dim, activation='relu'))
    model.add(Dense(64, activation='relu'))
    model.add(Dense(32, activation='relu'))
    model.add(Dense(1, activation='sigmoid'))  # Probabilità di vittoria (0 o 1)
    model.compile(loss='binary_crossentropy', optimizer=Adam(), metrics=['accuracy'])
    return model

# Carica il dataset da un file CSV
file_path = 'tablut_dataset.csv'  # Sostituisci con il percorso del tuo file CSV
df = pd.read_csv(file_path)

# Preprocessa i dati
X, y = preprocess_data(df)

# Crea il modello
model = create_model(X.shape[1])

# Allena il modello
model.fit(X, y, epochs=10, batch_size=32, validation_split=0.2)

# Usa il modello per predire la probabilità di vittoria (per un esempio di board)
example_board = np.array(convert_board_state(eval(df.iloc[0]['board_state'])))
example_turn = np.array([0])  # Turno 'W'
example_input = np.concatenate([example_board, example_turn]).reshape(1, -1)

prediction = model.predict(example_input)
print(f"Probabilità che il bianco vinca: {prediction[0][0]:.2f}")

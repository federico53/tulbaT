import logging
import os
from datetime import datetime

# Crea la cartella dei log se non esiste
if not os.path.exists('./logs'):
    os.makedirs('./logs')

def setup_logger(name, file_level=logging.DEBUG, console_level=logging.INFO):
    """Configura e restituisce un logger con un file di log univoco per ogni esecuzione."""
    # Genera un timestamp per il nome del file di log
    timestamp = datetime.now().strftime("%Y.%m.%d_%H:%M:%S")
    log_file = f'./logs/{timestamp}.log'
    
    formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')

    # Gestore per il log su file con livello DEBUG
    file_handler = logging.FileHandler(log_file)
    file_handler.setLevel(file_level)
    file_handler.setFormatter(formatter)

    # Gestore per il log su console con livello INFO
    console_handler = logging.StreamHandler()
    console_handler.setLevel(console_level)
    console_handler.setFormatter(formatter)

    # Configura il logger
    logger = logging.getLogger(name)
    logger.setLevel(logging.DEBUG)  # Imposta il livello minimo per il logger
    logger.addHandler(file_handler)
    logger.addHandler(console_handler)

    # Evita la duplicazione dei messaggi
    logger.propagate = False

    return logger

# Logger principale per l'applicazione
logger = setup_logger('tablut_logger')

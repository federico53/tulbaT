import os
import logging

# Crea la cartella dei log se non esiste
if not os.path.exists('heuristic/logs'):
    os.makedirs('heuristic/logs')

# Configurazione del logger
def setup_logger(name, log_file, level=logging.DEBUG):  # Imposta il livello a DEBUG
    """Configura e restituisce un logger."""
    formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')

    handler = logging.FileHandler(log_file)
    handler.setFormatter(formatter)

    console_handler = logging.StreamHandler()
    console_handler.setFormatter(formatter)

    logger = logging.getLogger(name)
    logger.setLevel(level)
    logger.addHandler(handler)
    logger.addHandler(console_handler)

    # Evita la duplicazione dei messaggi
    logger.propagate = False

    return logger

# Logger principale per l'applicazione
logger = setup_logger('tablut_logger', 'heuristic/logs/game.log')
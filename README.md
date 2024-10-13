# Tulbat
Tulbat project for AI

## **`player_heuristic.py`**

Questo script Python è progettato per giocare a Tablut, utilizzando un approccio basato su euristiche per valutare e selezionare le mosse.

### **Funzionalità principali:**

- **Networking**: Lo script si connette a un server di Tablut tramite socket, consentendo di comunicare con altri giocatori in tempo reale.

- **Valutazione delle mosse**: Contiene funzioni per valutare le mosse basate su specifiche euristiche per i pezzi neri e bianchi:
  - **Per il Nero**: L'euristica si concentra sull'intrappolamento del re, sull'attacco ai pezzi bianchi, sulla cattura dei bianchi e sul blocco del re per impedirgli di raggiungere posizioni di vittoria.
  - **Per il Bianco**: L'euristica enfatizza la protezione del re e l'evitare la prossimità ai pezzi neri.

- **Validazione delle mosse**: Prima di eseguire una mossa, lo script verifica la sua validità in base alle regole del gioco, assicurandosi che le mosse non atterrino su spazi occupati o in aree non consentite.

- **Generazione delle mosse**: Lo script genera tutte le possibili mosse valide per i pezzi del giocatore corrente, tenendo conto dello stato del campo e delle posizioni dei pezzi.

- **Ciclo di gioco**: Mantiene un ciclo continuo per aspettare il turno del giocatore, ricevere aggiornamenti sullo stato del gioco dal server e inviare la mossa migliore selezionata al server.

### **Utilizzo:**

Per eseguire lo script, l'utente deve fornire il proprio nome da giocatore e scegliere un colore (bianco o nero) prima di connettersi al server. Una volta connesso, il giocatore parteciperà al gioco effettuando mosse basate sulle valutazioni euristiche.

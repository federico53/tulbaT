# Implementazione

L'euristica è implementata come funzione lineare, è stato ottimizzato il codice per eseguire in maniera più efficiente e fare meno cicli possibili. Per una approssimazione ottimale è importante settare in maniera corretta i pesi dei vari aspetti del gioco sia per il bianco sia per il nero e tenere in considerazione il fatto che essi si influenzano tra di loro nel calcolo della mossa migliore

## White heuristic
- Number of white pieces
- Winning direction position of the king
- Free sides of the king
- Victory

## Black heuristic
- Number of black pieces
- Average distance from king (no 8 adj cells)
- 8 adj cells points
- Victory

# Aspetti da migliorare

- [x] ~~Rivedere bene le conclusioni delle partite~~
- [x] ~~Passaggio corretto degli argomenti come da specifiche~~
- [x] ~~Eliminare parti di codice superflue o ottimizzabili~~
- [x] ~~Implementare dinamiche "paracadute" per inviare in sicurezza una mossa (Luca)~~
- [x] ~~Multi-threading per migliorare ulteriormente le performance (Luca)~~
- [ ] Implementare una logica per non pareggiare se in vantaggio
- [ ] Rivedere cut size per minmax
- [ ] Aumento dinamico della depth in base alla dimensione della generate_all_possible_move
- [ ] Errore mossa nera da A1 a A1 (logs/2024.11.25_00:25:17.log)

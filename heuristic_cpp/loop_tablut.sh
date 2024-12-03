#!/bin/bash

# Variabile per tenere traccia dei cicli
turn=0

while [ $turn -lt 50 ]; do
    # Imposta Java 8 per il server
    export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64/jre  # Modifica con il percorso corretto di Java 8
    export PATH=$JAVA_HOME/bin:$PATH

    # Vai alla directory del server e avvialo
    cd /home/federico/Documents/AI/faikr/TablutCompetition/Tablut
    ant gui-server &  # Avvia il server in background

    sleep 5 #pausa di 5 secondi per inizializzare il server

    # Decidi i colori in base al numero di cicli (turn)
    if ((turn % 2 == 0)); then
        CLIENT1_COLOR="WHITE"
        CLIENT2_COLOR="BLACK"
    else
        CLIENT1_COLOR="BLACK"
        CLIENT2_COLOR="WHITE"
    fi

    # Imposta Java 21 per il primo client
    export JAVA_HOME=/usr/lib/jvm/java-21-openjdk-amd64  # Modifica con il percorso corretto di Java 21
    export PATH=$JAVA_HOME/bin:$PATH

    # Vai alla directory del primo client e avvialo con il colore deciso
    cd /home/federico/Documents/Projects/tulbaT/other/
    ./runmyplayer.sh $CLIENT1_COLOR &  # Avvia il primo client con il colore assegnato in background

    # Vai alla directory del secondo client e avvialo con il colore opposto
    cd /home/federico/Documents/Projects/tulbaT/heuristic_cpp
    ./runmyplayer.sh $CLIENT2_COLOR 60 127.0.0.1 5 100 &  # Avvia il secondo client con il colore assegnato in background

    # Attendere che i processi finiscano prima di ripetere
    wait

    # Incrementa il numero del turno
    ((turn++))

    # Aggiungi una pausa tra un ciclo e l'altro (opzionale)
    sleep 5  # Pausa di 5 secondi tra i cicli (puoi cambiare la durata)
done

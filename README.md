# Ingegneria-del-software
Cartella per il progetto "L'ingegneria della scacchiera" per Ingegneria del Software del corso laurea triennale Informatica, condotto da D'annibale Luigi e Venturini Daniele

## Prerequisiti

Questo progetto richiede alcune librerie esterne per essere compilato correttamente su Linux. Di seguito si trovano le istruzioni necessarie per l'installazione delle dipendenze e la compilazione del progetto.

## Librerie

### wxWidgets
La libreria `wxWidgets` è necessaria per l'interfaccia grafica del client.

```bash
sudo apt install libwxgtk3.0-gtk3-dev
```

### nlohmann json
Questa libreria è utilizzata per la gestione della comunicazione in JSON.
```bash
sudo apt install nlohmann-json3-dev
```

### PostgreSQL
PostgreSQL è utilizzato come database per il progetto.
```bash
sudo apt install libpq-dev
```

### Hiredis
`hiredis` è una libreria C per interagire con Redis.
```bash
sudo apt install libhiredis-dev
```

## Compilazione
Il progetto è suddiviso in due componenti principali: Client e Server. Ciascuno di essi ha una propria directory e richiede una compilazione separata con il proprio file Cmake.

### Compilazione Client
Avviare la Cmake nella cartella del Client e spostare l'eseguibile `chess` nella cartella `bin`

### Compilazione Server
Avviare la Cmake nella cartella del Server e spostasre l'eseguibile `Server` nella cartella `bin` (opzionale)

## Problemi di Compilazione

Se dovesse incontrare problemi con l'installazione delle librerie o la compilazione del progetto, siamo disponibili a far vedere il progetto direttamente e spiegare il suo funzionamento.


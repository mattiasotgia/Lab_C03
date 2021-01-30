-- 2019-12-04 -- versione 1.3 --

Per l'esame di Lab1B avete a disposizione i seguenti programmi


 * Programma per il calcolo di media, ed altre grandezze statistiche da un
   elenco di valori letti da file

   - File sorgente: media.cpp
   - Comando di compilazione: g++ -o media media.cpp
   - Formato del file di input: valori numerici separati da spazio, a capo, etc
   - Esempio d'uso: ./media dati.dat
   - Esempio di output (ottenuto con il file media.dat):

     5 valori letti dal file media.dat
     Media                   = 3.46
     Varianza                = 4.3264
     Varianza Empirica       = 5.408
     Dev. Standard           = 2.08
     Dev. Standard Adattata  = 2.32551
     Errore Standard         = 1.04



  * Programma per il metodo dei minimi quadrati.
    Il programma assume che le incertezze sull'asse delle x siano trascurabili.

    - File sorgente: minimiquadrati.cpp
    - Comando di compilazione: g++ -o minimiquadrati minimiquadrati.cpp
    - Formato del file di input: una riga per ogni punto su cui fare il fit,
      ciascuna riga composta da 3 valori: X, Y, eY
    - Esempio d'uso: ./minimiquadrati datimin.dat
    - Esempio di output (ottenuto con il file datimin.dat):
   
      8 punti letti da datimin.dat
      Calcolo dei minimi quadrati con la retta y = m*x + q
      m = 4.04874 +/- 0.0661461
      q = -0.00597211 +/- 0.0160821
      chi2 / NDF  = 1.74783 / 6



  * Macro di ROOT per graficare un set di dati ed una retta sovrapposta

    - File sorgente: plot_simple.C
    - Esempio d'uso da terminale: root -l plot_simple.C
    - Esempio d'uso dal prompt di root: .x plot_simple.C
    - File di dati da usare come test: datifit.dat
    - Parametri da modificare (vedere spiegazione nella macro)
      - Nome del file di dati da leggere
      - Numero di colonne contenute nel file di dati (3 o 4)
      - pendenza e intercetta della retta da disegnare



 *  Macro di ROOT per leggere un file, fare eventualmente manipolazione dei
    dati letti, fare un grafico ed eseguire un fit su di esso.
    La macro contiene l'implementazione di un fit lineare, ma seguendo i
    commenti al suo interno e' possibile modificarla per eseguire manipolazione
    dei dati e fit di andamenti non lineari
    
    - File sorgente: plot_fit.C
    - Esempio d'uso da terminale: root -l plot_fit.C
    - Esempio d'uso da prompt di root: .x plot_fit.C
    - File di dati da usare come test: datifit.dat
    - Parametri da modificare: vedere spiegazione nella macro



 *  Macro di ROOT per l'esperienza sul pendolo di Kater.
    Esegue fit di due parabole su un file di dati del tipo dati_kater.dat, le
    interseca e calcola il periodo di isocronia come fatto nella quinta
    esercitazione di Lab1A 

    - File sorgente: kater.C
    - Esempio d'uso da terminale: root -l kater.C
    - Esempio d'uso da prompt di root: .x kater.C
    - File di dati da usare come test: dati_kater.dat
    - Parametri da modificare: nome del file di input
    - Formato del file di input (per ogni riga): X eX T1 eT1 T2 eT2


Changelog

- Versione 1.3 (2019-12-04): aggiunta stampa stato di convergenza del fit in kater.C
- Versione 1.2 (2019-01-14): aggiunti chi2 e prob alla macro kater.C

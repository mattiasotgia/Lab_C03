{
   //----------------------------------------------
   //  Macro ROOT per fare il grafico di una serie di punti ed
   //  eventualmente sovrapporvi una retta
   //
   // Esempio di esecuzione (da terminale): root plot_simple.C
   //----------------------------------------------
   //
   // Il file del file di input deve essere del tipo (4 colonne)
   
   //   x_1 y_1 ex_1 ey_1
   //   x_2 y_2 ex_2 ey_2
   //   ..  ..  ..   .. 
   //   x_N y_N ex_N ey_N
   //
   // oppure del tipo (3 colonne)
   //
   //   x_1 y_1 ey_1
   //   x_2 y_2 ey_2
   //   ..  ..  .. 
   //   x_N y_N ey_N
   //
   //
   // Si possono modificare i seguenti parametri:
   //
   //
   // - Nome del file di input
   //
   const char* fileName = "datifit.dat";
   //
   //
   // - Parametri della retta da disegnare: y = m*x + q
   //
   double m = 4.04;
   double q = 0;
   //
   //
   // - Numero di colonne nel file (scegliere una delle due opzioni):
   //
   //const char* fmt = "%lf %lf %lf"; //scommentare per file con 3 colonne
   const char* fmt = "%lf %lf %lf %lf"; //scommentare per file con 4 colonne




   // creazione del grafico e lettura del file
   TGraphErrors gr(fileName, fmt);
   if (gr.GetN()==0){
      cout << "Errore nell'apertura/lettura del file: " << fileName << endl;
      return;
   }

   
   // Calcolo dell'intervallo di definizione della funzione a partire dai
   // dati contenuti nel grafico
   double xMin = TMath::MinElement(gr.GetN(), gr.GetX());
   double xMax = TMath::MaxElement(gr.GetN(), gr.GetX());
   double clearance = 0.1*(xMax - xMin);
   xMin -= clearance;
   xMax += clearance;
   
   // Creazione della funzione e inizializzazione dei parametri
   TF1 f("f","[0]+x*[1]", xMin, xMax);
   f.SetParameter(0, q);
   f.SetParameter(1, m);

   // Disegno il grafico determinando automaticamente gli assi (A)
   // e rappresentando ogni punto con un pallino (P)
   gr.SetTitle("Descrizione del grafico");
   gr.Draw("AP");
   gr.GetXaxis()->SetTitle("asse X [x]");
   gr.GetYaxis()->SetTitle("asse Y [y]");
   
   // faccio il grafico della funzione (senza fare il fit
   f.Draw("SAME");
}

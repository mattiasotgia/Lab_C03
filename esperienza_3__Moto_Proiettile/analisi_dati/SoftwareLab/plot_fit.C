{
   //-------------------------------------------------------
   //  Macro ROOT per fare plot e fit di una serie di punti
   //-------------------------------------------------------
   /*
    * Il file del file di input deve avere il formato
    *
    *   x_1 y_1 ex_1 ey_1
    *   x_2 y_2 ex_2 ey_2
    *   ..  ..  ..   .. 
    *   x_N y_N ex_N ey_N
    *
    * Altri formati possono essere trattati modificando la parte della macro
    * che legge il file
    */
   
   const char* fileName = "datifit.dat";

   /* controllo che il file esista e sia leggibile */
   ifstream inputfile(fileName); 
   if (!inputfile.good()) {  
      cout << "Impossibile leggere il file: " << fileName << endl;
      return;
   }

   /* lettura dei dati dal file */
   TGraphErrors gr;
   int i = 0;
   double x, y, ex, ey;
   while (inputfile >> x >> y >> ex >> ey) {
      //Qui e' possibile aggiungere un'eventuale propagazione degli errori
      gr.SetPoint(i, x, y);
      gr.SetPointError(i, ex, ey);
      i++;
   }

   
   // Calcolo dell'intervallo di definizione della funzione a partire dai
   // dati contenuti nel grafico
   double xMin = TMath::MinElement(gr.GetN(), gr.GetX());
   double xMax = TMath::MaxElement(gr.GetN(), gr.GetX());
   double clearance = 0.1*(xMax - xMin);
   xMin -= clearance;
   xMax += clearance;

   
   /*
    * Definizione della funzione ed inizializzazione dei parametri
    *
    *
    * Alcuni esempi di funzioni:
    *
    * "[0] + [1]*x"           --> polinomio di primo grado, 2 parametri
    *
    * "[0] + [1]*x + [2]*x*x" --> polinomio di secondo grado, 3 parametri
    *
    * "[0]*exp(-[1]*x)"       --> esponenziale decrescente, 2 parametri
    *
    * "[0]*sin([1]*x - [2])"   --> funzione armonica di ampiezza, frequenza
    *                              e fase configurabili, 3 parametri
    */
   TF1 f("f","[0]+x*[1]", xMin, xMax);
   f.SetParameter(0,0);
   f.SetParameter(1,4);

   /* plot dei dati */
   gr.Draw("AP");
   gr.SetTitle("Descrizione del grafico");
   gr.GetXaxis()->SetTitle("asse X [x]");
   gr.GetYaxis()->SetTitle("asse Y [y]");

   // Fit con la funzione f (che viene automaticamente graficata sui dati)
   gr.Fit("f");
}

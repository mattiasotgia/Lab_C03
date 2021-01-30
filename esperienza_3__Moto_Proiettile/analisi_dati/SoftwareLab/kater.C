{


  ifstream f("dati_kater.dat");


  TGraphErrors gr1;
  gr1.SetMarkerSize(0.8);
  gr1.SetMarkerStyle(22);
  gr1.SetMarkerColor(kRed);
  
  TGraphErrors gr2;
  gr2.SetMarkerSize(0.8);
  gr2.SetMarkerStyle(23);
  gr2.SetMarkerColor(kBlue);
  
  int i=0;
  double X, eX, T1, eT1, T2, eT2;
  while(f >> X >> eX >> T1 >> eT1 >> T2 >> eT2) {
     gr1.SetPoint(i, X, T1);
     gr2.SetPoint(i, X, T2);
     gr1.SetPointError(i, eX, eT1);
     gr2.SetPointError(i, eX, eT2);
     i++;
  }

  /*
   * Attenzione:
   * provare ad inizializzare i parametri delle funzioni fino quando non
   * si riesce a far convergere il fit.
   */
  TF1 f1("f1","[0]+[1]*x+[2]*x*x");
  f1.SetLineColor(kRed);
  f1.SetParameter(0,1.7);
  f1.SetParameter(1,0.001);
  f1.SetParameter(2,0.0);

  TF1 f2("f2","[0]+[1]*x+[2]*x*x");
  f2.SetLineColor(kBlue);
  f2.SetParameter(0,1.7);
  f2.SetParameter(1,0.001);
  f2.SetParameter(2,0.0);

  gr1.Draw("AP");
  gr1.GetXaxis()->SetTitle("Asse x [x]");
  gr1.GetYaxis()->SetTitle("Asse y [y]");
  gr2.Draw("P");
  TFitResultPtr fitResult1 = gr1.Fit("f1", "ES");
  TFitResultPtr fitResult2 = gr2.Fit("f2", "ES");

  // T1(x) = a1*x^2 + b1*x + c1
  double a1 = f1.GetParameter(2);
  double b1 = f1.GetParameter(1);
  double c1 = f1.GetParameter(0);

  // T2(x) = a2*x^2 + b2*x + c2
  double a2 = f2.GetParameter(2);
  double b2 = f2.GetParameter(1);
  double c2 = f2.GetParameter(0);

  
  // faccio la differenza tra T1(x) e T2(x) e impongo che sia uguale a zero
  // (ovvero risolvo l'equazione di secondo grado con coefficienti
  //  a2-a1, b2-b1 e c2-c1).
  double a = a2 - a1;
  double b = b2 - b1;
  double c = c2 - c1;
  
  double delta = b*b-4*a*c;
  double x1 = (-b + sqrt(delta)) / (2*a);
  double x2 = (-b - sqrt(delta)) / (2*a);
  

  // tra le due soluzioni, scelgo quella compresa tra xMin e xMax
  double xMin=gr1.GetXaxis()->GetXmin();
  double xMax=gr1.GetXaxis()->GetXmax();
  double x=0;
  if (x1 > xMin && x1 < xMax)
     x = x1;
  else
     x = x2;
  
  // calcolo il periodo di isocronia da una delle due parabole e stampo il
  // risultato su schermo
  double Tiso = a1*x*x+b1*x+c1;
  cout << endl;
  cout << " ------------------------------- " << endl;
  cout << "  fit1 ";
  if(fitResult1->IsValid())
     cout << "(SUCCESS)";
  else
     cout << "(FAILED)";
  cout << ": chi2 / NDF (prob): " << f1.GetChisquare()
       << " / " << f1.GetNDF() << " (" << f1.GetProb() << ")" << endl; 
  cout << "  fit2 ";
  if(fitResult2->IsValid())
     cout << "(SUCCESS)";
  else
     cout << "(FAILED)";
  cout << ": chi2 / NDF (prob): " << f2.GetChisquare()
       << " / " << f2.GetNDF() << " (" << f2.GetProb() << ")" << endl; 
  cout << "  Ascissa intersezione: " << x << endl;
  cout << "  Periodo di isocronia: " << Tiso << endl;
  cout << " ------------------------------- " << endl;
  cout << endl;
}

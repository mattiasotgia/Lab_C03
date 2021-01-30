#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
//#include <TMath.h>

using namespace std;


// calcolo del chi quadro
double Chiquadro(vector<double>& x,
		 vector<double>& y,
		 vector<double>& ey,
		 double m,
		 double q)
{
   double chi2 = 0;
   for (int i = 0; i < x.size(); i++)
      chi2 += pow( ((y[i]-(m*x[i]+q)) / ey[i]), 2);
  return chi2;
}


// funzione intermedia per il metodo dei minimi quadrati
double S0(vector<double>& ey)
{
   double sum = 0;
   for(int i = 0; i < ey.size(); i++)
      sum += 1./(ey[i]*ey[i]);
   return sum;
}


// funzione intermedia per il metodo dei minimi quadrati
double S1(vector<double>& v1, vector<double>& ey)
{
   // assume v1 and ey have the same size
   double sum = 0;
   for(int i = 0; i < ey.size(); i++)
      sum += v1[i]/(ey[i]*ey[i]);
   return sum;
}


// funzione intermedia per il metodo dei minimi quadrati
double S2(vector<double>& v1, vector<double>& v2, vector<double>& ey)
{
   // assume v1 and ey have the same size
   double sum = 0;
   for(int i = 0; i < ey.size(); i++)
      sum += v1[i]*v2[i]/(ey[i]*ey[i]);
   return sum;
}


// Intercetta della retta calcolata con il metodo dei minimi quadrati
double Intercetta(vector<double>& x, vector<double>& y, vector<double>& ey)
{
   double s0 = S0(ey);
   double sx = S1(x, ey);
   double sy = S1(y, ey);
   double sxx = S2(x, x, ey);
   double sxy = S2(x, y, ey);
   return (sy*sxx - sx*sxy)/(sxx*s0 - sx*sx);
}


// Pendenza della retta calcolata con il metodo dei minimi quadrati
double Pendenza(vector<double>& x, vector<double>& y, vector<double>& ey)
{
   double s0 = S0(ey);
   double sx = S1(x, ey);
   double sy = S1(y, ey);
   double sxx = S2(x, x, ey);
   double sxy = S2(x, y, ey);
   return (s0*sxy - sx*sy)/(sxx*s0 - sx*sx);
}

// incertezza sull'intercetta
double ErrIntercetta(vector<double>& x, vector<double>& y, vector<double>& ey)
{
   double s0 = S0(ey);
   double sx = S1(x, ey);
   double sxx = S2(x, x, ey);
   return sqrt(sxx/(sxx*s0 - sx*sx));
}

// incertezza sulla pendenza
double ErrPendenza(vector<double>& x, vector<double>& y, vector<double>& ey)
{
   double s0 = S0(ey);
   double sx = S1(x, ey);
   double sxx = S2(x, x, ey);
   return sqrt(s0/(sxx*s0 - sx*sx));
}






// programma principale
int main(int argc, char** argv)
{
   
   // help in caso di comando errato
   if (argc != 2)
   {
      cout << "Specificare il nome del file. Esempio:" << endl;
      cout << " ./minquad datimin.dat" << endl;
      cout << "Il formato del file deve essere del tipo: " << endl;
      cout << " x_1 y_1 ey_1" << endl;
      cout << " x_2 y_2 ey_2" << endl;
      cout << " ... ... ... " << endl;
      cout << " x_N y_N ey_N" << endl;
      return 1;
   }

   // controllo che il file esista e sia leggibile
  ifstream ifile(argv[1]);
  if (!ifile.good())
  {
     cout << "Impossibile leggere il file: " << argv[1] << endl;
    return 0;
  }

  vector<double> x;
  vector<double> y;
  vector<double> ey;

  // lettura del file e riempimento dei vector con x, y, ed ey
  double xVal, yVal, yErr;
  while (ifile>> xVal >> yVal >> yErr)
  {
     x.push_back(xVal);
     y.push_back(yVal);
     ey.push_back(yErr);
  }

  // stampa su schermo quanti punti contiene il file
  cout << x.size() << " punti letti da " << argv[1] << endl;

  // calcolo dei parametri della retta con il metodo dei minimi quadrati
  // retta parametrizzata come y = m*x + q
  double m = Pendenza(x, y, ey);
  double em = ErrPendenza(x, y, ey);
  double q = Intercetta(x, y, ey);
  double eq = ErrIntercetta(x, y, ey);
  double chi2 = Chiquadro(x, y, ey, m, q);
  int ndf = x.size() - 2;

  // stampa dei risultati su schermo
  cout << "Calcolo dei minimi quadrati con la retta y = m*x + q" << endl;
  cout << "m = " <<  m << " +/- " << em << endl; 
  cout << "q = " <<  q << " +/- " << eq << endl;
  cout << "chi2 / NDF " << " = " << chi2 << " / " << ndf << endl;
  //cout << "Prob(chi2) = " << TMath::Prob(chi2, ndf) << endl;

  return 0;
}

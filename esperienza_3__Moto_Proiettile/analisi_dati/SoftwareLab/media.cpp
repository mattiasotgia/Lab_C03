#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

/* media degli elementi del vettore */
double Media(vector<double>& v)
{
   double media = 0;
   for(int i = 0; i < v.size(); i++)
      media += v[i];
   return media / v.size();
}

/* media dei quadrati degli elementi del vettore */
double MediaQuadrati(vector<double>& v)
{
   double mediaQuadrati = 0;
   for(int i = 0; i < v.size(); i++)
      mediaQuadrati += v[i]*v[i];
   return mediaQuadrati / v.size();
}

/* varianza: <x^2> - <x>^2 */
double Varianza(vector<double>& v)
{
   double media = Media(v);
   double mediaQuadrati = MediaQuadrati(v);
   return mediaQuadrati - media*media;
}

/* varianza empirica: varianza * n/(n-1) */
double VarianzaEmpirica(vector<double>& v)
{
   double var = Varianza(v);
   return var * v.size()/(v.size() - 1);
}

/* deviazione standard: sqrt(varianza) */
double DevStandard(vector<double>& v)
{
   return sqrt(Varianza(v));
}


/* deviazione standard: sqrt(varianza) */
double DevStandardAdattata(vector<double>& v)
{
   return sqrt(VarianzaEmpirica(v));
}


/* deviazione standard: sqrt(varianza) */
double ErroreStandard(vector<double>& v)
{
   return DevStandardAdattata(v)/sqrt(v.size());
}




int main(int argc, char** argv)
{

  if (argc<=1){
    cout << "Specificare il nome del file dopo l'eseguibile. Esempio:" << endl;
    cout << " ./media dati.dat" << endl;
    cout << "Il formato del file deve essere del tipo: " << endl;
    cout << " x_1 " << endl;
    cout << " x_2 " << endl;
    cout << " ... " << endl;
    cout << " x_N " << endl;
    return 1;
  }
  
  ifstream ifile(argv[1]);
  if (!ifile.good())
  {
     cout << "Impossibile aprire il file: " << argv[1] << endl;
    return 2;
  }

  vector<double> valori;
  double val;
  while (ifile>>val)
     valori.push_back(val);

  cout << valori.size() << " valori letti dal file " << argv[1] << endl;
  if(valori.empty())
     return 0;
  
  cout << "Media                   = " << Media(valori) << endl;
  cout << "Varianza                = " << Varianza(valori) << endl;
  cout << "Varianza Empirica       = " << VarianzaEmpirica(valori) << endl;
  cout << "Dev. Standard           = " << DevStandard(valori) << endl;
  cout << "Dev. Standard Adattata  = " << DevStandardAdattata(valori) << endl;
  cout << "Errore Standard         = " << ErroreStandard(valori) << endl;
  return 0;
}

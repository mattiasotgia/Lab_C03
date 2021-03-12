#include "../../stat/stat.h"
#include <vector>
#include <iostream>
#include <fstream>


using namespace std;



int main(){

   ifstream p_oscillazioni("../dati/p_oscillazioni_NNx10.txt");
   ofstream output("../misc/p_osc_err.log");

   stat s1;

   for(int i = 0; i<7; i++){

      vector<double> v;
      for(int j=0; j<10; j++){
         double a;
         p_oscillazioni >> a;
         v.push_back(a/10);
      }
      s1.read_values(v);
      output << "T" << i+1 << " " << s1.avg() << " " << s1.std_err() << endl;
      v.clear();
      s1.clear();
   }

}
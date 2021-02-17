#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <string>



using namespace std;

// recalling functions
// functions that may come useful
// -------------------------------------------------- statistics_values
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

int main(){

    ifstream ed("../dati/dati_grezzi/eugenio_dormicchi_nuovi.txt");
    ifstream rp("../dati/dati_grezzi/riccardo_pizzimbone.txt");
    ifstream ms("../dati/dati_grezzi/mattia_sotgia.txt");
    ofstream ed_out("../dati/ed_dati.dat");
    ofstream rp_out("../dati/rp_dati.dat");
    ofstream ms_out("../dati/ms_dati.dat");
    ofstream ed_out_lin("../dati/ed_dati_lin.dat");
    ofstream rp_out_lin("../dati/rp_dati_lin.dat");
    ofstream ms_out_lin("../dati/ms_dati_lin.dat");

    double x, d, m;
    double eL, e_t, e_d, e_m;
    string txt;
    double L_i, T_i_i;
    vector<double> L;
    vector<double> T_i_v;
    vector<double> T;
    vector<double> eT;
 
    ed>>x>>d>>m;
    ed>>eL>>e_t>>e_d>>e_m;

    for(int i=0;i<5; i++){

        ed>>txt;
        ed>>L_i;
        L.push_back(L_i);
        ed>>txt;
        for(int j=0; j<10;j++){

            ed>>T_i_i;
            double T_i_temp=T_i_i/10;
            T_i_v.push_back(T_i_temp);

        }
        T.push_back(Media(T_i_v));
        eT.push_back(ErroreStandard(T_i_v));
        T_i_v.clear();
    }

    for(int i=0; i<5; i++){

        ed_out<<L[i]*(1e-3)<<" "<<T[i]<<" "<<eL/sqrt(3)*(1e-3)<<" "<<eT[i]<<endl;
        ed_out_lin<<L[i]*(1e-3)<<" "<<T[i]*T[i]<<" "<<eL/sqrt(3)*(1e-3)<<" "<<2*T[i]*eT[i]<<endl;
    }

    T.clear();
    L.clear();
    eT.clear();


    // inizio riccardo
    rp>>x>>d>>m;
    rp>>eL>>e_t>>e_d>>e_m;

    for(int i=0;i<5; i++){

        rp>>txt;
        rp>>L_i;
        L.push_back(L_i);
        rp>>txt;
        for(int j=0; j<10;j++){

            rp>>T_i_i;
            double T_i_temp=T_i_i/10;
            T_i_v.push_back(T_i_temp);

        }
        T.push_back(Media(T_i_v));
        eT.push_back(ErroreStandard(T_i_v));
        T_i_v.clear();
    }

    
    for(int i=0; i<5; i++){

        rp_out<<L[i]*(1e-3)<<" "<<T[i]<<" "<<eL/sqrt(3)*(1e-3)<<" "<<eT[i]<<endl;
        rp_out_lin<<L[i]*(1e-3)<<" "<<T[i]*T[i]<<" "<<eL/sqrt(3)*(1e-3)<<" "<<2*T[i]*eT[i]<<endl;
    }

    T.clear();
    L.clear();
    eT.clear();


    // inizio mattia sotgia
    ms>>x>>d>>m;
    ms>>eL>>e_t>>e_d>>e_m;
    double eL_stat=eL/sqrt(3);
    double ed_stat=e_d/sqrt(3);
    vector<double> eL_v;

    for(int i=0;i<7; i++){

        ms>>txt;
        ms>>L_i;
        L.push_back(sqrt(pow(L_i, 2)+pow(d/2, 2)));
        double eL_i=sqrt((pow(2*L_i/(2*sqrt(pow(L_i, 2)+pow(d/2, 2))), 2)*eL_stat*eL_stat)+(pow(d*0.5/(2*sqrt(pow(L_i, 2)+pow(d/2, 2))), 2)*ed_stat*ed_stat));
        eL_v.push_back(eL_i);

        ms>>txt;
        for(int j=0; j<10;j++){

            ms>>T_i_i;
            double T_i_temp=T_i_i/10;
            T_i_v.push_back(T_i_temp);

        }
        T.push_back(Media(T_i_v));
        eT.push_back(ErroreStandard(T_i_v));
        T_i_v.clear();
    }

    
    for(int i=0; i<7; i++){

        ms_out<<L[i]*(1e-3)<<" "<<T[i]<<" "<<eL_v[i]*(1e-3)<<" "<<eT[i]<<endl;
        ms_out_lin<<L[i]*(1e-3)<<" "<<T[i]*T[i]<<" "<<eL_v[i]*(1e-3)<<" "<<2*T[i]*eT[i]<<endl;
    }

    T.clear();
    L.clear();
    eT.clear();
    eL_v.clear();

}

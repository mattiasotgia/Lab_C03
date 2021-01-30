#include <cmath>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>

#include "TApplication.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLatex.h"
#include "TStyle.h"
#include "TAxis.h"
#include "TMath.h"


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

// -------------------------------------------------- statistic_plot_fit

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


//	!!	ATTENZIONE:	NON TOCCARE FUNZIONI SOPRA!
//	!!				IL PROGRAMMA FINISCE QUI!
//	!!				ULTERIORI FUNZIONI SONO DA IMPLEMENTARE SOTTO \/ \/
//	!!

void saveas(TCanvas* _c, string name_input, string MainTitle_text, double title_mar){

	gStyle->SetTextFont(42);
	TLatex data_presa_dati;
	data_presa_dati.SetTextSize(0.03);
	data_presa_dati.SetTextAlign(31);
	data_presa_dati.DrawLatexNDC(0.94, 0.91, "dati 2020/12/16, g = 9.8055 m/s^{2}");
	TLatex header;
	header.SetTextSize(0.04);
	header.DrawLatexNDC(title_mar, 0.91, MainTitle_text.c_str());

	// save plot figure in ./fig/ directory
	int slash=name_input.find_last_of("/");
	int dot=name_input.find_last_of(".");
	string filename="../fig/fig_"+name_input.substr(slash+1, dot-slash-1)+".pdf";
	_c->SaveAs(filename.c_str());
}

// Verifica della compatibilità per grandezze con errore ASSOLUTO
bool test_absErr(double g1, double g2, double eg1, double eg2, ofstream out){
	bool r;
	if ((abs(g1-g2)<(eg1+eg2))){
		out<<"COMPATIBILITA'"<<endl;
		r=true;
	}else{
		out<<"DISCREPANZA"<<endl;
		r=false;
	}
	return r;
}

// Verifica della compatibilità per grandezze con errore STATISTICO
// Entrambe le misure devono avere errore sistematico!
// Criterio del 3(sigma)
string test_statErr(double g1, double g2, double eg1, double eg2){
	string r;
	auto r_num=abs(g1-g2)-(3*sqrt((eg1*eg1)+(eg2*eg2)));
	if ((abs(g1-g2)<(3*sqrt((eg1*eg1)+(eg2*eg2))))){
		r="COMPATIBILITA'";
	}else{
		r="DISCREPANZA: scarto tra |g1-g2| e |eg1+eg2| = "+to_string(abs(r_num)) ;
	}
	return r;
}


// main function
int main(int argc, char* argv[])
{
	// initial setup
    TApplication app("app", 0, 0);
	/*	NON TOCCARE SOPRA:	setup iniziale, lancia applicazione root
	 */

   	//TCanvas mainPlot("mainPlot", "", 600, 500);	// IMPOSTARE w, h
    //mainPlot.SetMargin(0.08, 0.06, 0.10, 0.10); 	// Set canvas size and borders

	/////////////////////////////////////////////////////////// BEGIN MAIN FUNCTION
	/////////////////////////////////////////////////////////// BEGIN MAIN FUNCTION

	// ifstream /*<name>("../dati/<input_file_name>")*/
	// string in_f_main=argv[1];
	// string __input_main="../dati/"+in_f_main; // MODIFY!!
	string __input_main="../dati/dati_C03_201216_presa_dati1.txt";
	ifstream in_dati(__input_main.c_str());	// ------------------------- MAIN INPUT FILE
	
	// controllo parametri input
	if (!in_dati.good()){
		return 1;
	}
	

	// dichiarazione di oggetti standard
	// misc
	double x_i, y_i; // entrate per i vettori e per la media

	// angolo di uscita a v0
	double tetha0=0; // modify if necessary, otherwise it will not change
	
	// vettori per le misure ripetute
	vector<double> x_i_v;			// Vettore per calcolare la media (TEMP OBJ)
	vector<double> x_avg_SMALL_v; 	// VETTORE DI x MEDIE, posizione di caduta corpo -RICAVATE STATISTICAMENTE
	vector<double> x_BIG_v;			// Vettore di x del corpo grande (11-sima misurazione per ogni y)
	vector<double> ex_v;			// errore x statistico
	vector<double> y_i_v;			// VETTORE DI y originali, altezza del piano di caduta
	vector<double> ey_v;			// errore y [legato sia a e_y0 e a a sin(phi0)]


	vector<double> varianza_x_v;
	vector<double> std_adattata_x_v;


	double phi0=1; //angolo misurato di inclinazione del piano (default a 1, errore di misura dell'angolo della bolla)
	double phi_RAD=phi0*M_PI/(double)180;

	// costanti utilizzate
	const double g=9.8055; // COSTANTE espressa in m/s^2 <<---ATTENZIONE!: nota senza errore!
	
	// LEGGERE I VALORI

	// leggo valori di massa e misure pallina grande
	double M_BIG, eM_BIG;	// massa pallina grande 
	double Diam_BIG, eD_BIG;	//diametro e errore del solido GRANDE

	in_dati>>M_BIG>>eM_BIG>>Diam_BIG>>eD_BIG;

	M_BIG=M_BIG*(1e-3);
	eM_BIG=eM_BIG*(1e-3);
	Diam_BIG=Diam_BIG*(1e-3);
	eD_BIG=eD_BIG*(1e-3);


	// leggo valori di massa e misure pallina piccola
	double m_SMALL, em_SMALL; // massa pallina piccola
	double Diam_SMALL, eD_SMALL;	//diametro e errore del solido PICCOLO

	in_dati>>m_SMALL>>em_SMALL>>Diam_SMALL>>eD_SMALL;

	m_SMALL=m_SMALL*(1e-3);
	em_SMALL=em_SMALL*(1e-3);
	Diam_SMALL=Diam_SMALL*(1e-3);
	eD_SMALL=eD_SMALL*(1e-3);
	

	// leggo altezze
	// altezze scivolo
	double htop, e_htop;
	double hplate, e_hplate;

	in_dati>>htop>>e_htop>>hplate>>e_hplate;

	htop=htop*(1e-3);
	e_htop=e_htop*(1e-3);
	hplate=e_hplate*(1e-3);
	hplate=hplate*(1e-3);

	// evaluate v0;
	double v0;
	double delta_h=htop-hplate;
	double e_delta_h=e_htop+e_hplate; // Errore Assoluto
	
	// utilizziamo per comodità la formula che tiene conto solo di energia cinetica traslazionale, 
	// per fornire al fit una misura quantopiù approssimata di v0
	v0=sqrt(2*g*delta_h);


	double e_y0; 	// Errore su y_i (si suppone sia unico [errore assoluto dy]). 
					// ATTENZIONE!: BISOGNA POI CONSIDERARE PER OGNI PUNTO L'ERRORE 
					// DOVUTO ALLA POSSIBILE INCLINAZIONE DEL PIANO
	in_dati>>e_y0;

	e_y0=e_y0*(1e-3);

	// leggo i valori da y1 a y5, poi aggiungo al vettore
	for(int i=0; i<5; i++){
		in_dati>>y_i;
		y_i_v.push_back(y_i*(1e-3));
	}

	string br;
	for(int i=0; i<5; i++){
		in_dati>>br;
		for(int j=0; j<10; j++){
			in_dati>>x_i;
			x_i_v.push_back(x_i*(1e-3));
		}

		// ora ho un vettore x_i_v con i valori da 1 a 10 dei miei punti
		x_avg_SMALL_v.push_back(Media(x_i_v)); // Aggiungo i punti medi;
		ex_v.push_back(ErroreStandard(x_i_v)); // Aggiungo l'errore standard;
		varianza_x_v.push_back(Varianza(x_i_v));
		std_adattata_x_v.push_back(DevStandardAdattata(x_i_v));

		// aggiungo l'errore su y al vettore ey_v; lo converto in errore statistico considerando err_assoluto/sqrt(3)
		ey_v.push_back((e_y0+Media(x_i_v)*sin(phi_RAD))/sqrt(3));

		// rimuovo i dati da x_i_v, che è un oggetto temporaneo
		x_i_v.clear();
	}
	in_dati>>br;
	for(int i=0; i<5; i++){
		in_dati>>x_i;
		x_BIG_v.push_back(x_i*(1e-3));
	}

	TCanvas parabolic("parabolic", "", 1100, 500);	// IMPOSTARE w, h
    parabolic.SetMargin(0.12, 0.08, 0.12, 0.10); 	// Set canvas size and borders
	parabolic.Divide(2, 1);

	TGraphErrors _main_G_p;
	_main_G_p.GetXaxis()->SetTitle("X_{i} (m)");
	_main_G_p.GetXaxis()->SetTitleSize(0.05);
	_main_G_p.GetYaxis()->SetTitleOffset(0.85);
	_main_G_p.GetYaxis()->SetTitle("Altezza di caduta Y_{i} (m)");
	_main_G_p.GetYaxis()->SetTitleSize(0.05);

	// TGraphErrors _g_BIG;

	for(int i=0; i<x_avg_SMALL_v.size(); i++){

		// _g_BIG.SetPoint(i, x_BIG_v[i], y_i_v[i]);
		// _g_BIG.SetPointError(i, 1e-2, ey_v[i]);

		_main_G_p.SetPoint(i, x_avg_SMALL_v[i], y_i_v[i]);
		_main_G_p.SetPointError(i, ex_v[i], ey_v[i]);
	}

	double xmin=TMath::MinElement(_main_G_p.GetN(), _main_G_p.GetX());
	double xmax=TMath::MaxElement(_main_G_p.GetN(), _main_G_p.GetX());



	TGraphErrors _main_G_lineare;
	_main_G_lineare.GetXaxis()->SetTitle("X_{i} (m)");
	_main_G_lineare.GetXaxis()->SetTitleSize(0.05);
	_main_G_lineare.GetYaxis()->SetTitleOffset(0.85);
	_main_G_lineare.GetYaxis()->SetTitle("Linearizzazione Y_{i}/X_{i}");
	_main_G_lineare.GetYaxis()->SetTitleSize(0.05);


	vector<double> YsuX;
	vector<double> eYsuX;
	for(int i=0; i<x_avg_SMALL_v.size(); i++){

		YsuX.push_back(y_i_v[i]/x_avg_SMALL_v[i]);
		eYsuX.push_back(sqrt(pow((ey_v[i]/x_avg_SMALL_v[i]), 2) + pow(((y_i_v[i]*ex_v[i])/pow(x_avg_SMALL_v[i], 2)),2)));
	}

	for(int i=0; i<x_avg_SMALL_v.size(); i++){

		_main_G_lineare.SetPoint(i,  x_avg_SMALL_v[i], YsuX[i]);
		_main_G_lineare.SetPointError(i, ex_v[i], eYsuX[i]);
	}

	double xmin_lin=TMath::MinElement(_main_G_lineare.GetN(), _main_G_lineare.GetX());
	double xmax_lin=TMath::MaxElement(_main_G_lineare.GetN(), _main_G_lineare.GetX());


	// tentativo di fit
	TF1 f("f", "[2]*x+([0]/(2*[1]*[1]))*x*x", xmin, xmax);

	f.FixParameter(0, g);
	f.SetParameter(2, tan(phi_RAD));
	f.SetParameter(1, v0);
	f.SetLineColor(kRed);

	// TF1 fB("fB", "[2]*x+([0]/(2*[1]*[1]))*x*x", xmin, xmax);

	// fB.FixParameter(0, g);
	// fB.SetParameter(2, tan(phi_RAD));
	// fB.SetParameter(1, v0);
	// fB.SetLineColor(kBlack);


	// _g_BIG.GetXaxis()->SetTitle("X_{i} (m)");
	// _g_BIG.GetXaxis()->SetTitleSize(0.05);
	// _g_BIG.GetYaxis()->SetTitleOffset(0.85);
	// _g_BIG.GetYaxis()->SetTitle("Altezza di caduta Y_{i} (m)");
	// _g_BIG.GetYaxis()->SetTitleSize(0.05);

	// double BIG_xmax=TMath::MaxElement(_g_BIG.GetN(), _g_BIG.GetX());
	// double ymin=TMath::MinElement(_main_G_p.GetN(), _main_G_p.GetY());
	// double ymax=TMath::MaxElement(_main_G_p.GetN(), _main_G_p.GetY());
	// _g_BIG.SetLineColor(kBlack);
	// _g_BIG.Draw("ap");
	// _g_BIG.GetXaxis()->SetRangeUser(xmin-0.2*xmin, BIG_xmax+0.2*BIG_xmax);
	// _g_BIG.GetYaxis()->SetRangeUser(ymin-0.2*ymin, ymax+0.2*ymax);
	// _g_BIG.Draw("ap");
	// _g_BIG.Fit("fB");

	parabolic.cd(1);
	_main_G_p.Draw("ap");
	_main_G_p.Fit("f");


	gStyle->SetTextFont(42);
	TLatex data_presa_dati;
	data_presa_dati.SetTextSize(0.03);
	data_presa_dati.SetTextAlign(31);
	data_presa_dati.DrawLatexNDC(0.90, 0.91, "dati 2020/12/16, g = 9.8055 m/s^{2}");
	TLatex header;
	header.SetTextSize(0.035);
	header.DrawLatexNDC(0.10, 0.91, "#bf{Caduta libera PARABOLICA}");


	TF1 flineare("flineare", "[0]+[1]*x", xmin_lin, xmax_lin);

	auto n_pre_non_usare=g/(2*v0*v0);

	flineare.SetParameter(0, 0.0);				// k
	flineare.SetParameter(1, n_pre_non_usare);	// n


	parabolic.cd(2);
	_main_G_lineare.Draw("ap");
	_main_G_lineare.Fit("flineare");


	string chi2_s="#bf{#chi^{2}/ndf			"+to_string(flineare.GetChisquare())+"/"+to_string((int)flineare.GetNDF())+"}";
	gStyle->SetTextFont(42);
	TLatex chi2;
	chi2.SetTextSize(0.04);
	chi2.SetTextAlign(31);
	chi2.DrawLatexNDC(0.86, 0.15, chi2_s.c_str());

	cout<<"flineare(0) = "<<flineare.Eval(0)<<endl;


	gStyle->SetTextFont(42);
	TLatex data_presa_dati_1;
	data_presa_dati_1.SetTextSize(0.03);
	data_presa_dati_1.SetTextAlign(31);
	data_presa_dati_1.DrawLatexNDC(0.90, 0.91, "dati 2020/12/16, g = 9.8055 m/s^{2}");
	TLatex header_1;
	header_1.SetTextSize(0.035);
	header_1.DrawLatexNDC(0.10, 0.91, "#bf{Caduta libera LINEARIZZATA}");



	// save plot figure in ./fig/ directory
	int slash=__input_main.find_last_of("/");
	int dot=__input_main.find_last_of(".");
	string filename="../fig/fig_"+__input_main.substr(slash+1, dot-slash-1)+"_osse_lineare.pdf";
	parabolic.SaveAs(filename.c_str());


	double v0_T=sqrt(2*g*delta_h);
	double ev0_T=(abs(g/sqrt(2*g*delta_h))*e_delta_h)/sqrt(3);

	auto v0_R=sqrt((10/(double)7)*g*delta_h);
	auto ev0_R=(abs(g/sqrt((10/(double)7)*g*delta_h))*e_delta_h)/sqrt(3);

	auto n_fit=flineare.GetParameter(1);
	auto en_fit=flineare.GetParError(1);

	auto k=flineare.GetParameter(0);
	auto ek=flineare.GetParError(0);

	auto thet_fit=atan(k);
	auto e_thet_fit=((double)1/(double)1+pow(k, 2))*ek;

	auto v0_fit=sqrt(g/(2*n_fit*pow(cos(0), 2)));
	auto ev0_fit=sqrt(
		pow(
			((sqrt(g/(double)2)*pow(n_fit, -1.5)*en_fit)/(2*cos(0))), 2
		)+
		pow(
			((sqrt(g/(double)2)*pow(n_fit, -0.5)*sin(0)*e_thet_fit)/pow(cos(0), 2)), 2
		)
	);

	ofstream risultati("risultati_fit.txt");

	risultati<<"v0_T = "<<v0_T<<" +/- "<<ev0_T<<endl;
	risultati<<"v0_R = "<<v0_R<<" +/- "<<ev0_R<<endl<<endl;
	risultati<<"k = "<<k<<" +/- "<<ek<<endl;
	risultati<<"n = "<<n_fit<<" +/- "<<en_fit<<endl<<endl;
	risultati<<"theta_fit = "<<thet_fit<<" +/- "<<e_thet_fit<<endl;
	risultati<<"v0_fit = "<<v0_fit<<" +/- "<<ev0_fit<<endl<<endl;
	risultati<<"Confronto con v0_T e v0_fit:"<<endl;
	risultati<<test_statErr(v0_T, v0_fit, ev0_T, ev0_fit)<<endl;
	risultati<<"Confronto con v0_R e v0_fit:"<<endl;
	risultati<<test_statErr(v0_R, v0_fit,ev0_R, ev0_fit)<<endl<<endl;
	risultati<<"Contronto theta_fit e theta_0"<<endl;
	risultati<<test_statErr(thet_fit, 0, e_thet_fit, 0.01)<<endl;


	// saveas(&parabolic, __input_main, "#bf{Caduta libera OSSERVATA}", 0.12);

    app.Run();	///////////////////////////////////////////////////////////END OF MAIN FUNCTION
    return 0;	///////////////////////////////////////////////////////////END OF MAIN FUNCTION
}

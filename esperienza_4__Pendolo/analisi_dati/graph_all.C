#include <TApplication.h>
#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TMultiGraph.h>

#include <TAxis.h>
#include <string>
#include <iostream>
#include <cmath>
#include <ctime>
#include <TLatex.h>
#include <TStyle.h>
#include <TMath.h>

using namespace std;

void plotS1(TGraphErrors* _g, string data_set, bool lineare, string pdf_save){

	TCanvas c("c", "", 600, 500);
	c.SetMargin(0.16, 0.06, 0.12, 0.06);
	
	string formula;

	if(!lineare) formula="[2]+2*[1]*sqrt(x/[0])";
	else formula="[2]+[1]*[1]*4*(x/[0])";

	TF1 f_offset("fOFF", formula.c_str(), 0.95, 2.05);
	f_offset.SetParameter(0, 10);
	f_offset.FixParameter(1, M_PI);
	f_offset.SetParameter(2, 0);
	f_offset.SetLineColor(kRed);

	_g->SetTitle("");
	_g->GetXaxis()->SetTitle("Lunghezza L_{i} (m)");

	string periodo;
	if(lineare){
		periodo="Periodo quadrato T_{i}^{2} (s^{2})";
	}else{
		periodo="Periodo T_{i} (s)";
	}

	_g->GetYaxis()->SetTitle(periodo.c_str());
	_g->GetXaxis()->SetTitleOffset(0.85);
	_g->GetXaxis()->SetTitleSize(0.06);
	_g->GetYaxis()->SetTitleSize(0.06);
	_g->GetXaxis()->CenterTitle();
	_g->GetYaxis()->CenterTitle();
	
	_g->GetXaxis()->SetLimits(0.95, 2.05);

	cout<<"------------------"<<endl;
	cout<<"Analisi set dati "<<data_set<<endl;

	_g->Fit("fOFF");
	_g->Draw("ap");


	gStyle->SetTextFont(42);

    	string h="#bf{Dati "+data_set+"}";
	TLatex header;
	header.SetTextSize(0.065);
	header.DrawLatexNDC(0.25, 0.85, h.c_str());

	string g_total;
	if(lineare){
		g_total="g = "+to_string(f_offset.GetParameter(0))
			+" #pm "+to_string(f_offset.GetParError(0))+" m/s^{2}";
	}else{
		g_total="";
	}

	TLatex par;
	par.SetTextSize(0.04);
	par.DrawLatexNDC(0.50, 0.20, g_total.c_str());
	

	TLatex stat_l;
	stat_l.SetTextSize(0.04);
	
	string stat_s;
	if(lineare && data_set!="C"){
		stat_s="#bf{#splitline{#chi^{2}/ndf (prob.)}{"+to_string(f_offset.GetChisquare())+"/"
		+to_string(f_offset.GetNDF())+" ("+to_string(f_offset.GetProb()*1e2)+"#times10^{-2})}}";
	}else if(lineare && data_set=="C"){
		stat_s="#bf{#splitline{#chi^{2}/ndf (prob.)}{"+to_string(f_offset.GetChisquare())+"/"
		+to_string(f_offset.GetNDF())+" ("+to_string(f_offset.GetProb()*1e3)+"#times10^{-3})}}";
	}else{
		stat_s="";
	}
	stat_l.DrawLatexNDC(0.50, 0.28, stat_s.c_str());

	c.SaveAs(pdf_save.c_str());

}

void graph_all(){

	time_t rawtime;
  	struct tm * timeinfo;

  	time (&rawtime);
  	timeinfo = localtime(&rawtime);
  	printf ("Log date/time: %s", asctime (timeinfo));

	gStyle->SetFrameLineWidth(0);

	// Importing all graph data;
	TGraphErrors* gA = new TGraphErrors("../dati/ed_dati_modifica.dat");
	TGraphErrors* gB = new TGraphErrors("../dati/rp_dati.dat");
	TGraphErrors* gC = new TGraphErrors("../dati/ms_dati.dat");

	TGraphErrors* gA_lin = new TGraphErrors("../dati/ed_dati_lin_modifica.dat");
	TGraphErrors* gB_lin = new TGraphErrors("../dati/rp_dati_lin.dat");
	TGraphErrors* gC_lin = new TGraphErrors("../dati/ms_dati_lin.dat");

	cout<<"--------------------------"<<endl;
	cout<<"Analisi dati NON-LINEARIZZATI:"<<endl;

	plotS1(gA, "A", 0, "dati_a_nonlin.pdf");
	plotS1(gB, "B", 0, "dati_b_nonlin.pdf");
	plotS1(gC, "C", 0, "dati_c_nonlin.pdf");

	cout<<"--------------------------"<<endl;
	cout<<"Analisi dati LINEARIZZATI:"<<endl;

	plotS1(gA_lin, "A", 1, "dati_a_lin.pdf");
	plotS1(gB_lin, "B", 1, "dati_b_lin.pdf");
	plotS1(gC_lin, "C", 1, "dati_c_lin.pdf");
}

#include <TApplication.h>
#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TMultiGraph.h>

#include <TAxis.h>
#include <string>
#include <cmath>
#include <TLatex.h>
#include <TStyle.h>
#include <TMath.h>


{
	
	gStyle->SetFrameLineWidth(0);

	TGraphErrors* gA = new TGraphErrors("../dati/ed_dati_modifica.dat");
	TGraphErrors* gB = new TGraphErrors("../dati/rp_dati.dat");
	TGraphErrors* gC = new TGraphErrors("../dati/ms_dati.dat");

    	TMultiGraph* gTOT=new TMultiGraph;
	gTOT->Add(gA);
	gTOT->Add(gB); // maybe remove B data set, off data...!!!!
	gTOT->Add(gC);
	gTOT->SetName("g");

	TCanvas c("c", "", 600, 500);
	c.SetMargin(0.16, 0.06, 0.12, 0.06);
	string formula_o="[2]+2*[1]*sqrt(x/[0])";
	// string formula_no="2*[1]*sqrt(x/[0])";
	

	TF1 f_offset("fOFF", formula_o.c_str(), 0.95, 2.05);
	f_offset.SetParameter(0, 10);
	f_offset.FixParameter(1, M_PI);
	f_offset.SetParameter(2, 0);
	f_offset.SetLineColor(kGreen+1);

	gTOT->SetTitle("");
	gTOT->GetXaxis()->SetTitle("Lunghezza L_{i} (m)");
	gTOT->GetYaxis()->SetTitle("Periodo T_{i} (s)");

	gTOT->GetXaxis()->SetTitleOffset(0.85);
	gTOT->GetXaxis()->SetTitleSize(0.06);
	gTOT->GetYaxis()->SetTitleSize(0.06);
	gTOT->GetXaxis()->CenterTitle();
	gTOT->GetYaxis()->CenterTitle();
	gTOT->GetXaxis()->SetLimits(0.95, 2.05);

	gTOT->Fit("fOFF");
	gTOT->Draw("ap");


	gStyle->SetTextFont(42);

	string g_total="g = "+to_string(f_offset.GetParameter(0))+" #pm "
		+to_string(f_offset.GetParError(0))+" m/s^{2}";

	string stat_s="#bf{#splitline{#chi^{2}/ndf (prob.)}{"
		+to_string(f_offset.GetChisquare())+"/"
		+to_string(f_offset.GetNDF())
		+" ("+to_string(f_offset.GetProb()*1e13)+"#times10^{-13})}}";

	TLatex stat_l;
	stat_l.SetTextSize(0.04);
	stat_l.DrawLatexNDC(0.50, 0.28, stat_s.c_str());


	TLatex header;
	header.SetTextSize(0.065);
        header.DrawLatexNDC(0.25, 0.85, "#bf{Dati A+B+C}"); 
	// MODIFY data set eventually (B is not good data!)
	// header.DrawLatexNDC(0.25, 0.85, "#bf{Dati A+C}");

	TLatex par;
	par.SetTextSize(0.04);
	par.DrawLatexNDC(0.5, 0.2, g_total.c_str());


	c.SaveAs("multigraph_set_final_v2.pdf");
	// c.SaveAs("multigraph_set_final_v2_AC.pdf");
}

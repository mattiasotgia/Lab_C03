#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include <TCanvas.h>
#include <TPad.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TStyle.h>
#include <TVector.h>
#include <TAxis.h>
#include <TLatex.h>
// #include <TH1D.h>

// #include "../../stat/stat.h"


using namespace std;

vector<string> plot_Err(vector<double> _x_data, vector<double> _xerr_data, 
            vector<double> _y_data, vector<double> _yerr_data, 
            double params[], int par_N, string formula = "",
            string xlabel = "", string ylabel = "",
            string title = "", string pdf_save = "plot",
            int removed_point = 2, double ymin = 0.25, double ymax = 0.75)
{
    vector<string> params_out;

    gStyle->SetTextFont(42);

    TCanvas c("c", "", 600, 500);
    c.SetMargin(0.16, 0.06, 0.12, 0.06);
    // c.SetGrid();

    TGraphErrors _g;

    for (int i = 0; i<_x_data.size(); i++){
        if(i != removed_point-1){
            _g.SetPoint(i, _x_data[i], _y_data[i]);
            _g.SetPointError(i, _xerr_data[i], _yerr_data[i]);
        }
    }

    TF1 _f("f1", formula.c_str());
    
    for (int i = 0; i<par_N; i++){
        _f.SetParameter(i, params[i]);
    }

    _g.SetTitle("");
    _g.GetXaxis()->SetTitle(xlabel.c_str());
    _g.GetYaxis()->SetTitle(ylabel.c_str());
    _g.GetXaxis()->SetTitleOffset(0.85);
    _g.GetXaxis()->SetTitleSize(0.06);
    _g.GetYaxis()->SetTitleSize(0.06);
    _g.GetXaxis()->CenterTitle();
    _g.GetYaxis()->CenterTitle();

    _g.SetMinimum(ymin);
    _g.SetMaximum(ymax);
    _g.GetXaxis()->SetLimits(0.15, 1.2);

    _g.Fit("f1");
    _g.Draw("ap");

    gStyle->SetTextFont(42);

    string stat_str="#splitline{#chi^{2}/ndf (prob.)}{"
	    +to_string(_f.GetChisquare())+"/"
	    +to_string(_f.GetNDF())
	    +" ("+to_string(_f.GetProb())+")}";
    
    cout << "prob" << _f.GetProb() << endl;

	TLatex stat_l;
	    stat_l.SetTextSize(0.04);
	    stat_l.DrawLatexNDC(0.50, 0.28, stat_str.c_str());
    
    TLatex header;
	header.SetTextSize(0.065);
    header.DrawLatexNDC(0.25, 0.85, ("#bf{" + title + "}").c_str());

    c.SaveAs(("../fig/" + pdf_save + ".pdf").c_str());

    for(int i = 0; i<par_N; i++){
        params_out.push_back(to_string(_f.GetParameter(i)) + " " + to_string(_f.GetParError(i)));
    }

    return params_out;
}

void analisi_dettaglio_preliminare(){

    gStyle->SetFrameLineWidth(0);

    ifstream data_osc("../misc/p_oscillazioni_computed_txt.txt");
    ifstream data_sts("../misc/l_elong_computed_txt.txt");

    string line;
    
    vector<double> _M, _err_M, _T, _err_T, _T2, _err_T2, _L, _err_L;
    const double g = 9.8056; const double err_g = 0.0001;

    while(getline(data_osc, line)){

        istringstream issll(line);
        double M, err_M, T, err_T, T2, err_T2;
        issll >> M >> err_M >> T >> err_T >> T2 >> err_T2;
        _M.push_back(M);
        _err_M.push_back(err_M);
        _T.push_back(T);
        _err_T.push_back(err_T);
        _T2.push_back(T2);
        _err_T2.push_back(err_T2);

    }

    while(getline(data_sts, line)){

        istringstream issll(line);
        double M, err_M, L, err_L;
        issll >> M >> err_M >> L >> err_L;
        _L.push_back(L);
        _err_L.push_back(err_L);
    }

    ofstream data_output("../misc/computed_output_analisys.csv");
    data_output << "method, k, err_k, spring_m, err_spring_m, offset, err_offset" << endl;

    // --------------------------------------------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------------------------------

    string formula_dyn = "2*pi*sqrt((x+[1])/[0])";
    double params_dyn[] = {88, 0.08};

    data_output << "dynamic, " << flush;

    vector<string> results_dyn = plot_Err(_M, _err_M, _T, _err_T, params_dyn, 2, formula_dyn, 
                                "Massa (kg)", "Periodo (s)", "#splitline{Dati dinamica}{(non linearizzati)}", "analisi_15_03_dyn");

    istringstream rr_dyn_p0(results_dyn[0]);
    double dyn_p0, err_dyn_p0;
    rr_dyn_p0 >> dyn_p0 >> err_dyn_p0;

    data_output << dyn_p0 << ", " << err_dyn_p0 << ", " << flush;

    istringstream rr_dyn_p1(results_dyn[1]);
    double dyn_p1, err_dyn_p1;
    rr_dyn_p1 >> dyn_p1 >> err_dyn_p1;

    data_output << dyn_p1*3 << ", " << err_dyn_p1*3 << ", " << flush;

    data_output << "-, - "<< endl;

    // --------------------------------------------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------------------------------    

    string formula_dyn_lin = "[0]*x+[1]";
    double params_dyn_lin[] = {1, 1};

    data_output << "dynamic_lin, " << flush;
    
    vector<string> results_dyn_lin = plot_Err(_M, _err_M, _T2, _err_T2, params_dyn_lin, 2, formula_dyn_lin, 
                                    "Massa (kg)", "Periodo^{2} (s^{2})", "#splitline{Dati dinamica}{(linearizzati)}", "analisi_15_03_dyn__lin", 
                                    2, 0.0, 0.55);
    
    istringstream rr_dyn_lin_p0(results_dyn_lin[0]);
    double dyn_lin_p0, err_dyn_lin_p0;
    rr_dyn_lin_p0 >> dyn_lin_p0 >> err_dyn_lin_p0;

    data_output << (4*M_PI*M_PI)/dyn_lin_p0 << ", " << (4*M_PI*M_PI*err_dyn_lin_p0)/pow(dyn_lin_p0, 2) << ", " << flush;

    istringstream rr_dyn_lin_p1(results_dyn_lin[1]);
    double dyn_lin_p1, err_dyn_lin_p1;
    rr_dyn_lin_p1 >> dyn_lin_p1 >> err_dyn_lin_p1;

    data_output << (3*dyn_lin_p1)/dyn_lin_p0 << ", " 
                << sqrt(pow((3*err_dyn_lin_p1)/dyn_lin_p0, 2) + pow((3*dyn_lin_p1*err_dyn_lin_p0)/pow(dyn_lin_p0,2), 2))
                << ", " << flush;

    data_output << "-, - "<< endl;

    // --------------------------------------------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------------------------------
    
    string formula_sts = "[0]*x+[1]";
    double params_sts[] = {1, 0};

    data_output << "static, " << flush;

    vector<string> results_sts = plot_Err(_M, _err_M, _L, _err_L, params_sts, 2, formula_sts, 
                                "Massa (kg)", "Elongazione (m)", "Dati statica", "analisi_15_03_sts", 
                                    2, 0.03, 0.15);
    
    istringstream rr_sts_p0(results_sts[0]);
    double sts_p0, err_sts_p0;
    rr_sts_p0 >> sts_p0 >> err_sts_p0;

    data_output << g/sts_p0 << ", " 
                << sqrt(pow(err_g/sts_p0, 2) + pow(g*err_sts_p0/(sts_p0*sts_p0), 2)) 
                << ", " << flush;

    data_output << "-, -, " << flush;

    istringstream rr_sts_p1(results_sts[1]);
    double sts_p1, err_sts_p1;
    rr_sts_p1 >> sts_p1 >> err_sts_p1;

    data_output << sts_p1 << ", " << err_sts_p1 << endl;


}


int main(){
    analisi_dettaglio_preliminare();
    return 0;
}
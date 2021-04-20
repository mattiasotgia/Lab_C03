#include <vector>
#include <cmath>
#include <iostream>
#include <string>

#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TStyle.h>
#include <TAxis.h>
#include <TMath.h>
#include <TLatex.h>
#include <TLegend.h>

// definiamo valori costanti utili nel programma
#define G_T     9.8056      // {m/s2}
#define ERR_G_T 0.0001      // {m/s2}
#define K_B     1.3806e-23  // {J/K}
#define ALPHA   2           // {a. u.}
#define L_0     0.029       // {m}
#define ERR_L_0 0.001       // {m}
#define V_0_0   0.857e-3    // {V}
#define V_0_T   0.845e-3    // {V}

// pol1 = a0 + a1*x           ****

void print_mmsg(std::string mmsg){
    std::cout << std::endl 
        << " **********" << std::endl
        << "    " << mmsg << std::endl
        << " **********" << std::endl
        << std::endl;
}

void print_stat(TF1* _f){
    std::cout << std::endl
        << "** " << "CHI2 / NDF ( PROB. ) " 
        << _f->GetChisquare() << " / " << _f->GetNDF() << " ( " << _f->GetProb() << " )"
        << std::endl << std::endl;
}

double get_F(Double_t* params, double V_0, bool idrostatic = false){
    if(idrostatic){
        return G_T * (V_0 - V_0_0) / params[1];
    }
    return G_T * (V_0 - params[0]) / params[1];
}

double get_Ferr(Double_t* params, const Double_t* params_err, double V_0, double V_0err){
    double ddV_sqr = pow(G_T / params[1], 2);
    double dda_sqr = pow(G_T * (params[0] - V_0) / pow(params[1], 2), 2);
    double ddb_sqr = pow(-G_T / params[1], 2);
    double ddg_sqr = pow((V_0 - params[0]) / params[1], 2);

    return sqrt(
        (ddV_sqr * V_0err * V_0err) + 
        (dda_sqr * params_err[1] * params_err[1]) + 
        (ddb_sqr * params_err[0] * params_err[0]) + 
        (ddg_sqr * ERR_G_T * ERR_G_T)
    );
}

double det_N(Double_t* params){
    return params[1] * L_0 / (K_B * (ALPHA - pow(ALPHA, -2)));
}

double get_Nerr(Double_t* params, const Double_t* params_err){
    double ddl_sqr = pow(params[1] / (K_B * (ALPHA - pow(ALPHA, -2))), 2);
    double dda_sqr = pow(L_0 / (K_B * (ALPHA - pow(ALPHA, -2))), 2);

    return sqrt(
        (ddl_sqr * ERR_L_0 * ERR_L_0) + 
        (dda_sqr * params_err[1] * params_err[1])
    );
}

void analisi_dati(bool header = true){

    gStyle->SetFrameLineWidth(0);

    gStyle->SetTextFont(42);

    TCanvas* c1 = new TCanvas("c1", "", 600, 500);
    c1->SetMargin(0.16, 0.06, 0.12, 0.06);
    // c1.SetGrid();

    TGraphErrors* t1 = new TGraphErrors("../dati/taratura.txt");

    t1->SetTitle("");
    t1->GetXaxis()->SetTitle("Massa [kg]");
    t1->GetYaxis()->SetTitle("Tensione [mV]");
    t1->GetXaxis()->SetTitleOffset(0.85);
    t1->GetXaxis()->SetTitleSize(0.06);
    t1->GetYaxis()->SetTitleSize(0.06);
    t1->GetXaxis()->CenterTitle();
    t1->GetYaxis()->CenterTitle();

    TF1* f1 = new TF1("f1", "pol1");

    print_mmsg("RUN 0, PROCESSING 15 POINTS...");
    
    std::string f_Opt = "F";

    t1->Fit("f1", f_Opt.c_str());
    t1->Draw("ap");
    auto chi2 = f1->GetChisquare();
    auto prob_chi2 = f1->GetProb();

    print_stat(f1);

    c1->SaveAs("../fig/kater_plot_all.pdf");
    c1->Draw("ap");
//                               \/ chi2>(1.5 * (f1->GetNDF())) || (i=0)
    for (int i = t1->GetN() - 1; i>0; i--){
        print_mmsg("RUN "+ std::to_string(15-i) + ", PROCESSING " + std::to_string(i) + " POINTS...");
        t1->RemovePoint(i);
        t1->Fit("f1", f_Opt.c_str());
        chi2 = f1->GetChisquare();
        prob_chi2 = f1->GetProb();
        print_stat(f1);
        c1->SaveAs(("../fig/kater_plot_" + std::to_string(i) + "_points.pdf").c_str());
        c1->Draw("ap");
        // if(prob_chi2<0.05){
        //     break;
        // }
    }


    return;
}

#include<vector>
#include<cmath>
#include<iostream>
#include<fstream>
#include<string>

#include<TCanvas.h>
#include<TGraphErrors.h>
#include<TF1.h>
#include<TStyle.h>
#include<TAxis.h>
#include<TMath.h>
#include<TLatex.h>
#include<TLegend.h>

// definiamo valori costanti utili nel programma
#define G_T         9.8056                 // {m/s2}
#define ERR_G_T     0.0001                 // {m/s2}
#define K_B         1.3806e-23             // {J/K}
#define ALPHA       2                      // {a. u.}
#define L_0         0.029                  // {m}
#define ERR_L_0     0.001                  // {m}
#define V_0_0       0.857e-3               // {V}
#define ERR_V_0_0   0.001/(2*std::sqrt(3)) // {V}
#define V_0_T       0.845e-3               // {V}

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

double get_F(Double_t* params, double V_0_misurato){
    return G_T * (V_0_misurato - V_0_0) / params[1];
}

double get_Ferr(Double_t* params, const Double_t* params_err, double V_0, double V_0err){
    double ddV_sqr = pow(G_T / params[1], 2);
    double dda_sqr = pow(G_T * (V_0_0 - V_0) / pow(params[1], 2), 2);
    double ddV00_sqr = pow(-G_T / params[1], 2);
    // double ddg_sqr = pow((V_0 - V_0_0) / params[1], 2);

    return sqrt(
        (ddV_sqr * V_0err * V_0err) + 
        (dda_sqr * params_err[1] * params_err[1]) + 
        (ddV00_sqr * ERR_V_0_0 * ERR_V_0_0)
        // (ddg_sqr * ERR_G_T * ERR_G_T)
    );
}

double get_N(Double_t* params){
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

std::string compatible(double G1, double errG1,
                       double G2, double errG2){
    double abs_values = abs(G2-G1);
    double err_abs_val = 3*sqrt(pow(errG1, 2) + pow(errG2, 2));
    if(abs_values<err_abs_val){
        return "COMPATIBLE";
    }
    return "NON-COMPATIBLE";
}

double max_to_stat(double value){
    return value/(std::sqrt(3));
}

void analisi_dati(){

    // gStyle->SetFrameLineWidth(0);

    gStyle->SetTextFont(43);


    TCanvas* c1 = new TCanvas("c1", "", 600, 500);
    c1->SetMargin(0.16, 0.06, 0.12, 0.06);
    // c1.SetGrid();
    gStyle->SetTextFont(63);   

    TGraphErrors* t1 = new TGraphErrors("../dati/taratura.txt");

    t1->SetTitle("");
    t1->GetYaxis()->SetTitle("Tensione elettrica [V]");
    t1->GetYaxis()->SetTitleOffset(1);
    t1->GetYaxis()->SetTitleFont(43);
    t1->GetYaxis()->SetTitleSize(28);
    t1->GetYaxis()->SetLabelFont(43);
    t1->GetYaxis()->SetLabelSize(12);
    t1->GetYaxis()->CenterTitle();

    TF1* F1 = new TF1("F1", "pol1");

    print_mmsg("RUN 0, PROCESSING 15 POINTS...");
    
    std::string f_Opt = "E";

    TPad* p1 = new TPad("", "", 0.0, 0.3, 1.0, 1.0);
    TPad* p2 = new TPad("", "", 0.0, 0.0, 1.0, 0.3); 
    p1->SetMargin(0.14, 0.06, 0.0, 0.06);
    p1->Draw();
    p2->SetMargin(0.14, 0.06, 0.4, 0.0);
    p2->Draw();

    p1->cd();
    t1->Fit("F1", f_Opt.c_str());
    t1->Draw("ap");
    auto chi2 = F1->GetChisquare();
    auto prob_chi2 = F1->GetProb();

    std::string ss_1="#chi^{2}/ndf (prob.) = "
        +std::to_string(F1->GetChisquare())+"/"
        +std::to_string(F1->GetNDF())
        +" ("+std::to_string(F1->GetProb())+")";
    TLatex sl_1;
    sl_1.SetTextSize(0.035);
    sl_1.SetTextColor(kBlack);
    sl_1.DrawLatexNDC(0.50, 0.28, ss_1.c_str());
    
    TLatex header;
    header.SetTextSize(0.055);
    header.DrawLatexNDC(0.25, 0.85, "#bf{Dati dalla tabella 2}");

    print_stat(F1);
    print_mmsg(compatible(F1->GetParameter(0), F1->GetParError(0), V_0_T, 0.001e-3/(2*std::sqrt(3))));

    p2->cd();
    TGraphErrors* rg = new TGraphErrors();
    for(int i=0; i<t1->GetN(); i++){
        rg->SetPoint(i, t1->GetX()[i], (t1->GetY()[i] - F1->Eval(t1->GetX()[i])));
        rg->SetPointError(i, t1->GetEX()[i], t1->GetEY()[i]);
    }

    rg->GetXaxis()->SetTitle("Massa [kg]");
    rg->GetXaxis()->SetTitleOffset(2);
    rg->GetXaxis()->SetTitleFont(43);
    rg->GetXaxis()->SetTitleSize(28);

    rg->GetYaxis()->SetLabelFont(43);
    rg->GetYaxis()->SetLabelSize(12);
    rg->GetXaxis()->SetLabelFont(43);
    rg->GetXaxis()->SetLabelSize(12);
    rg->GetXaxis()->CenterTitle();

    rg->Draw("ap");

    TF1* rf0 = new TF1("rf0", "0"); rf0->Draw("same");

    c1->SaveAs("../fig/tar_plot.pdf");
    c1->Draw();

    // for (int i = t1->GetN() - 1; i>11; i--){
    //     print_mmsg("RUN "+ std::to_string(15-i) + ", PROCESSING " + std::to_string(i) + " POINTS...");
    //     t1->RemovePoint(i);
    //     t1->Fit("F1", f_Opt.c_str());
    //     chi2 = F1->GetChisquare();
    //     prob_chi2 = F1->GetProb();
    //     print_stat(F1);
    //     print_mmsg(compatible(F1->GetParameter(0), F1->GetParError(0), V_0_T, 0.001e-3/(2*std::sqrt(3))));
    //     // c1->SaveAs(("../fig/kater_plot_" + std::to_string(i) + "_points.pdf").c_str());
    //     c1->Draw();
    //     // if(prob_chi2<0.05){
    //     //     break;
    //     // }
    // }

    // **** SECONDA PARTE ****

    Double_t* p_taratura = F1->GetParameters();
    const Double_t* p_taratura_err = F1->GetParErrors();

    std::ifstream fileGJ_Tdown("../dati/effetto_G_J_Tdown.txt");
    std::ifstream fileGJ_Tup("../dati/effetto_G_J_Tup.txt");

    double v30, v30err, v1, v1err, v2, v2err, v3, v3err, T, Terr;

    TCanvas* c2 = new TCanvas("c2", "", 600, 500);
    c2->SetMargin(0.16, 0.06, 0.12, 0.06);

    TGraphErrors* g30 = new TGraphErrors(); g30->SetMarkerColor(kOrange); g30->SetLineColor(kOrange); g30->SetMarkerStyle(1); g30->SetName("g30");
    TGraphErrors* g1 = new TGraphErrors(); g1->SetMarkerColor(kRed); g1->SetLineColor(kRed); g1->SetMarkerStyle(2); g1->SetName("g1");
    TGraphErrors* g2 = new TGraphErrors(); g2->SetMarkerColor(kGreen); g2->SetLineColor(kGreen); g2->SetMarkerStyle(3); g2->SetName("g2");
    TGraphErrors* g3 = new TGraphErrors(); g3->SetMarkerColor(kAzure); g3->SetLineColor(kAzure); g3->SetMarkerStyle(4); g3->SetName("g3");

    TGraphErrors* g30_up = new TGraphErrors(); g30_up->SetMarkerColor(kOrange+1); g30_up->SetLineColor(kOrange+1); g30_up->SetMarkerStyle(1); g30_up->SetName("g30_up");
    TGraphErrors* g1_up = new TGraphErrors(); g1_up->SetMarkerColor(kRed+1); g1_up->SetLineColor(kRed+1); g1_up->SetMarkerStyle(2); g1_up->SetName("g1_up");
    TGraphErrors* g2_up = new TGraphErrors(); g2_up->SetMarkerColor(kGreen+1); g2_up->SetLineColor(kGreen+1); g2_up->SetMarkerStyle(3); g2_up->SetName("g2_up");
    TGraphErrors* g3_up = new TGraphErrors(); g3_up->SetMarkerColor(kAzure+1); g3_up->SetLineColor(kAzure+1); g3_up->SetMarkerStyle(4); g3_up->SetName("g3_up");

    g30->SetTitle("");
    g30->GetXaxis()->SetTitle("Temperatura [K]");
    g30->GetYaxis()->SetTitle("Tensione [N]");
    g30->GetXaxis()->SetTitleOffset(0.85);
    g30->GetXaxis()->SetTitleSize(0.06);
    g30->GetYaxis()->SetTitleSize(0.06);
    g30->GetXaxis()->CenterTitle();
    g30->GetYaxis()->CenterTitle();

    for(int i = 0; fileGJ_Tdown>>v30>>v30err>>v1>>v1err>>v2>>v2err>>v3>>v3err>>T>>Terr; i++){
        g30->SetPoint(i, T, get_F(p_taratura, v30));
        g30->SetPointError(i, max_to_stat(Terr), get_Ferr(p_taratura, p_taratura_err, v30, max_to_stat(v30err/2)));
        g1->SetPoint(i, T, get_F(p_taratura, v1));
        g1->SetPointError(i, max_to_stat(Terr), get_Ferr(p_taratura, p_taratura_err, v1, max_to_stat(v1err/2)));
        g2->SetPoint(i, T, get_F(p_taratura, v2));
        g2->SetPointError(i, max_to_stat(Terr), get_Ferr(p_taratura, p_taratura_err, v2, max_to_stat(v2err/2)));
        g3->SetPoint(i, T, get_F(p_taratura, v3));
        g3->SetPointError(i, max_to_stat(Terr), get_Ferr(p_taratura, p_taratura_err, v3, max_to_stat(v3err/2)));
    }

    for(int i = 0; fileGJ_Tup>>v30>>v30err>>v1>>v1err>>v2>>v2err>>v3>>v3err>>T>>Terr; i++){
        g30_up->SetPoint(i, T, get_F(p_taratura, v30));
        g30_up->SetPointError(i, max_to_stat(Terr), get_Ferr(p_taratura, p_taratura_err, v30, max_to_stat(v30err/2)));
        g1_up->SetPoint(i, T, get_F(p_taratura, v1));
        g1_up->SetPointError(i, max_to_stat(Terr), get_Ferr(p_taratura, p_taratura_err, v1, max_to_stat(v1err/2)));
        g2_up->SetPoint(i, T, get_F(p_taratura, v2));
        g2_up->SetPointError(i, max_to_stat(Terr), get_Ferr(p_taratura, p_taratura_err, v2, max_to_stat(v2err/2)));
        g3_up->SetPoint(i, T, get_F(p_taratura, v3));
        g3_up->SetPointError(i, max_to_stat(Terr), get_Ferr(p_taratura, p_taratura_err, v3, max_to_stat(v3err/2)));
    }

    TF1* f30 = new TF1("f30", "pol1"); f30->SetLineColor(kOrange);
    TF1*  f1 = new TF1("f1", "pol1"); f1->SetLineColor(kRed);
    TF1*  f2 = new TF1("f2", "pol1"); f2->SetLineColor(kGreen);
    TF1*  f3 = new TF1("f3", "pol1"); f3->SetLineColor(kAzure);

    TF1* f30_up = new TF1("f30_up", "pol1"); f30_up->SetLineColor(kOrange+1);
    TF1*  f1_up = new TF1("f1_up", "pol1"); f1_up->SetLineColor(kRed+1);
    TF1*  f2_up = new TF1("f2_up", "pol1"); f2_up->SetLineColor(kGreen+1);
    TF1*  f3_up = new TF1("f3_up", "pol1"); f3_up->SetLineColor(kAzure+1);

    // *** TEMPERATURE A SCENDERE
    print_mmsg("READING FROM INPUT ../dati/effetto_G_J_Tdown.txt");
    print_mmsg("PROCESSING G30...");
    g30->Fit("f30", f_Opt.c_str());
    print_stat(f30);

    print_mmsg("PROCESSING G1...");
    g1->Fit("f1", f_Opt.c_str());
    print_stat(f1);

    print_mmsg("PROCESSING G2...");
    g2->Fit("f2", f_Opt.c_str());
    print_stat(f2);

    print_mmsg("PROCESSING G3...");
    g3->Fit("f3", f_Opt.c_str());
    print_stat(f3);

    g30->Draw("ap");
    g1->Draw("p");
    g2->Draw("p");
    g3->Draw("p");

    print_mmsg("RISULTATI PER TEMPERATURE A SCENDERE PER N");

    std::cout << "N (da V_0 a 30 s)  = " << get_N(f30->GetParameters()) << " +/- " << get_Nerr(f30->GetParameters(), f30->GetParErrors()) << std::endl;
    std::cout << "N (da V_0 a 1 min) = " << get_N(f1->GetParameters()) << " +/- " << get_Nerr(f1->GetParameters(), f1->GetParErrors()) << std::endl;
    std::cout << "N (da V_0 a 2 min) = " << get_N(f2->GetParameters()) << " +/- " << get_Nerr(f2->GetParameters(), f2->GetParErrors()) << std::endl;
    std::cout << "N (da V_0 a 3 min) = " << get_N(f3->GetParameters()) << " +/- " << get_Nerr(f3->GetParameters(), f3->GetParErrors()) << std::endl;

    // *** TEMPERATURE A SALIRE

    print_mmsg("READING FROM INPUT ../dati/effetto_G_J_Tup.txt");
    print_mmsg("PROCESSING G30_UP...");
    g30_up->Fit("f30_up", f_Opt.c_str());
    print_stat(f30_up);

    print_mmsg("PROCESSING G1_UP...");
    g1_up->Fit("f1_up", f_Opt.c_str());
    print_stat(f1_up);

    print_mmsg("PROCESSING G2_UP...");
    g2_up->Fit("f2_up", f_Opt.c_str());
    print_stat(f2_up);

    print_mmsg("PROCESSING G3_UP...");
    g3_up->Fit("f3_up", f_Opt.c_str());
    print_stat(f3_up);

    g30_up->Draw("p");
    g1_up->Draw("p");
    g2_up->Draw("p");
    g3_up->Draw("p");

    print_mmsg("RISULTATI PER TEMPERATURE A SALIRE PER N");

    std::cout << "N (da V_0 a 30 s)  = " << get_N(f30_up->GetParameters()) << " +/- " << get_Nerr(f30_up->GetParameters(), f30_up->GetParErrors()) << std::endl;
    std::cout << "N (da V_0 a 1 min) = " << get_N(f1_up->GetParameters()) << " +/- " << get_Nerr(f1_up->GetParameters(), f1_up->GetParErrors()) << std::endl;
    std::cout << "N (da V_0 a 2 min) = " << get_N(f2_up->GetParameters()) << " +/- " << get_Nerr(f2_up->GetParameters(), f2_up->GetParErrors()) << std::endl;
    std::cout << "N (da V_0 a 3 min) = " << get_N(f3_up->GetParameters()) << " +/- " << get_Nerr(f3_up->GetParameters(), f3_up->GetParErrors()) << std::endl;

    
    TLatex header2;
    header2.SetTextSize(0.055);
    header2.DrawLatexNDC(0.2, 0.85, "#bf{Dati dalle tabelle 3 e 4}");

    TLegend* leg_down = new TLegend(0.2, 0.6, 0.6, 0.835);
    leg_down->SetHeader("Temperature da 58 #circ C a 20 #circ C (a scendere)");
    leg_down->AddEntry("g30", "Tensione dopo 30s");
    leg_down->AddEntry("g1", "Tensione dopo 1 min");
    leg_down->AddEntry("g2", "Tensione dopo 2 min");
    leg_down->AddEntry("g3", "Tensione dopo 3 min");
    leg_down->SetBorderSize(0);
    leg_down->SetFillColorAlpha(kWhite, 0);
    leg_down->Draw();

    TLegend* leg_up = new TLegend(0.2, 0.375, 0.6, 0.6);
    leg_up->SetHeader("Temperature da 20 #circ C a 49 #circ C (a salire)");
    leg_up->AddEntry("g30_up", "Tensione dopo 30s");
    leg_up->AddEntry("g1_up", "Tensione dopo 1 min");
    leg_up->AddEntry("g2_up", "Tensione dopo 2 min");
    leg_up->AddEntry("g3_up", "Tensione dopo 3 min");
    leg_up->SetBorderSize(0);
    leg_up->SetFillColorAlpha(kWhite, 0);
    leg_up->Draw();

    c2->SaveAs("../fig/plot_istresi.pdf");
    c2->Draw();

    std::ofstream o_file("../misc/tabella_f_T.log");
    for(int i = 0; i<g30->GetN(); i++){
        o_file << "$" << g30->GetY()[i] << "\\pm" << g30->GetEY()[i] << "$ & $" << g1->GetY()[i] << "\\pm" << g1->GetEY()[i] << "$ & $" << g2->GetY()[i] << "\\pm" << g2->GetEY()[i] << "$ & $" << g3->GetY()[i] << "\\pm" << g3->GetEY()[i] << "$ & $" << g30->GetX()[i] << "\\pm" << g30->GetEX()[i] << " \\[+0.5ex]" << std::endl;
    }
    o_file << "\\hline \\[-1.5ex]" << std::endl;
    for(int i = 1; i<g30_up->GetN(); i++){
        o_file << "$" << g30_up->GetY()[i] << "\\pm" << g30_up->GetEY()[i] << "$ & $" << g1_up->GetY()[i] << "\\pm" << g1_up->GetEY()[i] << "$ & $" << g2_up->GetY()[i] << "\\pm" << g2_up->GetEY()[i] << "$ & $" << g3_up->GetY()[i] << "\\pm" << g3_up->GetEY()[i] << "$ & $" << g30_up->GetX()[i] << "\\pm" << g30_up->GetEX()[i] << " \\[+0.5ex]" << std::endl;
    }

    return;
}
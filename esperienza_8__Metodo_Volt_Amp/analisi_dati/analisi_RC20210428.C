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

double tektronix_Verr_stat(double reading, double range){
    return max_to_stat((0.00015 * reading) + (0.00004 * range));
}

double amprobe_Aerr_stat(double reading){
    return max_to_stat((0.015 * reading));
}

// ** MAIN PROGRAM

const double R1_M = 9.9371e+03;                 // {ohm}
const double R1_Merr = amprobe_Aerr_stat(R1_M); // {ohm} // !! HANNO MISURATO CON IL MULT. DA BANCO?? **
const double R2_M = 32.770e+03;                 // {ohm}
const double R2_Merr = amprobe_Aerr_stat(R2_M); // {ohm} // !! HANNO MISURATO CON IL MULT. DA BANCO?? **

void analisi_RC20210428(){

    gStyle->SetFrameLineWidth(0);
    gStyle->SetTextFont(43);

    // ** METODO VOLT-AMPEROMETRICO PER RICAVARE R1 E R2

    std::ifstream m_VI_R1("../dati/metodo_VI/metodo_VI_R1.dat");
    std::ifstream m_VI_R2("../dati/metodo_VI/metodo_VI_R2.dat");

    double V_i, I_i, range_V, range_I;

    TCanvas* c1 = new TCanvas("c1", "", 600, 700);
    c1->SetMargin(0.16, 0.06, 0.12, 0.06);
    c1->Divide(1, 2);

    TGraphErrors* m_VI_g1 = new TGraphErrors();
    TGraphErrors* m_VI_g2 = new TGraphErrors();

    for(int i = 0; m_VI_R1 >> V_i >> range_V >> I_i >> range_I; i++){
        m_VI_g1->SetPoint(i, I_i, V_i);
        m_VI_g1->SetPointError(i, tektronix_Verr_stat(I_i, range_I), amprobe_Aerr_stat(V_i));
    }

    for(int i = 0; m_VI_R2 >> V_i >> range_V >> I_i >> range_I; i++){
        m_VI_g2->SetPoint(i, I_i, V_i);
        m_VI_g2->SetPointError(i, tektronix_Verr_stat(I_i, range_I), amprobe_Aerr_stat(V_i));
    }

    TF1* f1 = new TF1("f1", "pol1");
    TF1* f2 = new TF1("f2", "pol1");

    c1->cd(1);
    m_VI_g1->Fit("f1");
    m_VI_g1->Draw("ap");

    c1->cd(2);
    m_VI_g2->Fit("f2");
    m_VI_g2->Draw("ap");

    return;
}
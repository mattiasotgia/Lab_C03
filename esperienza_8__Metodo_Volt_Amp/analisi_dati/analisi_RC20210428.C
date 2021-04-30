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
        return "COMPATIBILE";
    }
    return "NON-COMPATIBILE";
}

double max_to_stat(double value){
    return value/(std::sqrt(3));
}

double tektronix_Aerr_stat(double reading, double range){
    if(range==200){
        return max_to_stat((0.0003 * reading) + (0.00005 * range));
    }
    return max_to_stat((0.0002 * reading) + (0.00005 * range));
}

double amprobe_Verr_stat(double reading, double range){
    if(range==2){
        return max_to_stat((0.01 * reading) + 0.001);
    }
    return max_to_stat((0.01 * reading) + 0.01);
}

double amprobe_Rerr_stat(double reading, double range){
    if(range==20){
        return max_to_stat((0.01 * reading) + (4 * 0.0001));
    }
    return max_to_stat((0.01 * reading) + (4 * 0.001));
}

double tektronix_Verr_stat(double reading){
    return max_to_stat((0.00015 * reading) + (0.00003 * 2));
}

double get_Verr(double current_reading, double last_reading, double time){

    double err1 = (std::abs(current_reading-last_reading) * 0.3)/(std::sqrt(12) * time);
    double err2 = tektronix_Verr_stat(current_reading);

    if(err1>err2){
        return err1;
    }
    return err2;
}

// ** MAIN PROGRAM

const double R1_M = 9.9371e-3;                  // {mohm}
const double R1_Merr = amprobe_Rerr_stat(R1_M, 20); // {mohm} // TODO: !! HANNO MISURATO CON IL MULT. DA BANCO?? **
const double R2_M = 32.770e-3;                  // {mohm}
const double R2_Merr = amprobe_Rerr_stat(R2_M, 200); // {mohm} // TODO: !! HANNO MISURATO CON IL MULT. DA BANCO?? **

struct resist
{
    double value[2];
    double err[2];
};


void analisi_RC20210428(){

    gStyle->SetFrameLineWidth(0);
    gStyle->SetTextFont(43);
    gStyle->SetLineScalePS(1);

    // ** METODO VOLT-AMPEROMETRICO PER RICAVARE R1 E R2

    print_mmsg("METODO VOLT-AMPEROMETRICO PER RICAVARE R1 E R2");

    double V_i, I_i, range_V, range_I;
    std::string paths_mVI[2] = {"../dati/metodo_VI/metodo_VI_R1.dat", "../dati/metodo_VI/metodo_VI_R2.dat"};

    TCanvas* c1 = new TCanvas("c1", "", 600, 1000);
    c1->SetMargin(0.16, 0.06, 0.12, 0.06);
    c1->SetFillStyle(4000);
    c1->Divide(1, 2);

    resist R;

    for(int i=0; i<2; i++){

        c1->cd(i+1);

        TGraphErrors* m_VI_g = new TGraphErrors();

        m_VI_g->SetTitle("");
        m_VI_g->GetYaxis()->SetTitle("Tensione elettrica [V]");
        m_VI_g->GetYaxis()->SetTitleOffset(2);
        m_VI_g->GetYaxis()->SetTitleFont(43);
        m_VI_g->GetYaxis()->SetTitleSize(24);
        m_VI_g->GetYaxis()->SetLabelFont(43);
        m_VI_g->GetYaxis()->SetLabelSize(12);
        m_VI_g->GetYaxis()->CenterTitle();

        TF1* f = new TF1("f", "[0]*x");

        TPad* p1 = new TPad("", "", 0.0, 0.3, 1.0, 1.0);
        TPad* p2 = new TPad("", "", 0.0, 0.0, 1.0, 0.295); 
        p1->SetMargin(0.14, 0.06, 0.0, 0.06);
        p1->SetFillStyle(4000);
        p1->Draw();
        p2->SetMargin(0.14, 0.06, 0.4, 1.0);
        p2->SetFillStyle(4000);
        p2->Draw();

        TLatex* sl_1 = new TLatex();
        sl_1->SetTextFont(43);
        sl_1->SetTextSize(15);
        sl_1->SetTextColor(kBlack);

        TLatex* header = new TLatex();
        header->SetTextFont(43);
        header->SetTextSize(15);

        TGraphErrors* rg = new TGraphErrors();
        TF1* rf = new TF1("rf", "0", 0, 1100); 

        rg->GetXaxis()->SetTitle("Corrente [#muA]");
        rg->GetXaxis()->SetTitleOffset(5);
        rg->GetXaxis()->SetTitleFont(43);
        rg->GetXaxis()->SetTitleSize(24);

        rg->GetYaxis()->SetTitle("Residui");
        rg->GetYaxis()->SetTitleOffset(2);
        rg->GetYaxis()->SetTitleFont(43);
        rg->GetYaxis()->SetTitleSize(24);
        rg->GetYaxis()->CenterTitle();

        rg->GetYaxis()->SetLabelFont(43);
        rg->GetYaxis()->SetLabelSize(12);
        rg->GetYaxis()->SetNdivisions(5, 5, 0);
        rg->GetXaxis()->SetLabelFont(43);
        rg->GetXaxis()->SetLabelSize(12);
        rg->GetXaxis()->CenterTitle();

        rf->SetLineStyle(2);

        std::ifstream m_VI_READ(paths_mVI[i]);

        for(int j=0; m_VI_READ >> V_i >> range_V >> I_i >> range_I; j++){
            m_VI_g->SetPoint(j, I_i, V_i);
            m_VI_g->SetPointError(j, tektronix_Aerr_stat(I_i, range_I), amprobe_Verr_stat(V_i ,range_V));
        }

        p1->cd();
        m_VI_g->Draw("ap");
        m_VI_g->Fit("f");

        std::string ss_1="#chi^{2}/ndf (prob.) = "
            +std::to_string(f->GetChisquare())+"/"
            +std::to_string(f->GetNDF())
            +" ("+std::to_string(f->GetProb())+")";

        header->DrawLatexNDC(0.20, 0.85, ("#splitline{" + paths_mVI[i] + "}{#bf{Misura R" + std::to_string(i+1) + "}}").c_str());

        sl_1->DrawLatexNDC(0.50, 0.10, ss_1.c_str());
        
        print_stat(f);
        R.value[i] = f->GetParameter(0);
        R.err[i] = f->GetParError(0);

        // ** RESIDUI

        p2->cd();

        for(int i=0; i<m_VI_g->GetN(); i++){
            rg->SetPoint(i, m_VI_g->GetX()[i], (m_VI_g->GetY()[i] - f->Eval(m_VI_g->GetX()[i])));
            rg->SetPointError(i, 0, m_VI_g->GetEY()[i]);
        }

        rg->Draw("ap");
        rf->Draw("same");

    }

    print_mmsg("CONTROLLO COMPATIBILITA' R1 E R2");

    std::cout << " ** R1 => " << compatible(R1_M, R1_Merr, R.value[0], R.err[0]) << std::endl; 
    std::cout << "R1 (misurata) " << R1_M << " +/- " << R1_Merr << " milli ohm" << std::endl;
    std::cout << "R1 (ricavata) " << R.value[0] << " +/- " << R.err[0] << " milli ohm" << std::endl << std::endl;

    std::cout << " ** R2 => " << compatible(R2_M, R2_Merr, R.value[1], R.err[1]) << std::endl; 
    std::cout << "R2 (misurata) " << R2_M << " +/- " << R2_Merr << " milli ohm" << std::endl;
    std::cout << "R2 (ricavata) " << R.value[1] << " +/- " << R.err[1] << " milli ohm" << std::endl << std::endl;

    c1->SaveAs("../fig/misura_R1_R2.pdf");

    // ** STUDIO CIRCUITO RC


    return;
}

#ifndef __CINT__
int main(){
    analisi_RC20210428();
}
#endif
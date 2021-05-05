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

const double title_size = 21;


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

double tektronix_Rerr_stat(double reading, double range){
    return max_to_stat((0.0002 * reading) + (0.00003 * range));
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

double getC(double tau, double R){
    return tau/R;
}

double getCerr(double C, double tau, double R, double tauerr, double Rerr){
    return C * sqrt(
        pow(tauerr/tau, 2) + pow(Rerr/R, 2)
    );
}

// ** MAIN PROGRAM

const double R1_M = 9.9371;                            // {kOhm}
const double R1_Merr = tektronix_Rerr_stat(R1_M, 20);  // {kOhm} // TODO: !! HANNO MISURATO CON IL MULT. DA BANCO?? **
const double R2_M = 32.770;                            // {kOhm}
const double R2_Merr = tektronix_Rerr_stat(R2_M, 200); // {kOhm} // TODO: !! HANNO MISURATO CON IL MULT. DA BANCO?? **

struct result
{
    double value[4];
    double err[4];
};


void analisi_RC20210428(){

    // Programma di analisi dati per l'esperienza 8.
    // Il programma prende come file di input per la prima parte
    // due file con i valori di I e V relativi alla misura fatta 
    // delle resistenze R1 e R2. 
    // (../dati/metodo_VI/metodo_VI_R1.dat e metodo_VI_R2.dat)
    //
    // Nella seconda parte prende come file di input i 4 file in
    // ..dati/circuito_RC/ . 


    std::ofstream out_table("result_table.txt");

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

    result R;

    for(int i=0; i<2; i++){

        c1->cd(i+1);

        TGraphErrors* m_VI_g = new TGraphErrors();

        m_VI_g->SetTitle("");
        m_VI_g->GetYaxis()->SetTitle("Tensione elettrica [V]");
        m_VI_g->GetYaxis()->SetTitleOffset(2);
        m_VI_g->GetYaxis()->SetTitleFont(43);
        m_VI_g->GetYaxis()->SetTitleSize(title_size);
        m_VI_g->GetYaxis()->SetLabelFont(43);
        m_VI_g->GetYaxis()->SetLabelSize(12);
        m_VI_g->GetYaxis()->CenterTitle();

        TF1* f = new TF1("f", "pol1");

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
        rg->GetXaxis()->SetTitleSize(title_size);

        rg->GetYaxis()->SetTitle("Residui [#sigma]");
        rg->GetYaxis()->SetTitleOffset(2);
        rg->GetYaxis()->SetTitleFont(43);
        rg->GetYaxis()->SetTitleSize(title_size);
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

            out_table << I_i << " " << tektronix_Aerr_stat(I_i, range_I) << " " << V_i << " " <<  amprobe_Verr_stat(V_i ,range_V) << std::endl;

        }
        out_table << "EOL" << std::endl;

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
        R.value[i] = f->GetParameter(1) * std::pow(10, 3);
        R.err[i] = f->GetParError(1) * std::pow(10, 3);

        std::cout << "** COMPATIBILITA' DI ZERO PER R" << i << " => " << compatible(f->GetParameter(0), f->GetParError(0), 0, 0) << std::endl << std::endl;

        // ** RESIDUI

        p2->cd();

        for(int i=0; i<m_VI_g->GetN(); i++){
            rg->SetPoint(i, m_VI_g->GetX()[i], (m_VI_g->GetY()[i] - f->Eval(m_VI_g->GetX()[i]))/m_VI_g->GetEY()[i]);
            rg->SetPointError(i, 0, 1);
        }

        rg->Draw("ap");
        rf->Draw("same");

    }

    print_mmsg("CONTROLLO COMPATIBILITA' R1 E R2");

    std::cout << " ** R1 => " << compatible(R1_M, R1_Merr, R.value[0], R.err[0]) << std::endl; 
    std::cout << "R1 (misurata) " << R1_M << " +/- " << R1_Merr << " kOhm" << std::endl;
    std::cout << "R1 (ricavata) " << R.value[0] << " +/- " << R.err[0] << " kOhm" << std::endl << std::endl;

    std::cout << " ** R2 => " << compatible(R2_M, R2_Merr, R.value[1], R.err[1]) << std::endl; 
    std::cout << "R2 (misurata) " << R2_M << " +/- " << R2_Merr << " kOhm" << std::endl;
    std::cout << "R2 (ricavata) " << R.value[1] << " +/- " << R.err[1] << " kOhm" << std::endl << std::endl;

    c1->SaveAs("../fig/misura_R1_R2.pdf");


    // ** STUDIO CIRCUITO RC

    print_mmsg("STUDIO CIRCUITO RC");

    result V_I0, tau, t_0;

    TCanvas* c2 = new TCanvas("c2", "", 1200, 1000);
    c2->SetMargin(0.16, 0.06, 0.12, 0.06);
    c2->Divide(2, 2);

    std::string paths_RC_zero[4] = {
        "../dati/circuito_RC/dati_azzerati/caricaR1_2.dat", 
        "../dati/circuito_RC/dati_azzerati/scaricaR1_2.dat",
        "../dati/circuito_RC/dati_azzerati/caricaR2_2.dat",
        "../dati/circuito_RC/dati_azzerati/scaricaR2_2.dat"
    };

    // std::string paths_RC_zero[4] = {
    //     "../dati/circuito_RC/dati_normalizzati/norm_caricaR1_2.dat", 
    //     "../dati/circuito_RC/dati_normalizzati/norm_scaricaR1_2.dat",
    //     "../dati/circuito_RC/dati_normalizzati/norm_caricaR2_2.dat",
    //     "../dati/circuito_RC/dati_normalizzati/norm_scaricaR2_2.dat"
    // };

    std::string head[4] = {
            "Carica consensatore (resistenza R1)",
            "Scarica condensatore (resistenza R1)",
            "Carica consensatore (resistenza R2)",
            "Scarica condensatore (resistenza R2)"
        };

    for(int i=0; i<4; i++){

        c2->cd(i+1);

        std::cout << "** READING FROM FILE "<< paths_RC_zero[i] << std::endl << std::endl;
        std::ifstream dati_RC(paths_RC_zero[i]);
        double time, V;

        TGraphErrors* RC_g = new TGraphErrors();

        RC_g->SetTitle("");
        RC_g->GetYaxis()->SetTitle("Tensione elettrica [V]");
        RC_g->GetYaxis()->SetTitleOffset(2);
        RC_g->GetYaxis()->SetTitleFont(43);
        RC_g->GetYaxis()->SetTitleSize(title_size);
        RC_g->GetYaxis()->SetLabelFont(43);
        RC_g->GetYaxis()->SetLabelSize(12);
        RC_g->GetYaxis()->CenterTitle();

        // ** CARICA, ALTRIMENTI CAMBIA
        std::string formula = "[0]*(1-exp(-(x-[2])/[1]))";

        // V_0/V_I = [0], tau = [1], t_0 = [2] for both formulas

        // ** SCARICA
        if(i==1 || i==3) formula = "[0]*exp(-(x)/[1])";

        TF1* f_RC = new TF1("f_RC", formula.c_str());

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
        TF1* rf = new TF1("rf", "0", -10, 100); 

        rg->GetXaxis()->SetTitle("Tempo [s]");
        rg->GetXaxis()->SetTitleOffset(5);
        rg->GetXaxis()->SetTitleFont(43);
        rg->GetXaxis()->SetTitleSize(title_size);

        rg->GetYaxis()->SetTitle("Residui [#sigma]");
        rg->GetYaxis()->SetTitleOffset(2);
        rg->GetYaxis()->SetTitleFont(43);
        rg->GetYaxis()->SetTitleSize(title_size);
        rg->GetYaxis()->CenterTitle();

        rg->GetYaxis()->SetLabelFont(43);
        rg->GetYaxis()->SetLabelSize(12);
        rg->GetYaxis()->SetNdivisions(5, 5, 0);
        rg->GetXaxis()->SetLabelFont(43);
        rg->GetXaxis()->SetLabelSize(12);
        rg->GetXaxis()->CenterTitle();

        rf->SetLineStyle(2);

        // ** CARICO I DATI

        dati_RC >> time >> V; // leggo la prima riga
        RC_g->SetPoint(0, 0, V); // imposto il primo punto

        double t0 = time;

        for (int j=1; dati_RC >> time >> V; j++){
            RC_g->SetPointError(
                j-1, 
                0.005 * ((time-t0) - RC_g->GetX()[j-1]), 
                get_Verr(V, RC_g->GetY()[j-1], (time - RC_g->GetX()[j-1]))
            );
            // ** PUNTO SUCCESSIVO
            RC_g->SetPoint(j, time-t0, V);
        }
        RC_g->SetPointError(
            RC_g->GetN()-1, 
            0.005 * (RC_g->GetX()[RC_g->GetN()] - RC_g->GetX()[RC_g->GetN()-1]), 
            get_Verr(V, RC_g->GetY()[RC_g->GetN()-1], (RC_g->GetX()[RC_g->GetN()] - RC_g->GetX()[RC_g->GetN()-1]))
        );
        
        if(i==1){
            f_RC->SetParameters(1, 10, 0);
        }else if(i==3){
            f_RC->SetParameters(1, 30, 0);
        }else{
            f_RC->SetParameters(1, 20, 0);
        }

        p1->cd();
        RC_g->Draw("ap");
        RC_g->Fit("f_RC", "E");

        std::string ss_1="#chi^{2}/ndf (prob.) = "
            +std::to_string(f_RC->GetChisquare())+"/"
            +std::to_string(f_RC->GetNDF())
            +" ("+std::to_string(f_RC->GetProb())+")";

        double xpos=0.35, ypos = 0.2;

        if(i==1 || i==3){
            xpos = 0.35;
            ypos = 0.85;
        }

        header->DrawLatexNDC(xpos, ypos, ("#splitline{" + paths_RC_zero[i] + "}{#bf{" + head[i] + "}}").c_str());
        sl_1->DrawLatexNDC(xpos, ypos-0.1, ss_1.c_str());
        
        print_stat(f_RC);

        // V_0/V_I = [0], tau = [1], t_0 = [2] for both formulas
        // ** READING OUPUT VALUES FROM FIT

        V_I0.value[i] = f_RC->GetParameter(0); V_I0.err[i] = f_RC->GetParError(0);
        std::cout << "V_0/V_I " << V_I0.value[i] << " +/- " << V_I0.err[i] << " V" << std::endl;
        tau.value[i] = f_RC->GetParameter(1); tau.err[i] = f_RC->GetParError(1);
        std::cout << "tau     " << tau.value[i] << " +/- " << tau.err[i] << " s" << std::endl << std::endl;
        if(i==0 || i==2){
            t_0.value[i] = f_RC->GetParameter(2); t_0.err[i] = f_RC->GetParError(2);
            std::cout << " ** COMPATIBILITA' DI ZERO =>" << compatible(t_0.value[i], t_0.err[i], 0, 0) << std::endl;
            std::cout << "t_0     " << t_0.value[i] << " +/- " << t_0.err[i] << " s" << std::endl << std::endl;
        }

        // ** RESIDUI

        p2->cd();

        for(int i=0; i<RC_g->GetN(); i++){
            rg->SetPoint(i, RC_g->GetX()[i], (RC_g->GetY()[i] - f_RC->Eval(RC_g->GetX()[i]))/RC_g->GetEY()[i]);
            rg->SetPointError(i, 0, 1);
        }

        rg->Draw("ap");
        rf->Draw("same");

        if(i==2 || i==3){
            RC_g->GetXaxis()->SetLimits(-5, 95);
            rg->GetXaxis()->SetLimits(-5, 95);
        }else if(i==0){
            RC_g->GetXaxis()->SetLimits(-5, 65);
            rg->GetXaxis()->SetLimits(-5, 65);
        }else{
            RC_g->GetXaxis()->SetLimits(-5, 55);
            rg->GetXaxis()->SetLimits(-5, 55);
        }

    }

    c2->SaveAs("../fig/plot_RC.pdf");

    // ** IMPLEMENTARE FINALE ANALISI DATI

    print_mmsg("STIMA DELLA CAPACITA' C ");

    result C;

    C.value[0] = getC(tau.value[0], R.value[0]); C.err[0] = getCerr(getC(tau.value[0], R.value[0]), tau.value[0], R.value[0], tau.err[0], R.err[0]);
    C.value[1] = getC(tau.value[1], R.value[0]); C.err[1] = getCerr(getC(tau.value[1], R.value[0]), tau.value[1], R.value[0], tau.err[1], R.err[0]);
    C.value[2] = getC(tau.value[2], R.value[1]); C.err[2] = getCerr(getC(tau.value[2], R.value[1]), tau.value[2], R.value[1], tau.err[2], R.err[1]);
    C.value[3] = getC(tau.value[3], R.value[1]); C.err[3] = getCerr(getC(tau.value[3], R.value[1]), tau.value[3], R.value[1], tau.err[3], R.err[1]);

    std::cout << "C (da carica R1)  " << C.value[0] << " +/- " << C.err[0] << " mF" << std::endl;
    std::cout << "C (da scarica R1) " << C.value[1] << " +/- " << C.err[1] << " mF" << std::endl;
    std::cout << "C (da carica R2)  " << C.value[2] << " +/- " << C.err[2] << " mF" << std::endl;
    std::cout << "C (da scarica R2) " << C.value[3] << " +/- " << C.err[3] << " mF" << std::endl;
    std::cout << std::endl;

    for(int m=0; m<4; m++){
        for(int n=0; n<4; n++){
            if(n!=m){
                std::cout << " ** COMPATIBILITA' TRA C[" << m << "] E C[" << n << "] => " << compatible(C.value[m], C.err[m], C.value[n], C.err[n]) << std::endl;
            }
        }
    }

    return;
}

#ifndef __CINT__
int main(){
    analisi_RC20210428();
    return 0;
}
#endif
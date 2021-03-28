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

struct x_values{
    double xpos[2];
    double xneg[2];        
};

void print_mmsg(std::string mmsg){
    std::cout << std::endl 
        << "*****************************************************" << std::endl
        << mmsg << std::endl
        << "*****************************************************" << std::endl
        << std::endl;
}

x_values isocrony_x(Double_t* params_1, const Double_t* err_params_1, 
                    Double_t* params_2, const Double_t* err_params_2){
    x_values x;

    // non usare valori assoluti per a, b e c per evitare di ottenere valore di isocronia errato.
    double a = params_1[0]-params_2[0], err_a = abs(err_params_1[0])+abs(err_params_2[0]);
    double b = params_1[1]-params_2[1], err_b = abs(err_params_1[1])+abs(err_params_2[1]);
    double c = params_1[2]-params_2[2], err_c = abs(err_params_1[2])+abs(err_params_2[2]);

    double delta = pow(b, 2) - 4*a*c;

    if(delta<0){
        std::cout << "delta is negative!" << std::endl;
        return {{0,0}, {0,0}};
    }

    x.xpos[0] = (-b+sqrt(delta))/(2*a);
    x.xneg[0] = (-b-sqrt(delta))/(2*a);

    double dda_sqr_pos = pow(((-1)*c/(a*sqrt(delta))) - ((-b + sqrt(delta))/(2*a*a)), 2);
    double ddb_sqr_pos = pow((-1 + b/sqrt(delta))/(2*a), 2);
    double ddc_sqr_pos = pow(-1/sqrt(delta), 2);

    x.xpos[1] = sqrt((dda_sqr_pos*err_a*err_a) + (ddb_sqr_pos*err_b*err_b) + (ddc_sqr_pos*err_c*err_c));

    double dda_sqr_neg = pow((b*b - 2*a*c + b*sqrt(delta))/(2*a*a*sqrt(delta)), 2);
    double ddb_sqr_neg = pow((-1 - (b/sqrt(delta)))/(2*a), 2);
    double ddc_sqr_neg = pow(1/sqrt(delta), 2);

    x.xneg[1] = sqrt((dda_sqr_neg*err_a*err_a) + (ddb_sqr_neg*err_b*err_b) + (ddc_sqr_neg*err_c*err_c));

    return x;
}

double* get_isoX(x_values x, TGraphErrors _g){
    
    double* _x = new double[2];

    if(x.xpos[0]>TMath::MinElement(_g.GetN(), _g.GetX())){
        _x[0] = x.xpos[0]; _x[1] = x.xpos[1];
    }else{
        _x[0] = x.xneg[0]; _x[1] = x.xneg[1];
    }
    
    return _x;

}

double get_err_T(double x_iso, TGraphErrors g1, TGraphErrors g2){

    double Terr_iso_1 = 0;
    double Terr_iso_2 = 0;
    double range = abs(g1.GetX()[0]-x_iso);

    for(int i=0; i<g1.GetN(); i++){
        // std::cout << i << " " << abs(x_iso-g1.GetX()[i]) << " x: " << g1.GetX()[i] << std::endl;
        // algorimo per identificazione dei valori di T1 T2 più
        // vicini al valore di isocronia

        if((abs(x_iso-g1.GetX()[i])<range)){
            Terr_iso_1 = g1.GetErrorY(i);
            Terr_iso_2 = g2.GetErrorY(i);
            range = abs(x_iso-g1.GetX()[i]);
        }
    }

    return sqrt(pow(Terr_iso_1, 2)+pow(Terr_iso_2, 2));
}

void kater_plot(bool fast = false){

    // gStyle->SetFrameLineWidth(0);

    gStyle->SetTextFont(42);

    TCanvas c1("c", "", 600, 500);
    c1.SetMargin(0.16, 0.06, 0.12, 0.06);
    // c1.SetGrid();

    TGraphErrors g1("../dati/computed_T1_x.txt");
    g1.SetMarkerStyle(4);
    g1.SetLineColor(kBlack);
    TGraphErrors g2("../dati/computed_T2_x.txt");
    g2.SetLineColor(kRed);
    g2.SetMarkerStyle(4);
    g2.SetMarkerColor(kRed);

    TF1 f1("f1", "[0]*x*x+[1]*x+[2]"); // a1 = p0, b1 = p1, c1 = p2;
    f1.SetParameters(0.4, 0.1, 1.4);
    f1.SetLineColor(kBlack);
    TF1 f2("f2", "[0]*x*x+[1]*x+[2]"); // a2 = p0, b2 = p1, c2 = p2;
    // f2.SetParameters(0.8, -1.1, 2);
    f2.SetLineColor(kRed);

    g1.SetTitle("");
    g1.GetXaxis()->SetTitle("Posizione x_{b} della massa M_{b} (m)");
    g1.GetYaxis()->SetTitle("Periodo T (s)");
    g1.GetXaxis()->SetTitleOffset(0.85);
    g1.GetXaxis()->SetTitleSize(0.06);
    g1.GetYaxis()->SetTitleSize(0.06);
    g1.GetXaxis()->CenterTitle();
    g1.GetYaxis()->CenterTitle();


    g1.Draw("ap");
    g1.Fit("f1");

    g2.Draw("p");
    g2.Fit("f2");

    Double_t* par_1 = f1.GetParameters();
    const Double_t* par_1_err = f1.GetParErrors();
    Double_t* par_2 = f2.GetParameters();
    const Double_t* par_2_err = f2.GetParErrors();

    if(false){
        x_values x;
        x = isocrony_x(par_1, par_1_err, par_2, par_2_err);

        std::cout << "x+ = " << x.xpos[0] << " +/- " << x.xpos[1] << std::endl;
        std::cout << "(T*)+ = " << f1.Eval(x.xpos[0]) << std::endl;
        std::cout << "x- = " << x.xneg[0] << " +/- " << x.xneg[1] << std::endl;
        std::cout << "(T*)- = " << f1.Eval(x.xneg[0]) << std::endl;
        std::cout << "--------------------------------------------" << std::endl
                  << "            AUTOMATIC METHOD" << std::endl
                  << "--------------------------------------------" << std::endl;
    }

    double* x_iso = get_isoX(isocrony_x(par_1, par_1_err, par_2, par_2_err), g1);
    double Terr_iso = get_err_T(x_iso[0], g1, g2);

    std::cout << "x* = " << x_iso[0] << " +/- " << x_iso[1] << std::endl;
    std::cout << "T* = " << f1.Eval(x_iso[0]) << " +/- " << Terr_iso << std::endl;

    if(!fast){
        std::string ss_1="#chi^{2}/ndf (prob.) = "
            +std::to_string(f1.GetChisquare())+"/"
            +std::to_string(f1.GetNDF())
            +" ("+std::to_string(f1.GetProb())+")";
        TLatex sl_1;
        sl_1.SetTextSize(0.035);
        sl_1.SetTextColor(kBlack);
        sl_1.DrawLatexNDC(0.50, 0.28, ss_1.c_str());
        
        
        std::string ss_2="#chi^{2}/ndf (prob.) = "
            +std::to_string(f2.GetChisquare())+"/"
            +std::to_string(f2.GetNDF())
            +" ("+std::to_string(f2.GetProb())+")";
        TLatex sl_2;
        sl_1.SetTextSize(0.035);
        sl_1.SetTextColor(kRed);
        sl_1.DrawLatexNDC(0.50, 0.24, ss_2.c_str());
    }
    

    TLatex header;
	header.SetTextSize(0.065);
    header.DrawLatexNDC(0.25, 0.85, "#bf{Pendolo di Kater}");

    // test(); // FOR TESTING PURPOSES

    c1.SaveAs("../fig/kater.pdf");

    delete[] x_iso;
    return;
}

int main(){
    kater_plot();
    return 0;
}
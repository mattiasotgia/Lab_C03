#include <vector>
#include <cmath>
#include <iostream>

#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TStyle.h>
#include <TAxis.h>
#include <TMath.h>

struct x_values{
    double xpos[2];
    double xneg[2];        
};

x_values isocrony_x(Double_t* params_1, const Double_t* err_params_1, 
                    Double_t* params_2, const Double_t* err_params_2){
    x_values x;

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

    x.xneg[1] = sqrt((dda_sqr_neg*err_a*err_a) + (ddb_sqr_neg*err_b*err_b) + (ddc_sqr_neg*err_c*err_c));;

    return x;
}

// double* get_isoX(x_values x){
//     double _x = {0,0};

//     return _x;

// }

void test(){
    TF1 f01("f01", "[0]*x*x+[1]*x+[2]"); // a = p0, b = p1, c = p2;
    f01.SetParameters(1,2,1);
    TF1 f02("f02", "[0]*x*x+[1]*x+[2]"); // a = p0, b = p1, c = p2;
    f02.SetParameters(1.1, 4.3, 1.2);

    Double_t* par01 = f01.GetParameters();
    const Double_t* par01_err = f01.GetParErrors();
    Double_t* par02 = f02.GetParameters();
    const Double_t* par02_err = f02.GetParErrors();

    x_values x;
    x = isocrony_x(par01, par01_err, par02, par02_err);

    std::cout << "x+ = " << x.xpos[0] << "+/-" << x.xpos[1] << std::endl;
    std::cout << "x- = " << x.xneg[0] << "+/-" << x.xneg[1] << std::endl;

    return;
}

void kater_multigraph(){

    gStyle->SetFrameLineWidth(0);

    gStyle->SetTextFont(42);

    TCanvas c1("c", "", 600, 500);
    c1.SetMargin(0.16, 0.06, 0.12, 0.06);
    c1.SetGrid();

    TGraphErrors g1("../dati/computed_T1_x.txt");
    g1.SetLineColor(kBlack);
    TGraphErrors g2("../dati/computed_T2_x.txt");
    g2.SetLineColor(kRed);

    TF1 f1("f1", "[0]*x*x+[1]*x+[2]"); // a1 = p0, b1 = p1, c1 = p2;
    f1.SetParameters(0.4, 0.1, 1.4);
    f1.SetLineColor(kBlack);
    TF1 f2("f2", "[0]*x*x+[1]*x+[2]"); // a2 = p0, b2 = p1, c2 = p2;
    f2.SetParameters(0.8, -1.1, 2);
    f2.SetLineColor(kRed);

    g1.SetTitle("");
    g1.GetXaxis()->SetTitle("Posizione M_{b} x_{b} (m)");
    g1.GetYaxis()->SetTitle("Periodo T (s)");
    g1.GetXaxis()->SetTitleOffset(0.85);
    g1.GetXaxis()->SetTitleSize(0.06);
    g1.GetYaxis()->SetTitleSize(0.06);
    g1.GetXaxis()->CenterTitle();
    g1.GetYaxis()->CenterTitle();

    // double ymin = TMath::Min(TMath::MinElement(g1.GetN(), g1.GetY()), TMath::MinElement(g2.GetN(), g2.GetY()))
    //             + 0.1
    // double ymax = TMath::Max(TMath::MaxElement(g1.GetN(), g1.GetY()), TMath::MaxElement(g2.GetN(), g2.GetY()))
    //             + 0.1*

    // g1.SetMinimum(ymin);
    // g1.SetMaximum(ymax);
    // // g1.GetXaxis()->SetLimits(0.15, 1.2);

    g1.Draw("ap");
    g1.Fit("f1");

    g2.Draw("p");
    g2.Fit("f2");

    Double_t* par_1 = f1.GetParameters();
    const Double_t* par_1_err = f1.GetParErrors();
    Double_t* par_2 = f2.GetParameters();
    const Double_t* par_2_err = f2.GetParErrors();

    x_values x;
    x = isocrony_x(par_1, par_1_err, par_2, par_2_err);

    std::cout << "x+ = " << x.xpos[0] << " +/- " << x.xpos[1] << std::endl;
    std::cout << "x- = " << x.xneg[0] << " +/- " << x.xneg[1] << std::endl;

    // test(); // FOR TESTING PURPOSES

    c1.SaveAs("test.pdf");

    return;
}

int main(){
    // test();
    kater_multigraph();
    return 0;
}
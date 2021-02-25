{
	ifstream file("../dati/sotgia/dati_C03_201116_presa_dati1_analisi.txt");

	Double_t V, eV;
	Char_t *note;
	
	auto c = new TCanvas("c", "", 800, 700);
	c->SetLeftMargin(0.15);
 
	
	TGraphErrors g1;
	g1.SetName("g1");
	TGraphErrors g2;
	g2.SetName("g2");
	TGraphErrors g3;
	g3.SetName("g3");

	
	auto legend = new TLegend(0.1, 0.1, 0.1, 0.1);

	legend->SetFillColor(0);
	legend->SetBorderSize(0);
	legend->SetTextSize(0.03);
	
	
	TF1 f("conv_Line", "[0]");
	f.SetParameter(0, 1.);
	
	file >> V >> eV >> note;
	g1.SetPoint(0, 1, V);
	g1.SetPointError(0, 0.5, eV);
	g1.SetMarkerStyle(31);
	g1.SetMarkerSize(2.0);
	g1.SetLineColor(kBlue);
	legend->AddEntry("g1", note, "lep");

	file >> V >> eV >> note;
	g2.SetPoint(1, 2, V);
	g2.SetPointError(1, 0.5, eV);
	g2.SetMarkerStyle(31);
	g2.SetMarkerSize(2.0);
	g2.SetLineColor(kGreen);
	legend->AddEntry("g2", note, "lep");

	file >> V >> eV >> note;
	g3.SetPoint(2, 3, V);
	g3.SetPointError(2, 0.5, eV);
	g3.SetMarkerStyle(31);
	g3.SetMarkerSize(2.0);
	g3.SetLineColor(kRed);
	legend->AddEntry("g3", note, "lep");
	
	
	g1.GetXaxis()->SetNdivisions(0);
	g1.GetYaxis()->SetTitle("Volume (mm^{3})");

	g1.Draw("ap");
	g2.Draw("p");
	g3.Draw("p");

	/* TGraphErrors g(g1+g2+g3); */
	/* g.Fit("conv_Line"); */
}

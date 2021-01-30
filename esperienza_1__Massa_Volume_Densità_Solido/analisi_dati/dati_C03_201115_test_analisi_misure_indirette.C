{

    auto c1 = new TCanvas("c1", "c1", 800, 500);
	c1->Divide(2,1);
	
	ifstream file("../dati/sotgia/dati_C03_201115_test_analisi_misure_indirette.txt");

	double D, V, eD, eV;

	TGraphErrors g;

	auto h = new TH2D("h" , "", 1, 0.5, 1.5, 100, 113.5e+3, 120.5e+3);
	
	while(file >> D >> V >> eD >> eV){
		
		g.SetPoint(g.GetN(), g.GetN(), V);
		g.SetPointError(g.GetN() - 1, 0.2, eV);

		h->Fill(1,V);		
	}

	g.SetTitle("data_C03_201115_test_analisi_misure_dirette");
	g.GetYaxis()->SetTitle("Volume (mm^{3})");

	h->SetTitle("Volume Distribution");
	h->GetXaxis()->SetNdivisions(0);
	h->SetLineColor(kBlack);
	
	c1->cd(1);
	g.Draw("a*");
	c1->cd(2);
	h->Draw("candlex5");

}

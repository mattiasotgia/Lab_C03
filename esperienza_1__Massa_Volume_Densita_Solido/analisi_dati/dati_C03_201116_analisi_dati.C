{
	TGraphErrors v("../dati/sotgia/dati_C03_201116_presa_dati1_analisi_v.txt");
	v.GetXaxis()->SetNdivisions(0);
	v.SetTitle("");
	v.SetMarkerStyle(21);
	v.SetMarkerSize(2.0);
	v.SetLineColor(kBlack);
	v.GetYaxis()->SetTitle("Volume (mm^{3})");

	TAxis *vx = v.GetXaxis();
	vx->SetLimits(0, 5.);
	
	TGraphErrors ro("../dati/sotgia/dati_C03_201116_presa_dati1_analisi_ro.txt");
	ro.SetMarkerStyle(21);
	ro.SetMarkerSize(2.0);
	ro.SetLineColor(kRed);
	ro.SetFillColor(kRed);
	ro.SetMarkerColor(kRed);
	
	TCanvas c1("c1", "v_ro_disc", 800, 700);
	c1.SetMargin(0.15, 0.15, 0.10, 0.10);
	TPad *p1 = new TPad("p1", "",  0, 0, 1, 1);
	p1->SetGrid();
	p1->SetMargin(0.15, 0.10, 0.10, 0.10);
	TPad *p2 = new TPad("p2", "", 0, 0, 1, 1);
	p2->SetMargin(0.15, 0.15, 0.10, 0.10);
	p2->SetFillStyle(4000);

	p1->Draw();
	p1->cd();
	
	v.Draw("ap");
	v.GetHistogram()->GetXaxis()->SetTitleOffset(1.25);
	v.GetHistogram()->GetYaxis()->SetTitleOffset(1.25);
	gPad->Update();

	Style_t tfont = v.GetHistogram()->GetYaxis()->GetTitleFont();
	Float_t tsize = v.GetHistogram()->GetYaxis()->GetTitleSize();
	Style_t lfont = v.GetHistogram()->GetYaxis()->GetLabelFont();
	Float_t lsize = v.GetHistogram()->GetYaxis()->GetLabelSize();

	
	Double_t xmin = p1->GetUxmin();
	Double_t xmax = p1->GetUxmax();
	Double_t dx = (xmax - xmin) / 0.8; // 10 percent margins left and right
	Double_t ymin = ro.GetHistogram()->GetMinimum();
	Double_t ymax = ro.GetHistogram()->GetMaximum();
	Double_t dy = (ymax - ymin) / 0.8; // 10 percent margins top and bottom
	p2->Range(xmin-0.1*dx, ymin-0.1*dy, xmax+0.1*dx, ymax+0.1*dy);

	p2->Draw();
	p2->cd();
	ro.Draw("P");
	gPad->Update();
  
	TGaxis *axis = new TGaxis(xmax, ymin, xmax, ymax, ymin, ymax, 510, "+L");
	axis->SetTitle("#rho(g/mm^{3})");
	axis->SetTitleFont(tfont);
	axis->SetTitleSize(tsize);
	axis->SetTitleColor(kRed);
	axis->SetTitleOffset(1.25);
	axis->SetLabelFont(lfont);
	axis->SetLabelSize(lsize*0.8);
	axis->SetLabelColor(kRed);
	axis->SetLineColor(kRed);
	axis->SetNdivisions(4);
	axis->Draw();
	gPad->Update();
}

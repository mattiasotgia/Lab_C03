{
	ifstream f("var_Diam.txt");

	Double_t d;

	TH1D h("h", "var_Diam", 10, 20, 2);

	while(f >> d){
		h.Fill(d);
	}

	h.Draw();
}

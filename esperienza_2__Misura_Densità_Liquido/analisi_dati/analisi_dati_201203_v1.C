{
	ifstream v_dirette("../dati/v_dirette.txt");
	ifstream v_idro("../dati/v_idrostatico.txt");
	ifstream L2("../dati/L2_v_idrostatico.txt");
	ifstream LM("../dati/LM_v_idrostatico.txt");

	Double_t A, eA, B, eB, C, eC, D, eD, avgA, avgB, avgC, avgD, sumA=0, sumB=0, sumC=0, sumD=0;
	while(v_dirette>>A>>eA>>B>>eB>>C>>eC>>D>>eD){
		sumA+=A;
		sumB+=B;
		sumC+=C;
		sumD+=D;}
	avgA=sumA/3;
	avgB=sumB/3;
	avgC=sumC/3;
	avgD=sumD/3;

	Double_t ro_L1, e_ro_L1, Dm_v, eDm_v, avgDm_v, sumDm_v=0;
	v_idro>>ro_L1>>e_ro_L1;
	while(v_idro>>Dm_v>>eDm_v)
		sumDm_v+=Dm_v;	
	avgDm_v=sumDm_v/3;
	

	Double_t V_dir=(avgA*avgB*avgC)-(avgA*(M_PI*(pow(avgD, 2)/4)));
	Double_t eV_dir=(abs((avgB*avgC)-(M_PI*(pow(avgD, 2)/4)))*eA)+
		(abs(avgA*avgC)*eB)+(abs(avgA*avgB)*eC)+(abs((M_PI*avgD*avgA)/2)*eD);

	Double_t V_idro=(avgDm_v/ro_L1);
	Double_t eV_idro=(abs((Double_t)1/ro_L1)*eDm_v)+(abs(-(avgDm_v/pow(ro_L1, 2)))*e_ro_L1);

	Double_t check=abs(V_idro-V_dir)-(eV_idro+eV_dir);
	if(check>0)
		cout<<"MISURE DISPCREPANTI"<<endl;
	else
		cout<<"MISURE COMPATIBILI"<<endl;

	ofstream o("results.txt");
	// x=0 y ex=0 ey
	o<<"1 "<<V_dir<<" 0 "<<eV_dir<<endl<<"2 "<<V_idro<<" 0 "<<eV_idro<<endl;;

	TCanvas c("c", "", 800, 700);
	c.SetMargin(0.15, 0.10, 0.11, 0.10); // Set canvas size and borders	TCanvas c1()
	
	TGraphErrors g("results.txt");
	g.SetMarkerStyle(4);
	g.SetTitle("");
	g.GetYaxis()->SetTitle("Volume V (mm^{3})");
	g.GetXaxis()->SetNdivisions(2);
	g.Draw("ap");
}

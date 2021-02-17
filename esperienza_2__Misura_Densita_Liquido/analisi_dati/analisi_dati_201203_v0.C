/* bool comp(Double_t g1, Double_t eg1, Double_t g2, Double_t eg2){ */
/* 	bool r; */
/* 	Double_t check=abs(g1-g2)-(eg1+eg2); */
/* 	if(check<0) */
/* 		r=true; */
/* 	else */
/* 		r=false; */

/* 	return r; */
/* } */



/* void analisi_dati_201203_v0() */{
	ifstream v_dirette("../dati/v_dirette.txt");
	ifstream v_idro("../dati/v_idrostatico.txt");
	ifstream L2("../dati/L2_v_idrostatico.txt");
	ifstream LM("../dati/LM_v_idrostatico.txt");

	Double_t A, eA, B, eB, C, eC, D, eD, avgA, avgB, avgC, avgD, sumA=0, sumB=0, sumC=0, sumD=0;
	while(v_dirette>>A>>eA>>B>>eB>>C>>eC>>D>>eD){
		sumA+=A;
		sumB+=B;
		sumC+=C;
		sumD+=D;
	}
	avgA=sumA/3;
	avgB=sumB/3;
	avgC=sumC/3;
	avgD=sumD/3;

	//	cout<<avgA<<endl<<avgB<<endl<<avgC<<endl<<avgD<<endl;

	Double_t V_dir=(avgA*avgB*avgC)-(avgA*(M_PI*(pow(avgD, 2)/4)));
	Double_t eV_dir=(abs((avgB*avgC)-(M_PI*(pow(avgD, 2)/4)))*eA)+
		(abs(avgA*avgC)*eB)+(abs(avgA*avgB)*eC)+(abs((M_PI*avgD*avgA)/2)*eD);

	//	cout<<endl<<V_dir<<" +- "<<eV_dir<<endl;

	Double_t ro_L1, e_ro_L1, Dm_v, eDm_v, avgDm_v, sumDm_v=0;
	v_idro>>ro_L1>>e_ro_L1;
	
	while(v_idro>>Dm_v>>eDm_v)
		sumDm_v+=Dm_v;
	
	avgDm_v=sumDm_v/3;

	//	cout<<endl<<avgDm_v<<endl;
	
	Double_t V_idro=(avgDm_v/ro_L1);
	Double_t eV_idro=(abs((Double_t)1/ro_L1)*eDm_v)+(abs(-(avgDm_v/pow(ro_L1, 2)))*e_ro_L1);

	//	cout<<endl<<V_idro<<" +- "<<eV_idro<<endl;

    /* if(!comp(V_idro, eV_idro, V_dir, eV_dir)) */
	/* 	cout<<"misure discrepanti"<<endl; */
	/* else */
	/* 	cout<<"misure compatibili"<<endl; */
	
	TGraphErrors vg;
	
	vg.SetPoint(1, 1.0, V_dir);
	vg.SetPointError(1, 0, eV_dir);
	vg.SetPoint(2, 2.0, V_idro);
	vg.SetPointError(2, 0., eV_idro);

	vg.SetTitle("");
	vg.GetYaxis()->SetTitle("Volume (mm^{3})");
	vg.Draw("ap");

}

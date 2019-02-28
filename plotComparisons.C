/*
enum struct PlotTypeDef {
	RESOLUTION,
	NUMHITS,
	RADLENGTH
};

typedef struct Style {
	double MarkerSize, MarkerStyle, MarkerColor, MarkerAlpha;
} Style;
Style blStyles[3] = { {0.5, 21, kBlack, 1.0}, {0.5, 21, kBlue, 1.0}, {0.5, 21, kRed, 1.0} };
Style compStyles[3] = { {0.5, 21, kBlack, 0.5}, {0.5, 21, kBlue, 0.5}, {0.5, 21, kRed, 0.5} };
Style ratStyles[3] = { {0.5, 21, kBlack, 1.0}, {0.5, 21, kBlue, 1.0}, {0.5, 21, kRed, 1.0} };

typedef struct Config {
	double ratioRangeMin, ratioRangeMax;
	double plotRangeMin, plotRangeMax;
} Config;
Config configs;

void makeTopPad(TCanvas * canvas, TPad * pad1, PlotTypeDef type = PlotTypeDef::RESOLUTION) {
	canvas->cd();
	pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
	pad1->SetTopMargin(0.125);
	pad1->SetBottomMargin(0);
	pad1->SetGridx();
	pad1->SetGridy();
	if (type == PlotTypeDef::RESOLUTION)
		pad1->SetLogy();
	pad1->SetTicks();
	pad1->Draw();
	pad1->cd();
}

void makeBottomPad(TCanvas * canvas, TPad * pad2) {
	canvas->cd();
	pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
	pad2->SetTopMargin(0);
	pad2->SetBottomMargin(0.25);
	pad2->SetGridx();
	pad2->SetGridy();
	pad2->SetTicks();
	pad2->Draw();
	pad2->cd();
}

void composePlots(TCanvas * canvas, std::vector<TProfile*> blPlots, std::vector<TProfile*> compPlots, PlotTypeDef type = PlotTypeDef::RESOLUTION) {

	gStyle->SetTitleY(0.97);
	gStyle->SetTitleH(0.05);
	blPlots[0]->GetYaxis()->SetLabelSize(0);
	blPlots[0]->GetYaxis()->SetTitleSize(18);
	blPlots[0]->GetYaxis()->SetTitleFont(43);
	blPlots[0]->GetYaxis()->SetTitleOffset(1.0);
	blPlots[0]->SetMaximum(configs.plotRangeMax);
	blPlots[0]->SetMinimum(configs.plotRangeMin);

	for (int i = 0; i < blPlots.size(); i++) {
		blPlots[i]->SetMarkerSize(blStyles[i].MarkerSize);
		blPlots[i]->SetMarkerStyle(blStyles[i].MarkerStyle);
		blPlots[i]->SetLineColorAlpha(blStyles[i].MarkerColor, blStyles[i].MarkerAlpha);
		blPlots[i]->SetMarkerColorAlpha(blStyles[i].MarkerColor, blStyles[i].MarkerAlpha);

		compPlots[i]->SetMarkerSize(compStyles[i].MarkerSize);
		compPlots[i]->SetMarkerStyle(compStyles[i].MarkerStyle);
		compPlots[i]->SetLineColorAlpha(compStyles[i].MarkerColor, compStyles[i].MarkerAlpha);
		compPlots[i]->SetMarkerColorAlpha(compStyles[i].MarkerColor, compStyles[i].MarkerAlpha);
	}

	if (type == PlotTypeDef::RADLENGTH) {
		blPlots[0]->Draw("HIST");
		blPlots[0]->SetFillColorAlpha(kRed, 0.5);
	}
	else
		blPlots[0]->Draw();
	if (type == PlotTypeDef::RADLENGTH) {
		compPlots[0]->Draw("HIST SAME");
		compPlots[0]->SetFillColorAlpha(kBlue, 0.5);
	}
	else
		compPlots[0]->Draw("SAME");
	for (int i = 1; i < blPlots.size(); i++) {
		blPlots[i]->Draw("SAME");
		compPlots[i]->Draw("SAME");
	}

	TGaxis * axis;
	if (type == PlotTypeDef::RESOLUTION)
		axis = new TGaxis(0, configs.plotRangeMin, 0, configs.plotRangeMax, configs.plotRangeMin, configs.plotRangeMax, 510, "G");
	else if (type == PlotTypeDef::NUMHITS || type == PlotTypeDef::RADLENGTH)
		axis = new TGaxis(0, configs.plotRangeMin, 0, configs.plotRangeMax, configs.plotRangeMin, configs.plotRangeMax, 510, "");
	axis->SetLabelFont(43);
	axis->SetLabelSize(15);
	axis->Draw();

	if (type == PlotTypeDef::RESOLUTION) {
		TLegend * legend = new TLegend(0.13, 0.85, 0.25, 0.68);
		legend->AddEntry(blPlots[0], "1 GeV");
		legend->AddEntry(blPlots[1], "10 GeV");
		legend->AddEntry(blPlots[2], "100 GeV");
		// legend->AddEntry(hZDist90mm_10, "10 GeV, 90 mm");
		// legend->AddEntry(hBaseline_100, "100 GeV, 70 mm");
		// legend->AddEntry(hZDist90mm_100, "100 GeV, 90 mm");
		legend->Draw("SAME");
	}

	TPaveText * legendcomplement = new TPaveText(0.26, 0.75, 0.5, 0.85, "NDC");
	if (type == PlotTypeDef::RADLENGTH) {
		legendcomplement->AddText("Red: TBPX (IT460)");
		legendcomplement->AddText("Blue: TFPX (IT461)");
	}
	else {
		legendcomplement->AddText("Transparent points: TFPX (IT461)");
		legendcomplement->AddText("Solid points: TBPX (IT460)");
	}
	legendcomplement->Draw("SAME");

}

void composeRatioPlots(TCanvas * canvas, std::vector<TProfile*> blPlots, std::vector<TProfile*> compPlots) {

	std::vector<TProfile*> ratPlots;
	for (int i = 0; i < compPlots.size(); i++) {
		TProfile * ratio = (TProfile*)compPlots[i]->Clone(Form("ratio%d",i));
		ratio->Divide(blPlots[i]);
		ratPlots.push_back(ratio);
	}

	ratPlots[0]->SetMinimum(configs.ratioRangeMin);
	ratPlots[0]->SetMaximum(configs.ratioRangeMax);
	ratPlots[0]->SetTitle("");
	ratPlots[0]->SetStats(0);

	ratPlots[0]->GetYaxis()->SetTitle("TFPX/TBPX");
	ratPlots[0]->GetYaxis()->SetNdivisions(505);
	ratPlots[0]->GetYaxis()->SetTitleSize(18);
	ratPlots[0]->GetYaxis()->SetTitleFont(43);
	ratPlots[0]->GetYaxis()->SetTitleOffset(1.2);
	ratPlots[0]->GetYaxis()->SetLabelFont(43);
	ratPlots[0]->GetYaxis()->SetLabelSize(15);
	ratPlots[0]->GetYaxis()->CenterTitle();

	ratPlots[0]->GetXaxis()->SetTitle("#eta");
	ratPlots[0]->GetXaxis()->SetTitleSize(18);
	ratPlots[0]->GetXaxis()->SetTitleFont(43);
	ratPlots[0]->GetXaxis()->SetTitleOffset(3.0);
	ratPlots[0]->GetXaxis()->SetLabelFont(43);
	ratPlots[0]->GetXaxis()->SetLabelSize(15);

	for (int i = 0; i < ratPlots.size(); i++) {
		ratPlots[i]->SetMarkerSize(ratStyles[i].MarkerSize);
		ratPlots[i]->SetMarkerStyle(ratStyles[i].MarkerStyle);
		ratPlots[i]->SetMarkerColorAlpha(ratStyles[i].MarkerColor, ratStyles[i].MarkerAlpha);
		ratPlots[i]->SetDirectory(0);
	}

	ratPlots[0]->Draw("histp");
	for (int i = 1; i < ratPlots.size(); i++)
		ratPlots[i]->Draw("SAMEhistp");

}

void makePlot(TCanvas * newC, TPad * pad1, TPad * pad2, std::vector<TProfile*> blPlots, std::vector<TProfile*> compPlots, PlotTypeDef type = PlotTypeDef::RESOLUTION) {

	newC = new TCanvas();
	makeTopPad(newC, pad1, type);
	composePlots(newC, blPlots, compPlots, type);
	makeBottomPad(newC, pad2);
	composeRatioPlots(newC, blPlots, compPlots);
}
*/
void plotComparisons() {

	TH1::AddDirectory(kFALSE);

	//cout << "flag here "<<endl;
       
	std::vector<TProfile*> blPlots;
	std::vector<TProfile*> compPlots;

	TFile fComp("../Data/summary_OT614_200_IT440.root");
	TFile fBaseline("../Data/summary_OT614_200_IT440_base.root");
	TCanvas * newC;
	TPad * pad1; TPad * pad2;
	TProfile * hComp1, * hComp10, * hComp100;
	TProfile * hBl1, * hBl10, * hBl100;

	// Longitudinal impact parameter error
	hComp1 = (TProfile*)fComp.Get("Resolution_tracker_dzres_tracker_MS_Pt_z_vs_eta1tracker_profile");
	hComp10 = (TProfile*)fComp.Get("Resolution_tracker_dzres_tracker_MS_Pt_z_vs_eta10tracker_profile");
	hComp100 = (TProfile*)fComp.Get("Resolution_tracker_dzres_tracker_MS_Pt_z_vs_eta100tracker_profile");
	hBl1 = (TProfile*)fBaseline.Get("Resolution_tracker_dzres_tracker_MS_Pt_z_vs_eta1tracker_profile");
	hBl10 = (TProfile*)fBaseline.Get("Resolution_tracker_dzres_tracker_MS_Pt_z_vs_eta10tracker_profile");
	hBl100 = (TProfile*)fBaseline.Get("Resolution_tracker_dzres_tracker_MS_Pt_z_vs_eta100tracker_profile");
	/*
	configs = { 0.85, 1.03, 6, 1000 };

	blPlots = { hBl1, hBl10, hBl100};
	compPlots = { hComp1, hComp10, hComp100};
	makePlot(newC, pad1, pad2, blPlots, compPlots);

	// ctau resolution
	hComp1 = (TProfile*)fComp.Get("Resolution_tracker_lres_tracker_MS_Pt_l_vs_eta1tracker_profile");
	hComp10 = (TProfile*)fComp.Get("Resolution_tracker_lres_tracker_MS_Pt_l_vs_eta10tracker_profile");
	hComp100 = (TProfile*)fComp.Get("Resolution_tracker_lres_tracker_MS_Pt_l_vs_eta100tracker_profile");
	hBl1 = (TProfile*)fBaseline.Get("Resolution_tracker_lres_tracker_MS_Pt_l_vs_eta1tracker_profile");
	hBl10 = (TProfile*)fBaseline.Get("Resolution_tracker_lres_tracker_MS_Pt_l_vs_eta10tracker_profile");
	hBl100 = (TProfile*)fBaseline.Get("Resolution_tracker_lres_tracker_MS_Pt_l_vs_eta100tracker_profile");

	configs = { 0.9, 1.03, 3, 400 };

	blPlots = { hBl1, hBl10, hBl100};
	compPlots = { hComp1, hComp10, hComp100};
	makePlot(newC, pad1, pad2, blPlots, compPlots);

	// pt resolution
	hComp1 = (TProfile*)fComp.Get("Resolution_tracker_ptres_tracker_MS_Pt_pt_vs_eta1tracker_profile");
	hComp10 = (TProfile*)fComp.Get("Resolution_tracker_ptres_tracker_MS_Pt_pt_vs_eta10tracker_profile");
	hComp100 = (TProfile*)fComp.Get("Resolution_tracker_ptres_tracker_MS_Pt_pt_vs_eta100tracker_profile");
	hBl1 = (TProfile*)fBaseline.Get("Resolution_tracker_ptres_tracker_MS_Pt_pt_vs_eta1tracker_profile");
	hBl10 = (TProfile*)fBaseline.Get("Resolution_tracker_ptres_tracker_MS_Pt_pt_vs_eta10tracker_profile");
	hBl100 = (TProfile*)fBaseline.Get("Resolution_tracker_ptres_tracker_MS_Pt_pt_vs_eta100tracker_profile");

	configs = { 0.9, 1.11, 0.11, 100 };

	blPlots = { hBl1, hBl10, hBl100};
	compPlots = { hComp1, hComp10, hComp100};
	makePlot(newC, pad1, pad2, blPlots, compPlots);

	// Transverse impact parameter error
	hComp1 = (TProfile*)fComp.Get("Resolution_tracker_dxyres_tracker_MS_Pt_d_vs_eta1tracker_profile");
	hComp10 = (TProfile*)fComp.Get("Resolution_tracker_dxyres_tracker_MS_Pt_d_vs_eta10tracker_profile");
	hComp100 = (TProfile*)fComp.Get("Resolution_tracker_dxyres_tracker_MS_Pt_d_vs_eta100tracker_profile");
	hBl1 = (TProfile*)fBaseline.Get("Resolution_tracker_dxyres_tracker_MS_Pt_d_vs_eta1tracker_profile");
	hBl10 = (TProfile*)fBaseline.Get("Resolution_tracker_dxyres_tracker_MS_Pt_d_vs_eta10tracker_profile");
	hBl100 = (TProfile*)fBaseline.Get("Resolution_tracker_dxyres_tracker_MS_Pt_d_vs_eta100tracker_profile");

	configs = { 0.9, 1.03, 3, 300 };

	blPlots = { hBl1, hBl10, hBl100};
	compPlots = { hComp1, hComp10, hComp100};
	makePlot(newC, pad1, pad2, blPlots, compPlots);

	// Phi error
	hComp1 = (TProfile*)fComp.Get("Resolution_tracker_phires_tracker_MS_Pt_phi_vs_eta1tracker_profile");
	hComp10 = (TProfile*)fComp.Get("Resolution_tracker_phires_tracker_MS_Pt_phi_vs_eta10tracker_profile");
	hComp100 = (TProfile*)fComp.Get("Resolution_tracker_phires_tracker_MS_Pt_phi_vs_eta100tracker_profile");
	hBl1 = (TProfile*)fBaseline.Get("Resolution_tracker_phires_tracker_MS_Pt_phi_vs_eta1tracker_profile");
	hBl10 = (TProfile*)fBaseline.Get("Resolution_tracker_phires_tracker_MS_Pt_phi_vs_eta10tracker_profile");
	hBl100 = (TProfile*)fBaseline.Get("Resolution_tracker_phires_tracker_MS_Pt_phi_vs_eta100tracker_profile");

	configs = { 0.92, 1.03, 0.002, 3 };

	blPlots = { hBl1, hBl10, hBl100};
	compPlots = { hComp1, hComp10, hComp100};
	makePlot(newC, pad1, pad2, blPlots, compPlots);

	// Theta error
	hComp1 = (TProfile*)fComp.Get("Resolution_tracker_cotThetares_tracker_MS_Pt_ctgTheta_vs_eta1tracker_profile");
	hComp10 = (TProfile*)fComp.Get("Resolution_tracker_cotThetares_tracker_MS_Pt_ctgTheta_vs_eta10tracker_profile");
	hComp100 = (TProfile*)fComp.Get("Resolution_tracker_cotThetares_tracker_MS_Pt_ctgTheta_vs_eta100tracker_profile");
	hBl1 = (TProfile*)fBaseline.Get("Resolution_tracker_cotThetares_tracker_MS_Pt_ctgTheta_vs_eta1tracker_profile");
	hBl10 = (TProfile*)fBaseline.Get("Resolution_tracker_cotThetares_tracker_MS_Pt_ctgTheta_vs_eta10tracker_profile");
	hBl100 = (TProfile*)fBaseline.Get("Resolution_tracker_cotThetares_tracker_MS_Pt_ctgTheta_vs_eta100tracker_profile");

	configs = { 0.88, 1.03, 0.00005, 0.2 };

	blPlots = { hBl1, hBl10, hBl100};
	compPlots = { hComp1, hComp10, hComp100};
	makePlot(newC, pad1, pad2, blPlots, compPlots);

	// Number of hits in IT
	hComp1 = (TProfile*)fComp.Get("Geometry_pixel_img_totalEtaProfileSensors");
	hBl1 = (TProfile*)fBaseline.Get("Geometry_pixel_img_totalEtaProfileSensors");

	configs = { 0.97, 1.07, 0.05, 20 };

	blPlots = { hBl1 };
	compPlots = { hComp1 };
	makePlot(newC, pad1, pad2, blPlots, compPlots, PlotTypeDef::NUMHITS);

	// Number of hits in IT+OT
	hComp1 = (TProfile*)fComp.Get("Info_img_totalEtaProfileSensors_px");
	hBl1 = (TProfile*)fBaseline.Get("Info_img_totalEtaProfileSensors_px");

	configs = { 0.97, 1.03, 5, 27 };

	blPlots = { hBl1 };
	compPlots = { hComp1 };
	makePlot(newC, pad1, pad2, blPlots, compPlots, PlotTypeDef::NUMHITS);

	// Radiation length
	hComp1 = (TProfile*)fComp.Get("Material_pixel_matOverviewFull_rglobal_profile");
	hBl1 = (TProfile*)fBaseline.Get("Material_pixel_matOverviewFull_rglobal_profile");

	configs = { 0.7, 1.29, 0.05, 1.0 };

	blPlots = { hBl1 };
	compPlots = { hComp1 };
	makePlot(newC, pad1, pad2, blPlots, compPlots, PlotTypeDef::RADLENGTH);
	*/

}

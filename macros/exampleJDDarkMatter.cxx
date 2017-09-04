/*
 * JDDM.cc
 *
 *  Created on: 03/07/2017
 *  Authors: David Navarro Gironés 	<<david.navarrogir@e-campus.uab.cat>>
 *  		 Joaquim Palacio 		<<jpalacio@ifae.es>>
 *
 *  		 ADD A GENERAL DESCRIPTION ON THE CLASS, THE MAIN FUNCTIONS, THE VARIABLES
 *  		 AND MENTION THE "runExample#.C" THAT SHOWS HOW TO USE IT
 */

#include <Rtypes.h>
#include <TAttFill.h>
#include <TAttLine.h>
#include <TAttText.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TGraph.h>
#include <TH1.h>
#include <TLegend.h>
#include <TPave.h>
#include <TString.h>
#include <TVirtualPad.h>
#include <iostream>
#include <TStyle.h>


//this works home
#include "../source/JDDarkMatter.cc"
//#include "../source/JDInstrument.cc"
//#include "../source/JDOptimization.cc"

//#include </home/david/Work/Software/root_v6.06.08/include/TStyle.h>


//this works at IFAE
//#include </home/david/Work/Software/scripts/style.h>

using namespace std;

//-------------------------------------
//  new (QUIM)
void PrintListOfPossibilities()
{
	JDDarkMatter* 	JFactor = new JDDarkMatter();
	JFactor->GetListOfAuthors();
	JFactor->SetAuthor("Bonnivard");
	JFactor->GetListOfSources();
	JFactor->GetListOfCandidates();
	JFactor->GetUnits();
	JFactor->GetListOfConstructors();

}



//-------------------------------------
//  This function shows the JFactor of the author, source and candidate we want.
//	(QUIM) Declare author, source, candidate
void DrawJFactorFromReferences()
{

	TString author = "Bonnivard";
	TString source = "uma2";
	TString candidate = "Annihilation";
	TString mySourcePath = "/home/jpalacio/Work/eclipse/workspace/pic/DarkMatter/ObservationOptimization";

	JDDarkMatter* 	JFactor = new JDDarkMatter(author, source, candidate, mySourcePath);
	TF1* functionJFactor = JFactor->GetTF1JFactorVsTheta();

	functionJFactor->SetLineColor(2);
	functionJFactor->SetLineStyle(1);
	TCanvas* canvas = new TCanvas("canvas","",600,550);
	TH1I* dummy = new TH1I("dummy", JFactor->GetAuthor(),1,0.01,JFactor->GetThetaMax());
	dummy->SetMaximum(JFactor->GetJFactorMax()*10.);
	dummy->SetMinimum(JFactor->GetJFactorMin()*0.1);
	dummy->SetStats(0);
	dummy->SetXTitle(" #theta ");
	if (JFactor->GetCandidate() == "Annihilation")	{dummy->SetYTitle(" J Factor [GeV^{2}/cm^{5}]");}
	else if (JFactor->GetCandidate() == "Decay")	{dummy->SetYTitle(" J Factor [GeV/cm^{2}]");	}
	dummy->GetXaxis()->SetTitleOffset(1.3);
	dummy->GetYaxis()->SetTitleOffset(1.5);
	dummy->DrawCopy();
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionJFactor->Draw("same");

	// This is for plotting purposes only
	TString isBonnivard = (JFactor->GetIsBonnivard()? "arxiv:1504.02048" : "");
	TString isGeringer = (JFactor->GetIsGeringer()? "arxiv:1408.0002" : "");

	TLegend* leg=new TLegend(.18,.15,.88,.25);
	leg->AddEntry(functionJFactor, JFactor->GetSourceName()+" "+JFactor->GetCandidate()+" - "+isBonnivard+isGeringer, "l");
	leg->SetFillColor(0);
	leg->SetLineColor(1);
	leg->SetBorderSize(1);
	leg->SetTextSize(0.037);
	leg->Draw();
	gPad->Modified();
	gPad->Update();

//	canvas->Print("/home/david/Documents/DarkMatter/Resultats/JFactor/"+author+"_"+candidate+"_"+source+".png");
}

//-------------------------------------
//  This function shows the LOS of the author, source and candidate we want.
//	(QUIM) a definition of los should be given in relation to JFactor, probably written in here and specifying the units
//	(QUIM) most of the comments of ShowJFactor apply in here
void DrawLOSFromReferences()
{
	TString author = "Bonnivard";
	TString source = "uma2";
	TString candidate = "Annihilation";
	TString mySourcePath = "/home/jpalacio/Work/eclipse/workspace/pic/DarkMatter/ObservationOptimization";

	JDDarkMatter* 	JFactor = new JDDarkMatter(author, source, candidate, mySourcePath);
	TF1* functionLOS = JFactor->GetTF1LOSVsTheta();
	functionLOS->SetLineColor(2);
	functionLOS->SetLineStyle(1);

	TCanvas* canvas1 = new TCanvas("canvas1","",600,550);
	TH1I* dummy1 = new TH1I("dummy", JFactor->GetAuthor(),1,0.01,JFactor->GetThetaMax());
	dummy1->SetMaximum(3.e22);
	dummy1->SetMinimum(1.e17);
	dummy1->SetStats(0);
	dummy1->SetXTitle(" #theta "); //(QUIM) specify UNITS!!!
	if (JFactor->GetCandidate() == "Annihilation")	{dummy1->SetYTitle(" J Factor [GeV^{2}/cm^{5}]");}
	else if (JFactor->GetCandidate() == "Decay")	{dummy1->SetYTitle(" J Factor [GeV/cm^{2}]");	}
	dummy1->GetXaxis()->SetTitleOffset(1.3);
	dummy1->GetYaxis()->SetTitleOffset(1.5);
	dummy1->DrawCopy();
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionLOS->Draw("same");

	// This is for plotting purposes only
	TString isBonnivard = (JFactor->GetIsBonnivard()? "arxiv:1504.02048" : "");
	TString isGeringer = (JFactor->GetIsGeringer()? "arxiv:1408.0002" : "");

	TLegend* leg1=new TLegend(.18,.15,.88,.25);
	leg1->AddEntry(functionLOS, JFactor->GetSourceName()+" "+JFactor->GetCandidate()+" - "+isBonnivard+isGeringer, "l");
	leg1->SetFillColor(0);
	leg1->SetLineColor(1);
	leg1->SetBorderSize(1);
	leg1->SetTextSize(0.037);
	leg1->Draw();
	gPad->Modified();
	gPad->Update();

//	canvas1->Print("/home/david/Documents/DarkMatter/Resultats/LOS/"+author+"_"+candidate+"_"+source+".png");

}

//-------------------------------------
//  new (QUIM)
void DrawNormalizedLOSFromReferences()
{
	TString author = "Bonnivard";
	TString source = "uma2";
	TString candidate = "Annihilation";
	TString mySourcePath = "/home/jpalacio/Work/eclipse/workspace/pic/DarkMatter/ObservationOptimization";
	Double_t normTheta=0.1; // [deg]

	JDDarkMatter* 	JFactor = new JDDarkMatter(author, source, candidate, mySourcePath);
	TF1* functionLOS = JFactor->GetTF1NormLOSVsTheta(normTheta);
	functionLOS->SetLineColor(2);
	functionLOS->SetLineStyle(1);

	TCanvas* canvas1 = new TCanvas("canvas1","",600,550);
	TH1I* dummy1 = new TH1I("dummy", JFactor->GetAuthor(),1,0.01,JFactor->GetThetaMax());
	dummy1->SetMaximum(2e0);
	dummy1->SetMinimum(1.e-2);
	dummy1->SetStats(0);
	dummy1->SetXTitle(" #theta "); //(QUIM) specify UNITS!!!
	if (JFactor->GetCandidate() == "Annihilation")	{dummy1->SetYTitle(" J Factor [GeV^{2}/cm^{5}]");}
	else if (JFactor->GetCandidate() == "Decay")	{dummy1->SetYTitle(" J Factor [GeV/cm^{2}]");	}
	dummy1->GetXaxis()->SetTitleOffset(1.3);
	dummy1->GetYaxis()->SetTitleOffset(1.5);
	dummy1->DrawCopy();
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionLOS->Draw("same");

	// This is for plotting purposes only
	TString isBonnivard = (JFactor->GetIsBonnivard()? "arxiv:1504.02048" : "");
	TString isGeringer = (JFactor->GetIsGeringer()? "arxiv:1408.0002" : "");

	TLegend* leg1=new TLegend(.18,.15,.88,.25);
	leg1->AddEntry(functionLOS, JFactor->GetSourceName()+" "+JFactor->GetCandidate()+" - "+isBonnivard+isGeringer, "l");
	leg1->SetFillColor(0);
	leg1->SetLineColor(1);
	leg1->SetBorderSize(1);
	leg1->SetTextSize(0.037);
	leg1->Draw();
	gPad->Modified();
	gPad->Update();

//	canvas1->Print("/home/david/Documents/DarkMatter/Resultats/LOS/"+author+"_"+candidate+"_"+source+".png");

}

//-------------------------------------
//  This function ... (QUIM)
void DrawJFactorFromTGraph()
{
	//J-factor of TrigII from Saturni (from Michele Doro)
	TGraph* jfactor = new TGraph();
	// angle (deg) ; J(<angle) Gev2cm-5
	jfactor->SetPoint(0,  0.011,1.257e19);
	jfactor->SetPoint(1,  0.013, 1.772e19);
	jfactor->SetPoint(2,  0.016, 2.649e19);
	jfactor->SetPoint(3,  0.021, 4.045e19);
	jfactor->SetPoint(4,  0.026, 5.911e19);
	jfactor->SetPoint(5,  0.033, 8.540e19);
	jfactor->SetPoint(6,  0.041, 1.166e20);
	jfactor->SetPoint(7,  0.055, 1.786e20);
	jfactor->SetPoint(8,  0.070, 2.472e20);
	jfactor->SetPoint(9,  0.084, 3.014e20);
	jfactor->SetPoint(10, 0.101, 3.738e20);
	jfactor->SetPoint(11, 0.126, 4.851e20);
	jfactor->SetPoint(12, 0.160, 6.270e20);
	jfactor->SetPoint(13, 0.207, 8.321e20);
	jfactor->SetPoint(14, 0.262, 1.032e21);
	jfactor->SetPoint(15, 0.324, 1.145e21);
	jfactor->SetPoint(16, 0.427, 1.283e21);
	jfactor->SetPoint(17, 0.510, 1.305e21);
	jfactor->SetPoint(18, 0.620, 1.470e21);
	jfactor->SetPoint(19, 0.774, 1.569e21);
	jfactor->SetPoint(20, 0.949, 1.651e21);
	jfactor->SetPoint(21, 1.175, 1.856e21);
	jfactor->SetPoint(22, 1.484, 1.953e21);
	jfactor->SetPoint(23, 1.777, 1.986e21);
	jfactor->SetPoint(24, 1.983, 2.072e21);

	JDDarkMatter* 	JFactor = new JDDarkMatter((TGraph*)jfactor);
	JFactor->SetCandidate("Annihilation");
	JFactor->SetAuthor("Saturni-Doro");
	JFactor->SetSourceName("TrigII");
	TF1* functionJFactor = JFactor->GetTF1JFactorVsTheta();
	Double_t thetaMax = JFactor->GetThetaMax();

	functionJFactor->SetLineColor(2);
	functionJFactor->SetLineStyle(1);
	TCanvas* canvas = new TCanvas("canvas","",600,550);
	TH1I* dummy = new TH1I("dummy", JFactor->GetAuthor(),1,0.01,JFactor->GetThetaMax());
	dummy->SetMaximum(JFactor->GetJFactorMax()*10.);
	dummy->SetMinimum(JFactor->GetJFactorMin()*0.1);
	dummy->SetStats(0);
	dummy->SetXTitle(" #theta ");
	if (JFactor->GetCandidate() == "Annihilation")	{dummy->SetYTitle(" J Factor [GeV^{2}/cm^{5}]");}
	else if (JFactor->GetCandidate() == "Decay")	{dummy->SetYTitle(" J Factor [GeV/cm^{2}]");	}
	dummy->GetXaxis()->SetTitleOffset(1.3);
	dummy->GetYaxis()->SetTitleOffset(1.5);
	dummy->DrawCopy();
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionJFactor->Draw("same");

	TLegend* leg=new TLegend(.18,.15,.88,.25);
	leg->AddEntry(functionJFactor, JFactor->GetSourceName()+" "+JFactor->GetCandidate()+" ("+JFactor->GetAuthor()+")", "l");
	leg->SetFillColor(0);
	leg->SetLineColor(1);
	leg->SetBorderSize(1);
	leg->SetTextSize(0.037);
	leg->Draw();
	gPad->Modified();
	gPad->Update();

//	canvas->Print("/home/david/Documents/DarkMatter/Resultats/JFactor/"+author+"_"+candidate+"_"+source+".png");
}

//-------------------------------------
//  new (QUIM)
void DrawLOSFromTGraph()
{
	//J-factor of TrigII from Saturni (from Michele Doro)
	TGraph* jfactor = new TGraph();
	// angle (deg) ; J(<angle) Gev2cm-5
	jfactor->SetPoint(0,  0.011,1.257e19);
	jfactor->SetPoint(1,  0.013, 1.772e19);
	jfactor->SetPoint(2,  0.016, 2.649e19);
	jfactor->SetPoint(3,  0.021, 4.045e19);
	jfactor->SetPoint(4,  0.026, 5.911e19);
	jfactor->SetPoint(5,  0.033, 8.540e19);
	jfactor->SetPoint(6,  0.041, 1.166e20);
	jfactor->SetPoint(7,  0.055, 1.786e20);
	jfactor->SetPoint(8,  0.070, 2.472e20);
	jfactor->SetPoint(9,  0.084, 3.014e20);
	jfactor->SetPoint(10, 0.101, 3.738e20);
	jfactor->SetPoint(11, 0.126, 4.851e20);
	jfactor->SetPoint(12, 0.160, 6.270e20);
	jfactor->SetPoint(13, 0.207, 8.321e20);
	jfactor->SetPoint(14, 0.262, 1.032e21);
	jfactor->SetPoint(15, 0.324, 1.145e21);
	jfactor->SetPoint(16, 0.427, 1.283e21);
	jfactor->SetPoint(17, 0.510, 1.305e21);
	jfactor->SetPoint(18, 0.620, 1.470e21);
	jfactor->SetPoint(19, 0.774, 1.569e21);
	jfactor->SetPoint(20, 0.949, 1.651e21);
	jfactor->SetPoint(21, 1.175, 1.856e21);
	jfactor->SetPoint(22, 1.484, 1.953e21);
	jfactor->SetPoint(23, 1.777, 1.986e21);
	jfactor->SetPoint(24, 1.983, 2.072e21);

	JDDarkMatter* 	JFactor = new JDDarkMatter((TGraph*)jfactor);
	JFactor->SetCandidate("Annihilation");
	JFactor->SetAuthor("Saturni-Doro");
	JFactor->SetSourceName("TrigII");
	TF1* functionLOS = JFactor->GetTF1LOSVsTheta();
	Double_t Theta = JFactor->GetThetaMax();
	functionLOS->SetLineColor(2);
	functionLOS->SetLineStyle(1);

	TCanvas* canvas1 = new TCanvas("canvas1","",600,550);
	TH1I* dummy1 = new TH1I("dummy", JFactor->GetAuthor(),1,0.01,JFactor->GetThetaMax());
	dummy1->SetMaximum(1.e25);
	dummy1->SetMinimum(1.e20);
	dummy1->SetStats(0);
	dummy1->SetXTitle(" #theta "); //(QUIM) specify UNITS!!!
	if (JFactor->GetCandidate() == "Annihilation")	{dummy1->SetYTitle(" J Factor [GeV^{2}/cm^{5}]");}
	else if (JFactor->GetCandidate() == "Decay")	{dummy1->SetYTitle(" J Factor [GeV/cm^{2}]");	}
	dummy1->GetXaxis()->SetTitleOffset(1.3);
	dummy1->GetYaxis()->SetTitleOffset(1.5);
	dummy1->DrawCopy();
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionLOS->Draw("same");


	TLegend* leg1=new TLegend(.18,.15,.88,.25);
	leg1->AddEntry(functionLOS, JFactor->GetSourceName()+" "+JFactor->GetCandidate()+" ("+JFactor->GetAuthor()+")", "l");
	leg1->SetFillColor(0);
	leg1->SetLineColor(1);
	leg1->SetBorderSize(1);
	leg1->SetTextSize(0.037);
	leg1->Draw();
	gPad->Modified();
	gPad->Update();

//	canvas1->Print("/home/david/Documents/DarkMatter/Resultats/LOS/"+author+"_"+candidate+"_"+source+".png");

}

//-------------------------------------
//  new (QUIM)
void DrawNormLOSFromTGraph()
{
	//J-factor of TrigII from Saturni (from Michele Doro)
	TGraph* jfactor = new TGraph();
	// angle (deg) ; J(<angle) Gev2cm-5
	jfactor->SetPoint(0,  0.011,1.257e19);
	jfactor->SetPoint(1,  0.013, 1.772e19);
	jfactor->SetPoint(2,  0.016, 2.649e19);
	jfactor->SetPoint(3,  0.021, 4.045e19);
	jfactor->SetPoint(4,  0.026, 5.911e19);
	jfactor->SetPoint(5,  0.033, 8.540e19);
	jfactor->SetPoint(6,  0.041, 1.166e20);
	jfactor->SetPoint(7,  0.055, 1.786e20);
	jfactor->SetPoint(8,  0.070, 2.472e20);
	jfactor->SetPoint(9,  0.084, 3.014e20);
	jfactor->SetPoint(10, 0.101, 3.738e20);
	jfactor->SetPoint(11, 0.126, 4.851e20);
	jfactor->SetPoint(12, 0.160, 6.270e20);
	jfactor->SetPoint(13, 0.207, 8.321e20);
	jfactor->SetPoint(14, 0.262, 1.032e21);
	jfactor->SetPoint(15, 0.324, 1.145e21);
	jfactor->SetPoint(16, 0.427, 1.283e21);
	jfactor->SetPoint(17, 0.510, 1.305e21);
	jfactor->SetPoint(18, 0.620, 1.470e21);
	jfactor->SetPoint(19, 0.774, 1.569e21);
	jfactor->SetPoint(20, 0.949, 1.651e21);
	jfactor->SetPoint(21, 1.175, 1.856e21);
	jfactor->SetPoint(22, 1.484, 1.953e21);
	jfactor->SetPoint(23, 1.777, 1.986e21);
	jfactor->SetPoint(24, 1.983, 2.072e21);

	JDDarkMatter* 	JFactor = new JDDarkMatter((TGraph*)jfactor);
	JFactor->SetCandidate("Annihilation");
	JFactor->SetAuthor("Saturni-Doro");
	JFactor->SetSourceName("TrigII");
	Double_t normTheta=0.05; // [deg]
	TF1* functionLOS = JFactor->GetTF1NormLOSVsTheta(normTheta);
	Double_t Theta = JFactor->GetThetaMax();
	functionLOS->SetLineColor(2);
	functionLOS->SetLineStyle(1);

	TCanvas* canvas1 = new TCanvas("canvas1","",600,550);
	TH1I* dummy1 = new TH1I("dummy", JFactor->GetAuthor(),1,0.01,JFactor->GetThetaMax());
	dummy1->SetMaximum(2.e0);
	dummy1->SetMinimum(1.e-2);
	dummy1->SetStats(0);
	dummy1->SetXTitle(" #theta "); //(QUIM) specify UNITS!!!
	if (JFactor->GetCandidate() == "Annihilation")	{dummy1->SetYTitle(" J Factor [GeV^{2}/cm^{5}]");}
	else if (JFactor->GetCandidate() == "Decay")	{dummy1->SetYTitle(" J Factor [GeV/cm^{2}]");	}
	dummy1->GetXaxis()->SetTitleOffset(1.3);
	dummy1->GetYaxis()->SetTitleOffset(1.5);
	dummy1->DrawCopy();
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionLOS->Draw("same");

	// This for the Donut MC
	Double_t thetaMin=0.05; 					// [deg]
	Double_t thetaMax=JFactor->GetThetaMax();	// [deg]
	for(Int_t i=0; i<100; i++)
	{
		Double_t theta = thetaMin + (thetaMax-thetaMin)/100.*i;
		cout << theta << " " << functionLOS->Eval(theta) << endl;
	}


	TLegend* leg1=new TLegend(.18,.15,.88,.25);
	leg1->AddEntry(functionLOS, JFactor->GetSourceName()+" "+JFactor->GetCandidate()+" ("+JFactor->GetAuthor()+")", "l");
	leg1->SetFillColor(0);
	leg1->SetLineColor(1);
	leg1->SetBorderSize(1);
	leg1->SetTextSize(0.037);
	leg1->Draw();
	gPad->Modified();
	gPad->Update();

	canvas1->Print("~/Desktop/normalizedLOS.pdf");
	canvas1->Print("~/Desktop/normalizedLOS.root");

}

//-------------------------------------
//  This function ... (QUIM)
void DrawJFactorFromTxtFile()
{

	TString txtFile = "/home/jpalacio/Work/eclipse/workspace/pic/DarkMatter/ObservationOptimization/references/JFactor/JFactorSegue.txt";
	JDDarkMatter* 	JFactor = new JDDarkMatter(txtFile);
	JFactor->SetCandidate("Annihilation");
	JFactor->SetSourceName("Segue");
	TF1* functionJFactor = JFactor->GetTF1JFactorVsTheta();
	TF1* functionJFactorFromLOS = JFactor->GetTF1JFactorFromLOSVsTheta();
	functionJFactorFromLOS->SetLineColor(1);
	functionJFactorFromLOS->SetLineStyle(2);


	Double_t thetaMax = JFactor->GetThetaMax();

	functionJFactor->SetLineColor(2);
	functionJFactor->SetLineStyle(1);
	TCanvas* canvas = new TCanvas("canvas","",600,550);
	TH1I* dummy = new TH1I("dummy", JFactor->GetAuthor(),1,0.01,JFactor->GetThetaMax());
	dummy->SetMaximum(JFactor->GetJFactorMax()*10.);
	dummy->SetMinimum(JFactor->GetJFactorMax()*0.01);
	dummy->SetStats(0);
	dummy->SetXTitle(" #theta ");
	if (JFactor->GetCandidate() == "Annihilation")	{dummy->SetYTitle(" J Factor [GeV^{2}/cm^{5}]");}
	else if (JFactor->GetCandidate() == "Decay")	{dummy->SetYTitle(" J Factor [GeV/cm^{2}]");	}
	dummy->GetXaxis()->SetTitleOffset(1.3);
	dummy->GetYaxis()->SetTitleOffset(1.5);
	dummy->DrawCopy();
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionJFactor->Draw("same");
	functionJFactorFromLOS->Draw("same");

	TLegend* leg=new TLegend(.18,.15,.88,.25);
	leg->AddEntry(functionJFactor, JFactor->GetSourceName()+" "+JFactor->GetCandidate()+" ("+JFactor->GetAuthor()+")", "l");
	leg->SetFillColor(0);
	leg->SetLineColor(1);
	leg->SetBorderSize(1);
	leg->SetTextSize(0.037);
	leg->Draw();
	gPad->Modified();
	gPad->Update();

//	canvas->Print("/home/david/Documents/DarkMatter/Resultats/JFactor/"+author+"_"+candidate+"_"+source+".png");
}

//-------------------------------------
//  This function shows the JFactor of the author, source and candidate we want.
//	(QUIM) Declare author, source, candidate
void DrawJFactorFromLOS()
{

	TString author = "Bonnivard";
	TString source = "uma2";
	TString candidate = "Annihilation";
	TString mySourcePath = "/home/jpalacio/Work/eclipse/workspace/pic/DarkMatter/ObservationOptimization";

	JDDarkMatter* 	JFactor = new JDDarkMatter(author, source, candidate, mySourcePath);
	TF1* functionJFactorFromLOS = JFactor->GetTF1JFactorFromLOSVsTheta();

	TF1* functionJFactor = JFactor->GetTF1JFactorVsTheta();
	functionJFactor->SetLineColor(2);
	functionJFactor->SetLineStyle(1);
	TCanvas* canvas = new TCanvas("canvas","",600,550);
	TH1I* dummy = new TH1I("dummy", JFactor->GetAuthor(),1,0.01,JFactor->GetThetaMax());
	dummy->SetMaximum(JFactor->GetJFactorMax()*100.);
	dummy->SetMinimum(JFactor->GetJFactorMin()*0.1);
	dummy->SetStats(0);
	dummy->SetXTitle(" #theta ");
	if (JFactor->GetCandidate() == "Annihilation")	{dummy->SetYTitle(" J Factor [GeV^{2}/cm^{5}]");}
	else if (JFactor->GetCandidate() == "Decay")	{dummy->SetYTitle(" J Factor [GeV/cm^{2}]");	}
	dummy->GetXaxis()->SetTitleOffset(1.3);
	dummy->GetYaxis()->SetTitleOffset(1.5);
	dummy->DrawCopy();
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionJFactorFromLOS->SetLineColor(1);
	functionJFactorFromLOS->SetLineStyle(2);
	functionJFactor->Draw("same");
	functionJFactorFromLOS->Draw("same");
	// This is for plotting purposes only
	TString isBonnivard = (JFactor->GetIsBonnivard()? "arxiv:1504.02048" : "");
	TString isGeringer = (JFactor->GetIsGeringer()? "arxiv:1408.0002" : "");

	TLegend* leg=new TLegend(.18,.15,.88,.25);
	leg->AddEntry(functionJFactor, JFactor->GetSourceName()+" "+JFactor->GetCandidate()+" - "+isBonnivard+isGeringer, "l");
	leg->AddEntry(functionJFactorFromLOS, "(idem) from LOS", "l");
	leg->SetFillColor(0);
	leg->SetLineColor(1);
	leg->SetBorderSize(1);
	leg->SetTextSize(0.037);
	leg->Draw();
	gPad->Modified();
	gPad->Update();

}



void exampleJDDarkMatter()
{
//	PrintListOfPossibilities();

//	DrawJFactorFromReferences();
//	DrawLOSFromReferences();	// (QUIM) this is useful for the DonutMC
//	DrawNormalizedLOSFromReferences();

//	DrawJFactorFromTGraph();
//	DrawLOSFromTGraph();
//	DrawNormLOSFromTGraph();

//	DrawJFactorFromTxtFile();

	DrawJFactorFromLOS();


}

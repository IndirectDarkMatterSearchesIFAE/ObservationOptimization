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

#include "../source/JDInstrument.cc"

using namespace std;

//-------------------------------------
//  new (QUIM)
void PrintListOfPossibilities()
{
	JDInstrument* Epsilon = new JDInstrument();
	Epsilon->GetListOfInstruments();
	Epsilon->GetUnits();
	Epsilon->GetListOfConstructors();

}

//-------------------------------------
// new (QUIM)
void PlotCameraAcceptanceFromIDEAL()
{

	TString instrumentName= "IDEAL";
	Double_t distanceCameraCenterMax=5;	// [deg]
	Double_t wobbleDist=0.;	// [deg]

	JDInstrument* Epsilon = new JDInstrument(distanceCameraCenterMax,wobbleDist, instrumentName);
	TF1* functionCameraAcceptance = Epsilon->GetTF1EpsilonVsTheta();
	Double_t distCamCenterMax = Epsilon->GetDistCameraCenterMax();

	// This is for plotting purposes only
	TString isIDEAL = (Epsilon->GetIsIdeal()? "IDEAL" : "");

	TLegend* leg=new TLegend(.18,.75,.80,.85);
	leg->AddEntry(functionCameraAcceptance, isIDEAL+" (w="+Form("%2.1f",Epsilon->GetWobbleDistance())+" [deg])" , "l");
	leg->SetFillColor(0);
	leg->SetLineColor(1);
	leg->SetBorderSize(1);
	leg->SetTextSize(0.037);

	TCanvas* canvas = new TCanvas("canvas","",600,550);
	TH1I* dummy = new TH1I("dummy", "Camera Acceptance",1,0.,distCamCenterMax);
	dummy->SetMaximum(1.5);
	dummy->SetMinimum(0.);
	dummy->SetStats(0);
	dummy->SetXTitle(" distCamCenter [deg]");
	dummy->SetYTitle(" Camera Acceptance [%]");
	dummy->GetXaxis()->SetTitleOffset(1.3);
	dummy->GetYaxis()->SetTitleOffset(1.5);
	dummy->DrawCopy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionCameraAcceptance->Draw("same");

	TF2* functionCameraAcceptanceVsThetaAndPhi = Epsilon->GetTF2EpsilonVsThetaAndPhi();

	TCanvas* canvas1 = new TCanvas("canvas1","",600,550);
	TH2I* dummy1 = new TH2I("dummy1", "Camera Acceptance",1,0.,distCamCenterMax, 1, 0., 2*TMath::Pi());
	dummy1->SetMaximum(1.5);
	dummy1->SetMinimum(0.);
	dummy1->SetStats(0);
	dummy1->SetXTitle(" #Theta [deg]");
	dummy1->SetYTitle(" #Phi [rad]");
	dummy1->GetXaxis()->SetTitleOffset(1.3);
	dummy1->GetYaxis()->SetTitleOffset(1.3);
	dummy1->DrawCopy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionCameraAcceptanceVsThetaAndPhi->Draw("colz same");
	leg->Draw();
	gPad->Modified();
	gPad->Update();

	TF2* functionCameraAcceptanceVsXAndY = Epsilon->GetTF2EpsilonVsXAndY();

	TCanvas* canvas2 = new TCanvas("canvas2","",600,550);
	TH2I* dummy2 = new TH2I("dummy2", "Camera Acceptance",1,-distCamCenterMax,distCamCenterMax, 1, -distCamCenterMax,distCamCenterMax);
	dummy2->SetMaximum(1.5);
	dummy2->SetMinimum(0.);
	dummy2->SetStats(0);
	dummy2->SetXTitle(" #Theta_{x} [deg]");
	dummy2->SetYTitle(" #Theta_{y} [deg]");
	dummy2->GetXaxis()->SetTitleOffset(1.3);
	dummy2->GetYaxis()->SetTitleOffset(1.3);
	dummy2->DrawCopy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionCameraAcceptanceVsXAndY->Draw("colz same");
	leg->Draw();
	gPad->Modified();
	gPad->Update();

}

//-------------------------------------
// new (QUIM)
void PlotEfficiencyFromIDEAL()
{

	TString instrumentName= "IDEAL";
	Double_t distanceCameraCenterMax=5;	// [deg]
	Double_t wobbleDist=1.;	// [deg]

	JDInstrument* Epsilon = new JDInstrument(distanceCameraCenterMax,wobbleDist, instrumentName);
	TF1* functionEfficientyVsTheta = Epsilon->GetTF1EfficiencyVsTheta(0.);
	Double_t distCamCenterMax = Epsilon->GetDistCameraCenterMax();

	// This is for plotting purposes only
	TString isIDEAL = (Epsilon->GetIsIdeal()? "IDEAL" : "");

	TLegend* leg=new TLegend(.18,.75,.80,.85);
	leg->AddEntry(functionEfficientyVsTheta, isIDEAL+" (w="+Form("%2.1f",Epsilon->GetWobbleDistance())+" [deg])" , "l");
	leg->SetFillColor(0);
	leg->SetLineColor(1);
	leg->SetBorderSize(1);
	leg->SetTextSize(0.037);

	TCanvas* canvas = new TCanvas("canvas","",600,550);
	TH1I* dummy = new TH1I("dummy", "Efficiency",1,0.,distCamCenterMax);
	dummy->SetMaximum(1.5);
	dummy->SetMinimum(0.);
	dummy->SetStats(0);
	dummy->SetXTitle(" #Theta [deg]");
	dummy->SetYTitle(" Efficiency [%]");
	dummy->GetXaxis()->SetTitleOffset(1.3);
	dummy->GetYaxis()->SetTitleOffset(1.5);
	dummy->DrawCopy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionEfficientyVsTheta->Draw("same");
	leg->Draw();
	gPad->Modified();
	gPad->Update();
}

void PlotCameraAcceptanceFromMAGIC()
{

	TString instrumentName= "MAGICPointLike";
	Double_t wobble=0.4;	// [deg]
	TString myInstrumentPath = "/home/david/Work/eclipse/workspace/TFG/ObservationOptimization";

	JDInstrument* Epsilon = new JDInstrument(instrumentName, wobble, myInstrumentPath);
	TF1* functionCameraAcceptance = Epsilon->GetTF1EpsilonVsTheta();
	Double_t distCamCenterMax = Epsilon->GetDistCameraCenterMax();

	// This is for plotting purposes only
	TString isMAGIC = (Epsilon->GetIsMAGIC()? "arxiv:1409.5594" : "");
	TLegend* leg=new TLegend(.18,.75,.80,.85);
	leg->AddEntry(functionCameraAcceptance, Form("MAGICPointLike (w=%2.1f [deg])",Epsilon->GetWobbleDistance()),"l");
	leg->AddEntry(functionCameraAcceptance, "("+isMAGIC+")" , "");
	leg->SetFillColor(0);
	leg->SetLineColor(1);
	leg->SetBorderSize(1);
	leg->SetTextSize(0.037);

	TCanvas* canvas = new TCanvas("canvas","",600,550);
	TH1I* dummy = new TH1I("dummy", "Camera Acceptance",1,0.,distCamCenterMax);
	dummy->SetMaximum(1.5);
	dummy->SetMinimum(0.);
	dummy->SetStats(0);
	dummy->SetXTitle(" distCamCenter [deg]");
	dummy->SetYTitle(" Camera Acceptance [%]");
	dummy->GetXaxis()->SetTitleOffset(1.3);
	dummy->GetYaxis()->SetTitleOffset(1.5);
	dummy->DrawCopy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionCameraAcceptance->Draw("same");
	leg->Draw();
	gPad->Modified();
	gPad->Update();

	TF2* functionCameraAcceptanceVsThetaAndPhi = Epsilon->GetTF2EpsilonVsThetaAndPhi();
	// This is for plotting purposes only

	TCanvas* canvas1 = new TCanvas("canvas1","",600,550);
	TH2I* dummy1 = new TH2I("dummy1", "Camera Acceptance",1,0.,distCamCenterMax, 1, -TMath::Pi(), TMath::Pi());
	dummy1->SetMaximum(1.5);
	dummy1->SetMinimum(0.);
	dummy1->SetStats(0);
	dummy1->SetXTitle(" #Theta [deg]");
	dummy1->SetYTitle(" #Phi [rad]");
	dummy1->GetXaxis()->SetTitleOffset(1.3);
	dummy1->GetYaxis()->SetTitleOffset(1.3);
	dummy1->DrawCopy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionCameraAcceptanceVsThetaAndPhi->Draw("colz same");
	leg->Draw();
	gPad->Modified();
	gPad->Update();

	TF2* functionCameraAcceptanceVsXAndY = Epsilon->GetTF2EpsilonVsXAndY();

	TCanvas* canvas2 = new TCanvas("canvas2","",600,550);
	TH2I* dummy2 = new TH2I("dummy2", "Camera Acceptance",1,-distCamCenterMax,distCamCenterMax, 1, -distCamCenterMax,distCamCenterMax);
	dummy2->SetMaximum(1.5);
	dummy2->SetMinimum(0.);
	dummy2->SetStats(0);
	dummy2->SetXTitle(" #Theta_{x} [deg]");
	dummy2->SetYTitle(" #Theta_{y} [deg]");
	dummy2->GetXaxis()->SetTitleOffset(1.3);
	dummy2->GetYaxis()->SetTitleOffset(1.3);
	dummy2->DrawCopy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionCameraAcceptanceVsXAndY->Draw("colz same");
	leg->Draw();
	gPad->Modified();
	gPad->Update();

}

//-------------------------------------
// new (QUIM)
void PlotEfficiencyFromMAGIC()
{

	TString instrumentName= "MAGICPointLike";
	Double_t wobble=0.4;	// [deg]
	TString myInstrumentPath = "/home/jpalacio/Work/eclipse/workspace/pic/DarkMatter/ObservationOptimization";

	JDInstrument* Epsilon = new JDInstrument(instrumentName, wobble, myInstrumentPath);
	TF1* functionEfficientyVsTheta = Epsilon->GetTF1EfficiencyVsTheta();
	Double_t distCamCenterMax = Epsilon->GetDistCameraCenterMax();

	// This is for plotting purposes only
	TString isMAGIC = (Epsilon->GetIsMAGIC()? "MAGICPointLike-arxiv:1409.5594" : "");

	TLegend* leg=new TLegend(.18,.75,.80,.85);
	leg->AddEntry(functionEfficientyVsTheta, Form("MAGICPointLike (w=%2.1f [deg])",Epsilon->GetWobbleDistance()),"l");
	leg->AddEntry(functionEfficientyVsTheta, "("+isMAGIC+")" , "");

	leg->SetFillColor(0);
	leg->SetLineColor(1);
	leg->SetBorderSize(1);
	leg->SetTextSize(0.037);

	TCanvas* canvas = new TCanvas("canvas","",600,550);
	TH1I* dummy = new TH1I("dummy", "Efficiency",1,0.,distCamCenterMax);
	dummy->SetMaximum(1.5);
	dummy->SetMinimum(0.);
	dummy->SetStats(0);
	dummy->SetXTitle(" #Theta [deg]");
	dummy->SetYTitle(" Efficiency [%]");
	dummy->GetXaxis()->SetTitleOffset(1.3);
	dummy->GetYaxis()->SetTitleOffset(1.5);
	dummy->DrawCopy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionEfficientyVsTheta->Draw("same");
	leg->Draw();
	gPad->Modified();
	gPad->Update();
}

//-------------------------------------
// new (QUIM)
void PlotCameraAcceptanceFromTGraph()
{

	//IACT acceptance for test
	TGraph* cameraAcceptance = new TGraph();
	// angle [deg] ; Acceptance [%]
	cameraAcceptance->SetPoint(0,0.2005571030640669, 3.9132208316997907/3.9132208316997907);
	cameraAcceptance->SetPoint(1,0.350974930362117, 3.9132208316997907/3.9132208316997907);
	cameraAcceptance->SetPoint(2,0.3992571959145774, 3.9132208316997907/3.9132208316997907);
	cameraAcceptance->SetPoint(3,0.7000928505106778, 2.1264653896144985/3.9132208316997907);
	cameraAcceptance->SetPoint(4,1.0027855153203342, 1.0411956886637337/3.9132208316997907);
	cameraAcceptance->SetPoint(5,1.4020427112349119, 0.5398071884460788/3.9132208316997907);
	cameraAcceptance->SetPoint(6,1.8012999071494895, 0.12097386953624159/3.9132208316997907);

	TString instrumentName= "TEST";
	Double_t wobble=0.4;	// [deg]

	JDInstrument* Epsilon = new JDInstrument(cameraAcceptance,wobble);
	Epsilon->SetInstrumentName(instrumentName);
	TF1* functionCameraAcceptance = Epsilon->GetTF1EpsilonVsTheta();
	Double_t distCamCenterMax = Epsilon->GetDistCameraCenterMax();

	TLegend* leg=new TLegend(.18,.75,.80,.85);
	leg->AddEntry(functionCameraAcceptance, Epsilon->GetInstrumentName()+Form("(w=%2.1f [deg])",Epsilon->GetWobbleDistance()),"l");
	leg->SetFillColor(0);
	leg->SetLineColor(1);
	leg->SetBorderSize(1);
	leg->SetTextSize(0.037);

	TCanvas* canvas = new TCanvas("canvas","",600,550);
	TH1I* dummy = new TH1I("dummy", "Camera Acceptance",1,0.,distCamCenterMax);
	dummy->SetMaximum(1.5);
	dummy->SetMinimum(0.);
	dummy->SetStats(0);
	dummy->SetXTitle(" distCamCenter [deg]");
	dummy->SetYTitle(" Camera Acceptance [%]");
	dummy->GetXaxis()->SetTitleOffset(1.3);
	dummy->GetYaxis()->SetTitleOffset(1.5);
	dummy->DrawCopy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionCameraAcceptance->Draw("same");
	leg->Draw();
	gPad->Modified();
	gPad->Update();

	TF2* functionCameraAcceptanceVsThetaAndPhi = Epsilon->GetTF2EpsilonVsThetaAndPhi();

	TCanvas* canvas1 = new TCanvas("canvas1","",600,550);
	TH2I* dummy1 = new TH2I("dummy1", "Camera Acceptance",1,0.,distCamCenterMax, 1, -TMath::Pi(), TMath::Pi());
	dummy1->SetMaximum(1.5);
	dummy1->SetMinimum(0.);
	dummy1->SetStats(0);
	dummy1->SetXTitle(" #Theta [deg]");
	dummy1->SetYTitle(" #Phi [rad]");
	dummy1->GetXaxis()->SetTitleOffset(1.3);
	dummy1->GetYaxis()->SetTitleOffset(1.3);
	dummy1->DrawCopy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionCameraAcceptanceVsThetaAndPhi->Draw("colz same");
	leg->Draw();
	gPad->Modified();
	gPad->Update();

	TF2* functionCameraAcceptanceVsXAndY = Epsilon->GetTF2EpsilonVsXAndY();

	TCanvas* canvas2 = new TCanvas("canvas2","",600,550);
	TH2I* dummy2 = new TH2I("dummy2", "Camera Acceptance",1,-distCamCenterMax,distCamCenterMax, 1, -distCamCenterMax,distCamCenterMax);
	dummy2->SetMaximum(1.5);
	dummy2->SetMinimum(0.);
	dummy2->SetStats(0);
	dummy2->SetXTitle(" #Theta_{x} [deg]");
	dummy2->SetYTitle(" #Theta_{y} [deg]");
	dummy2->GetXaxis()->SetTitleOffset(1.3);
	dummy2->GetYaxis()->SetTitleOffset(1.3);
	dummy2->DrawCopy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionCameraAcceptanceVsXAndY->Draw("colz same");
	leg->Draw();
	gPad->Modified();
	gPad->Update();

}

//-------------------------------------
// new (QUIM)
void PlotEfficiencyFromTGraph()
{
	//IACT acceptance for test
	TGraph* cameraAcceptance = new TGraph();
	// angle [deg] ; Acceptance [%]
	cameraAcceptance->SetPoint(0,0.2005571030640669, 3.9132208316997907/3.9132208316997907);
	cameraAcceptance->SetPoint(1,0.350974930362117, 3.9132208316997907/3.9132208316997907);
	cameraAcceptance->SetPoint(2,0.3992571959145774, 3.9132208316997907/3.9132208316997907);
	cameraAcceptance->SetPoint(3,0.7000928505106778, 2.1264653896144985/3.9132208316997907);
	cameraAcceptance->SetPoint(4,1.0027855153203342, 1.0411956886637337/3.9132208316997907);
	cameraAcceptance->SetPoint(5,1.4020427112349119, 0.5398071884460788/3.9132208316997907);
	cameraAcceptance->SetPoint(6,1.8012999071494895, 0.12097386953624159/3.9132208316997907);

	TString instrumentName= "TEST";
	Double_t wobble=0.4;	// [deg]

	JDInstrument* Epsilon = new JDInstrument(cameraAcceptance,wobble);
	Epsilon->SetInstrumentName(instrumentName);
	TF1* functionEfficientyVsTheta = Epsilon->GetTF1EfficiencyVsTheta();
	Double_t distCamCenterMax = Epsilon->GetDistCameraCenterMax();

	// This is for plotting purposes only
	TString isMAGIC = (Epsilon->GetIsMAGIC()? "MAGICPointLike-arxiv:1409.5594" : "");

	TLegend* leg=new TLegend(.18,.75,.80,.85);
	leg->AddEntry(functionEfficientyVsTheta, Epsilon->GetInstrumentName()+Form("(w=%2.1f [deg])",Epsilon->GetWobbleDistance()),"l");
	leg->SetFillColor(0);
	leg->SetLineColor(1);
	leg->SetBorderSize(1);
	leg->SetTextSize(0.037);

	TCanvas* canvas = new TCanvas("canvas","",600,550);
	TH1I* dummy = new TH1I("dummy", "Efficiency",1,0.,distCamCenterMax);
	dummy->SetMaximum(1.5);
	dummy->SetMinimum(0.);
	dummy->SetStats(0);
	dummy->SetXTitle(" #Theta [deg]");
	dummy->SetYTitle(" Efficiency [%]");
	dummy->GetXaxis()->SetTitleOffset(1.3);
	dummy->GetYaxis()->SetTitleOffset(1.5);
	dummy->DrawCopy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionEfficientyVsTheta->Draw("same");
	leg->Draw();
	gPad->Modified();
	gPad->Update();
}

void PlotCameraAcceptanceFromTxtFile()
{
	TString txtFile = "/home/david/Work/eclipse/workspace/TFG/ObservationOptimization/references/JFactor/JFactorSegue.txt";

	TString instrumentName= "TEST";
	Double_t wobble=0.4;	// [deg]

	JDInstrument* Epsilon = new JDInstrument(txtFile,wobble);
	Epsilon->SetInstrumentName(instrumentName);
	TF1* functionCameraAcceptance = Epsilon->GetTF1EpsilonVsTheta();
	Double_t distCamCenterMax = Epsilon->GetDistCameraCenterMax();

	TLegend* leg=new TLegend(.18,.75,.80,.85);
	leg->AddEntry(functionCameraAcceptance, Epsilon->GetInstrumentName()+Form("(w=%2.1f [deg])",Epsilon->GetWobbleDistance()),"l");
	leg->SetFillColor(0);
	leg->SetLineColor(1);
	leg->SetBorderSize(1);
	leg->SetTextSize(0.037);

	TCanvas* canvas = new TCanvas("canvas","",600,550);
	TH1I* dummy = new TH1I("dummy", "Camera Acceptance",1,0.,distCamCenterMax);
	dummy->SetMaximum(1.5);
	dummy->SetMinimum(0.);
	dummy->SetStats(0);
	dummy->SetXTitle(" distCamCenter [deg]");
	dummy->SetYTitle(" Camera Acceptance [%]");
	dummy->GetXaxis()->SetTitleOffset(1.3);
	dummy->GetYaxis()->SetTitleOffset(1.5);
	dummy->DrawCopy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionCameraAcceptance->Draw("same");
	leg->Draw();
	gPad->Modified();
	gPad->Update();

	TF2* functionCameraAcceptanceVsThetaAndPhi = Epsilon->GetTF2EpsilonVsThetaAndPhi();

	TCanvas* canvas1 = new TCanvas("canvas1","",600,550);
	TH2I* dummy1 = new TH2I("dummy1", "Camera Acceptance",1,0.,distCamCenterMax, 1, -TMath::Pi(), TMath::Pi());
	dummy1->SetMaximum(1.5);
	dummy1->SetMinimum(0.);
	dummy1->SetStats(0);
	dummy1->SetXTitle(" #Theta [deg]");
	dummy1->SetYTitle(" #Phi [rad]");
	dummy1->GetXaxis()->SetTitleOffset(1.3);
	dummy1->GetYaxis()->SetTitleOffset(1.3);
	dummy1->DrawCopy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionCameraAcceptanceVsThetaAndPhi->Draw("colz same");
	leg->Draw();
	gPad->Modified();
	gPad->Update();

	TF2* functionCameraAcceptanceVsXAndY = Epsilon->GetTF2EpsilonVsXAndY();

	TCanvas* canvas2 = new TCanvas("canvas2","",600,550);
	TH2I* dummy2 = new TH2I("dummy2", "Camera Acceptance",1,-distCamCenterMax,distCamCenterMax, 1, -distCamCenterMax,distCamCenterMax);
	dummy2->SetMaximum(1.5);
	dummy2->SetMinimum(0.);
	dummy2->SetStats(0);
	dummy2->SetXTitle(" #Theta_{x} [deg]");
	dummy2->SetYTitle(" #Theta_{y} [deg]");
	dummy2->GetXaxis()->SetTitleOffset(1.3);
	dummy2->GetYaxis()->SetTitleOffset(1.3);
	dummy2->DrawCopy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionCameraAcceptanceVsXAndY->Draw("colz same");
	leg->Draw();
	gPad->Modified();
	gPad->Update();

}

void 	PlotEfficiencyFromTxtFile()
{
	TString txtFile = "/home/david/Work/eclipse/workspace/TFG/ObservationOptimization/references/JFactor/JFactorSegue.txt";

	TString instrumentName= "TEST";
	Double_t wobble=0.4;	// [deg]

	JDInstrument* Epsilon = new JDInstrument(txtFile,wobble);
	Epsilon->SetInstrumentName(instrumentName);
	TF1* functionEfficientyVsTheta = Epsilon->GetTF1EfficiencyVsTheta();
	Double_t distCamCenterMax = Epsilon->GetDistCameraCenterMax();

	// This is for plotting purposes only
	TString isMAGIC = (Epsilon->GetIsMAGIC()? "MAGICPointLike-arxiv:1409.5594" : "");

	TLegend* leg=new TLegend(.18,.75,.80,.85);
	leg->AddEntry(functionEfficientyVsTheta, Epsilon->GetInstrumentName()+Form("(w=%2.1f [deg])",Epsilon->GetWobbleDistance()),"l");
	leg->SetFillColor(0);
	leg->SetLineColor(1);
	leg->SetBorderSize(1);
	leg->SetTextSize(0.037);

	TCanvas* canvas = new TCanvas("canvas","",600,550);
	TH1I* dummy = new TH1I("dummy", "Efficiency",1,0.,distCamCenterMax);
	dummy->SetMaximum(1.5);
	dummy->SetMinimum(0.);
	dummy->SetStats(0);
	dummy->SetXTitle(" #Theta [deg]");
	dummy->SetYTitle(" Efficiency [%]");
	dummy->GetXaxis()->SetTitleOffset(1.3);
	dummy->GetYaxis()->SetTitleOffset(1.5);
	dummy->DrawCopy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionEfficientyVsTheta->Draw("same");
	leg->Draw();
	gPad->Modified();
	gPad->Update();
}
void exampleJDInstrument()
{
//	PrintListOfPossibilities();

//	PlotCameraAcceptanceFromIDEAL();
	PlotEfficiencyFromIDEAL();

//	PlotCameraAcceptanceFromMAGIC();
//	PlotEfficiencyFromMAGIC();

//	PlotCameraAcceptanceFromCTA(); 	// (QUIM)
//	PlotEfficiencyFromCTA();		// (QUIM)

//	PlotCameraAcceptanceFromTGraph();
//	PlotEfficiencyFromTGraph();

//	PlotCameraAcceptanceFromTxtFile();
//	PlotEfficiencyFromTxtFile();
}

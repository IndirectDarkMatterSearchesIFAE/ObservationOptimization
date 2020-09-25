/*
 *
 *  Created on: 03/07/2017
 *  Las modification on: 02/12/2017
 *
 *  Authors: David Navarro Gironés 	<<david.navarro.girones@gmail.com>>
 *  		 Joaquim Palacio 		<<jpalacio@ifae.es>>
 *
 *
 *  		 This is a tutorial on the main features of the class JDOptimization
 */

//#include "../source/JDAstroProfile.cc"
#include "../source/JDDarkMatter.cc"
#include "../source/JDInstrument.cc"
#include "../source/JDOptimization.cc"

#include <TStyle.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TF1.h>


using namespace std;

// General path
//TString myInstrumentPath = "/Users/palacio/Work/Software/ObservationOptimization/ObservationOptimization";
//TString mySourcePath = "/Users/palacio/Work/Software/ObservationOptimization/ObservationOptimization";


//-------------------------------------
//  Draw QFactor (without instrumental effects) as a function of the integration angle (theta [deg])
//
//  TString author      	-> Name of the author to use as reference: Geringer-Sameth (2014), Bonnivard (2016);
//  TString source			-> Name of the dSph galaxy to use;
//  TString candidate; 		-> Type of DM candidate: Annihilation or Decay
//  TString instrumentName 	-> Instrument name
//  Double_t wobble			-> Wobble distance [deg]
void PlotQ0Factor()
{
	TString myInstrumentPath = "/Users/palacio/Work/Software/ObservationOptimization/ObservationOptimization";
	TString mySourcePath = "/Users/palacio/Work/Software/ObservationOptimization/ObservationOptimization";
	cout << mySourcePath << endl;
	TString author = "Bonnivard";
	TString source = "uma2";
	TString candidate = "Decay";
	TString instrumentName= "MAGICPointLike";
	Double_t distanceCameraCenterMax=5;	// [deg]
	Double_t wobbleDist=1.;	// [deg]
	Double_t normalizationPoint=8.8136e+19;

	JDOptimization* QFactor = new JDOptimization(author, source, candidate, mySourcePath, myInstrumentPath, instrumentName, distanceCameraCenterMax, wobbleDist);
	TF1* functionQFactorVsTheta= QFactor->GetTF1QFactorVsTheta(0,0.4);

	Double_t thetaMax = QFactor->GetThetaMax();
	Double_t distCameraCenterMax = QFactor->GetDistCameraCenterMax();

	functionQFactorVsTheta->SetLineColor(2);
	functionQFactorVsTheta->SetLineStyle(1);

//	// This is for plotting purposes only
	TString isBonnivard = (QFactor->GetIsBonnivard()? "arxiv:1504.02048" : "");
	TString isGeringer = (QFactor->GetIsGeringer()? "arxiv:1408.0002" : "");
	TString isIDEAL = (QFactor->GetIsIdeal()? "IDEAL" : "");

	TLegend* leg=new TLegend(.18,.15,.88,.25);
	leg->AddEntry(functionQFactorVsTheta, QFactor->GetSourceName()+" - "+isIDEAL+" - "+isBonnivard+isGeringer, "l");
	leg->SetFillColor(0);
	leg->SetLineColor(1);
	leg->SetBorderSize(1);
	leg->SetTextSize(0.037);

	TCanvas* canvas2 = new TCanvas("canvas2","",600,550);
	TH1I* dummy1 = new TH1I("dummy1","" ,1,1e-2,thetaMax);
	dummy1->SetMaximum(3.);
	dummy1->SetMinimum(0.);
	dummy1->SetStats(0);
	dummy1->SetXTitle(" #theta [deg]");
	if(QFactor->GetCandidate()=="Annihilation"){	dummy1->SetYTitle(" QFactor_{Ann} ");}
	else if(QFactor->GetCandidate()=="Decay"){	dummy1->SetYTitle(" QFactor_{Dec} ");}
	dummy1->GetXaxis()->SetTitleOffset(1.3);
	dummy1->GetYaxis()->SetTitleOffset(1.5);
	dummy1->DrawCopy();
//	gPad->SetLogx();
//	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionQFactorVsTheta->Draw("same");
	leg->Draw();
	gPad->Modified();
	gPad->Update();

}

//-------------------------------------
//  Draw QFactor (wit Leakage effect) as a function of the integration angle (theta [deg])
//
//  TString author      	-> Name of the author to use as reference: Geringer-Sameth (2014), Bonnivard (2016);
//  TString source			-> Name of the dSph galaxy to use;
//  TString candidate; 		-> Type of DM candidate: Annihilation or Decay
//  TString instrumentName 	-> Instrument name
//  Double_t wobble			-> Wobble distance [deg]
void PlotQ1Factor()
{
	TString myInstrumentPath = "/Users/palacio/Work/Software/ObservationOptimization/ObservationOptimization";
	TString mySourcePath = "/Users/palacio/Work/Software/ObservationOptimization/ObservationOptimization";
	TString author = "Bonnivard";
	TString source = "uma2";
	TString candidate = "Decay";
	TString instrumentName= "MAGICPointLike";
	Double_t distanceCameraCenterMax=5;	// [deg]
	Double_t wobbleDist=3.;	// [deg]
	Double_t normalizationPoint=8.8136e+19;

	JDOptimization* QFactor = new JDOptimization(author, source, candidate, mySourcePath, myInstrumentPath, instrumentName, distanceCameraCenterMax, wobbleDist);
	TF1* functionQFactorVsTheta= QFactor->GetTF1QFactorVsTheta(1,0.4);

	Double_t thetaMax = QFactor->GetThetaMax();
	Double_t distCameraCenterMax = QFactor->GetDistCameraCenterMax();

	functionQFactorVsTheta->SetLineColor(2);
	functionQFactorVsTheta->SetLineStyle(1);

//	// This is for plotting purposes only
	TString isBonnivard = (QFactor->GetIsBonnivard()? "arxiv:1504.02048" : "");
	TString isGeringer = (QFactor->GetIsGeringer()? "arxiv:1408.0002" : "");
	TString isIDEAL = (QFactor->GetIsIdeal()? "IDEAL" : "");

	TLegend* leg=new TLegend(.18,.15,.88,.25);
	leg->AddEntry(functionQFactorVsTheta, QFactor->GetSourceName()+" - "+isIDEAL+" - "+isBonnivard+isGeringer, "l");
	leg->SetFillColor(0);
	leg->SetLineColor(1);
	leg->SetBorderSize(1);
	leg->SetTextSize(0.037);

	TCanvas* canvas2 = new TCanvas("canvas2","",600,550);
	TH1I* dummy1 = new TH1I("dummy1","" ,1,1e-2,thetaMax);
	dummy1->SetMaximum(3.);
	dummy1->SetMinimum(0.);
	dummy1->SetStats(0);
	dummy1->SetXTitle(" #theta [deg]");
	if(QFactor->GetCandidate()=="Annihilation"){	dummy1->SetYTitle(" QFactor_{Ann} ");}
	else if(QFactor->GetCandidate()=="Decay"){	dummy1->SetYTitle(" QFactor_{Dec} ");}
	dummy1->GetXaxis()->SetTitleOffset(1.3);
	dummy1->GetYaxis()->SetTitleOffset(1.5);
	dummy1->DrawCopy();
//	gPad->SetLogx();
//	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionQFactorVsTheta->Draw("same");
	leg->Draw();
	gPad->Modified();
	gPad->Update();

}

//-------------------------------------
//  Draw QFactor (wit Uncertainties effect) as a function of the integration angle (theta [deg])
//
//  TString author      	-> Name of the author to use as reference: Geringer-Sameth (2014), Bonnivard (2016);
//  TString source			-> Name of the dSph galaxy to use;
//  TString candidate; 		-> Type of DM candidate: Annihilation or Decay
//  TString instrumentName 	-> Instrument name
//  Double_t wobble			-> Wobble distance [deg]
void PlotQ2Factor()
{
	TString myInstrumentPath = "/Users/palacio/Work/Software/ObservationOptimization/ObservationOptimization";
	TString mySourcePath = "/Users/palacio/Work/Software/ObservationOptimization/ObservationOptimization";
	TString author = "Bonnivard";
	TString source = "uma2";
	TString candidate = "Decay";
	TString instrumentName= "MAGICPointLike";
	Double_t distanceCameraCenterMax=5;	// [deg]
	Double_t wobbleDist=3.;	// [deg]
	Double_t normalizationPoint=8.8136e+19;

	JDOptimization* QFactor = new JDOptimization(author, source, candidate, mySourcePath, myInstrumentPath, instrumentName, distanceCameraCenterMax, wobbleDist);
	TF1* functionQFactorVsTheta= QFactor->GetTF1QFactorVsTheta(2,0.4);

	Double_t thetaMax = QFactor->GetThetaMax();
	Double_t distCameraCenterMax = QFactor->GetDistCameraCenterMax();

	functionQFactorVsTheta->SetLineColor(2);
	functionQFactorVsTheta->SetLineStyle(1);

//	// This is for plotting purposes only
	TString isBonnivard = (QFactor->GetIsBonnivard()? "arxiv:1504.02048" : "");
	TString isGeringer = (QFactor->GetIsGeringer()? "arxiv:1408.0002" : "");
	TString isIDEAL = (QFactor->GetIsIdeal()? "IDEAL" : "");

	TLegend* leg=new TLegend(.18,.15,.88,.25);
	leg->AddEntry(functionQFactorVsTheta, QFactor->GetSourceName()+" - "+isIDEAL+" - "+isBonnivard+isGeringer, "l");
	leg->SetFillColor(0);
	leg->SetLineColor(1);
	leg->SetBorderSize(1);
	leg->SetTextSize(0.037);

	TCanvas* canvas2 = new TCanvas("canvas2","",600,550);
	TH1I* dummy1 = new TH1I("dummy1","" ,1,1e-2,thetaMax);
	dummy1->SetMaximum(3.);
	dummy1->SetMinimum(0.);
	dummy1->SetStats(0);
	dummy1->SetXTitle(" #theta [deg]");
	if(QFactor->GetCandidate()=="Annihilation"){	dummy1->SetYTitle(" QFactor_{Ann} ");}
	else if(QFactor->GetCandidate()=="Decay"){	dummy1->SetYTitle(" QFactor_{Dec} ");}
	dummy1->GetXaxis()->SetTitleOffset(1.3);
	dummy1->GetYaxis()->SetTitleOffset(1.5);
	dummy1->DrawCopy();
//	gPad->SetLogx();
//	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionQFactorVsTheta->Draw("same");
	leg->Draw();
	gPad->Modified();
	gPad->Update();

}

//-------------------------------------
//  Draw QFactor (wit Acceptance effect) as a function of the integration angle (theta [deg])
//
//  TString author      	-> Name of the author to use as reference: Geringer-Sameth (2014), Bonnivard (2016);
//  TString source			-> Name of the dSph galaxy to use;
//  TString candidate; 		-> Type of DM candidate: Annihilation or Decay
//  TString instrumentName 	-> Instrument name
//  Double_t wobble			-> Wobble distance [deg]
void PlotQ3Factor()
{
	TString myInstrumentPath = "/Users/palacio/Work/Software/ObservationOptimization/ObservationOptimization";
	TString mySourcePath = "/Users/palacio/Work/Software/ObservationOptimization/ObservationOptimization";
	TString author = "Bonnivard";
	TString source = "uma2";
	TString candidate = "Decay";
	TString instrumentName= "IDEAL";
	Double_t distanceCameraCenterMax=5;	// [deg]
	Double_t wobbleDist=0.4;	// [deg]
	Double_t normalizationPoint=8.8136e+19;

	JDOptimization* QFactor = new JDOptimization(author, source, candidate, mySourcePath, myInstrumentPath, instrumentName, distanceCameraCenterMax, wobbleDist);
	TF1* functionQFactorVsTheta= QFactor->GetTF1QFactorVsTheta(3,0.4);

	Double_t thetaMax = QFactor->GetThetaMax();
	Double_t distCameraCenterMax = QFactor->GetDistCameraCenterMax();

	functionQFactorVsTheta->SetLineColor(2);
	functionQFactorVsTheta->SetLineStyle(1);

//	// This is for plotting purposes only
	TString isBonnivard = (QFactor->GetIsBonnivard()? "arxiv:1504.02048" : "");
	TString isGeringer = (QFactor->GetIsGeringer()? "arxiv:1408.0002" : "");
	TString isIDEAL = (QFactor->GetIsIdeal()? "IDEAL" : "");

	TLegend* leg=new TLegend(.18,.15,.88,.25);
	leg->AddEntry(functionQFactorVsTheta, QFactor->GetSourceName()+" - "+isIDEAL+" - "+isBonnivard+isGeringer, "l");
	leg->SetFillColor(0);
	leg->SetLineColor(1);
	leg->SetBorderSize(1);
	leg->SetTextSize(0.037);

	TCanvas* canvas2 = new TCanvas("canvas2","",600,550);
	TH1I* dummy1 = new TH1I("dummy1","" ,1,1e-2,thetaMax);
	dummy1->SetMaximum(3.);
	dummy1->SetMinimum(0.);
	dummy1->SetStats(0);
	dummy1->SetXTitle(" #theta [deg]");
	if(QFactor->GetCandidate()=="Annihilation"){	dummy1->SetYTitle(" QFactor_{Ann} ");}
	else if(QFactor->GetCandidate()=="Decay"){	dummy1->SetYTitle(" QFactor_{Dec} ");}
	dummy1->GetXaxis()->SetTitleOffset(1.3);
	dummy1->GetYaxis()->SetTitleOffset(1.5);
	dummy1->DrawCopy();
//	gPad->SetLogx();
//	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionQFactorVsTheta->Draw("same");
	leg->Draw();
	gPad->Modified();
	gPad->Update();

}

////-------------------------------------
////  Draw QFactor (wit Leakage+Uncertanties effect) as a function of the integration angle (theta [deg])
////
////  TString author      	-> Name of the author to use as reference: Geringer-Sameth (2014), Bonnivard (2016);
////  TString source			-> Name of the dSph galaxy to use;
////  TString candidate; 		-> Type of DM candidate: Annihilation or Decay
////  TString instrumentName 	-> Instrument name
////  Double_t wobble			-> Wobble distance [deg]
//void PlotQ12Factor()
//{
//	TString author = "Bonnivard";
//	TString source = "uma2";
//	TString candidate = "Decay";
//	TString instrumentName= "MAGICPointLike";
//	Double_t distanceCameraCenterMax=5;	// [deg]
//	Double_t wobbleDist=1.;	// [deg]
//	Double_t normalizationPoint=8.8136e+19;
//
//	JDOptimization* QFactor = new JDOptimization(author, source, candidate, mySourcePath, myInstrumentPath, instrumentName, distanceCameraCenterMax, wobbleDist);
//	TF1* functionQFactorVsTheta= QFactor->GetTF1QFactorVsTheta(12,0.4);
//	TF1* functionEfficiency = QFactor->GetTF1EfficiencyVsTheta();
//
//	Double_t thetaMax = QFactor->GetThetaMax();
//	Double_t distCameraCenterMax = QFactor->GetDistCameraCenterMax();
//
//	functionQFactorVsTheta->SetLineColor(2);
//	functionQFactorVsTheta->SetLineStyle(1);
//
////	// This is for plotting purposes only
//	TString isBonnivard = (QFactor->GetIsBonnivard()? "arxiv:1504.02048" : "");
//	TString isGeringer = (QFactor->GetIsGeringer()? "arxiv:1408.0002" : "");
//	TString isIDEAL = (QFactor->GetIsIdeal()? "IDEAL" : "");
//
//	TLegend* leg=new TLegend(.18,.15,.88,.25);
//	leg->AddEntry(functionQFactorVsTheta, QFactor->GetSourceName()+" - "+isIDEAL+" - "+isBonnivard+isGeringer, "l");
//	leg->SetFillColor(0);
//	leg->SetLineColor(1);
//	leg->SetBorderSize(1);
//	leg->SetTextSize(0.037);
//
//	TCanvas* canvas2 = new TCanvas("canvas2","",600,550);
//	TH1I* dummy1 = new TH1I("dummy1","" ,1,1e-2,thetaMax);
//	dummy1->SetMaximum(3.);
//	dummy1->SetMinimum(0.);
//	dummy1->SetStats(0);
//	dummy1->SetXTitle(" #theta [deg]");
//	if(QFactor->GetCandidate()=="Annihilation"){	dummy1->SetYTitle(" QFactor_{Ann} ");}
//	else if(QFactor->GetCandidate()=="Decay"){	dummy1->SetYTitle(" QFactor_{Dec} ");}
//	dummy1->GetXaxis()->SetTitleOffset(1.3);
//	dummy1->GetYaxis()->SetTitleOffset(1.5);
//	dummy1->DrawCopy();
////	gPad->SetLogx();
////	gPad->SetLogy();
//	gPad->SetGridy();
//	gPad->SetGridx();
//	functionQFactorVsTheta->Draw("same");
//	leg->Draw();
//	gPad->Modified();
//	gPad->Update();
//
//}
//
////-------------------------------------
////  Draw QFactor (wit Leakage+Acceptance effect) as a function of the integration angle (theta [deg])
////
////  TString author      	-> Name of the author to use as reference: Geringer-Sameth (2014), Bonnivard (2016);
////  TString source			-> Name of the dSph galaxy to use;
////  TString candidate; 		-> Type of DM candidate: Annihilation or Decay
////  TString instrumentName 	-> Instrument name
////  Double_t wobble			-> Wobble distance [deg]
//void PlotQ13Factor()
//{
//	TString author = "Bonnivard";
//	TString source = "uma2";
//	TString candidate = "Decay";
//	TString instrumentName= "IDEAL";
//	Double_t distanceCameraCenterMax=10;	// [deg]
//	Double_t wobbleDist=1.;	// [deg]
//	Double_t normalizationPoint=8.8136e+19;
//
//	JDOptimization* QFactor = new JDOptimization(author, source, candidate, mySourcePath, myInstrumentPath, instrumentName, distanceCameraCenterMax, wobbleDist);
//	TF1* functionQFactorVsTheta= QFactor->GetTF1QFactorVsTheta(13,0.4);
//	TF1* functionEfficiency = QFactor->GetTF1EfficiencyVsTheta();
//
//	Double_t thetaMax = QFactor->GetThetaMax();
//	Double_t distCameraCenterMax = QFactor->GetDistCameraCenterMax();
//
//	functionQFactorVsTheta->SetLineColor(2);
//	functionQFactorVsTheta->SetLineStyle(1);
//
////	// This is for plotting purposes only
//	TString isBonnivard = (QFactor->GetIsBonnivard()? "arxiv:1504.02048" : "");
//	TString isGeringer = (QFactor->GetIsGeringer()? "arxiv:1408.0002" : "");
//	TString isIDEAL = (QFactor->GetIsIdeal()? "IDEAL" : "");
//
//	TLegend* leg=new TLegend(.18,.15,.88,.25);
//	leg->AddEntry(functionQFactorVsTheta, QFactor->GetSourceName()+" - "+isIDEAL+" - "+isBonnivard+isGeringer, "l");
//	leg->SetFillColor(0);
//	leg->SetLineColor(1);
//	leg->SetBorderSize(1);
//	leg->SetTextSize(0.037);
//
//	TCanvas* canvas2 = new TCanvas("canvas2","",600,550);
//	TH1I* dummy1 = new TH1I("dummy1","" ,1,1e-2,thetaMax);
////	dummy1->SetMaximum(3.);
////	dummy1->SetMinimum(0.);
//	dummy1->SetMaximum(1.e22);
//	dummy1->SetMinimum(1.e15);
//	dummy1->SetStats(0);
//	dummy1->SetXTitle(" #theta [deg]");
//	if(QFactor->GetCandidate()=="Annihilation"){	dummy1->SetYTitle(" QFactor_{Ann} ");}
//	else if(QFactor->GetCandidate()=="Decay"){	dummy1->SetYTitle(" QFactor_{Dec} ");}
//	dummy1->GetXaxis()->SetTitleOffset(1.3);
//	dummy1->GetYaxis()->SetTitleOffset(1.5);
//	dummy1->DrawCopy();
//	gPad->SetLogx();
//	gPad->SetLogy();
//	gPad->SetGridy();
//	gPad->SetGridx();
//	functionQFactorVsTheta->Draw("");
//	leg->Draw();
//	gPad->Modified();
//	gPad->Update();
//
//}
//
////-------------------------------------
////  Draw QFactor (wit Uncertainties+Acceptance effect) as a function of the integration angle (theta [deg])
////
////  TString author      	-> Name of the author to use as reference: Geringer-Sameth (2014), Bonnivard (2016);
////  TString source			-> Name of the dSph galaxy to use;
////  TString candidate; 		-> Type of DM candidate: Annihilation or Decay
////  TString instrumentName 	-> Instrument name
////  Double_t wobble			-> Wobble distance [deg]
//void PlotQ23Factor()
//{
//	TString author = "Bonnivard";
//	TString source = "uma2";
//	TString candidate = "Decay";
//	TString instrumentName= "MAGICPointLike";
//	Double_t distanceCameraCenterMax=5;	// [deg]
//	Double_t wobbleDist=0.;	// [deg]
//	Double_t normalizationPoint=8.8136e+19;
//
//	JDOptimization* QFactor = new JDOptimization(author, source, candidate, mySourcePath, myInstrumentPath, instrumentName, distanceCameraCenterMax, wobbleDist);
//	TF1* functionQFactorVsTheta= QFactor->GetTF1QFactorVsTheta(23,0.4);
//	TF1* functionEfficiency = QFactor->GetTF1EfficiencyVsTheta();
//
//	Double_t thetaMax = QFactor->GetThetaMax();
//	Double_t distCameraCenterMax = QFactor->GetDistCameraCenterMax();
//
//	functionQFactorVsTheta->SetLineColor(2);
//	functionQFactorVsTheta->SetLineStyle(1);
//
////	// This is for plotting purposes only
//	TString isBonnivard = (QFactor->GetIsBonnivard()? "arxiv:1504.02048" : "");
//	TString isGeringer = (QFactor->GetIsGeringer()? "arxiv:1408.0002" : "");
//	TString isIDEAL = (QFactor->GetIsIdeal()? "IDEAL" : "");
//
//	TLegend* leg=new TLegend(.18,.15,.88,.25);
//	leg->AddEntry(functionQFactorVsTheta, QFactor->GetSourceName()+" - "+isIDEAL+" - "+isBonnivard+isGeringer, "l");
//	leg->SetFillColor(0);
//	leg->SetLineColor(1);
//	leg->SetBorderSize(1);
//	leg->SetTextSize(0.037);
//
//	TCanvas* canvas2 = new TCanvas("canvas2","",600,550);
//	TH1I* dummy1 = new TH1I("dummy1","" ,1,1e-2,thetaMax);
//	dummy1->SetMaximum(3.);
//	dummy1->SetMinimum(0.);
//	dummy1->SetStats(0);
//	dummy1->SetXTitle(" #theta [deg]");
//	if(QFactor->GetCandidate()=="Annihilation"){	dummy1->SetYTitle(" QFactor_{Ann} ");}
//	else if(QFactor->GetCandidate()=="Decay"){	dummy1->SetYTitle(" QFactor_{Dec} ");}
//	dummy1->GetXaxis()->SetTitleOffset(1.3);
//	dummy1->GetYaxis()->SetTitleOffset(1.5);
//	dummy1->DrawCopy();
////	gPad->SetLogx();
////	gPad->SetLogy();
//	gPad->SetGridy();
//	gPad->SetGridx();
//	functionQFactorVsTheta->Draw("same");
//	leg->Draw();
//	gPad->Modified();
//	gPad->Update();
//
//}
//
////-------------------------------------
////  Draw QFactor (wit Acceptance+Uncertainties+Leakage effect) as a function of the integration angle (theta [deg])
////
////  TString author      	-> Name of the author to use as reference: Geringer-Sameth (2014), Bonnivard (2016);
////  TString source			-> Name of the dSph galaxy to use;
////  TString candidate; 		-> Type of DM candidate: Annihilation or Decay
////  TString instrumentName 	-> Instrument name
////  Double_t wobble			-> Wobble distance [deg]
//void PlotQ123Factor()
//{
//	TString author = "Bonnivard";
//	TString source = "uma2";
//	TString candidate = "Decay";
//	TString instrumentName= "IDEAL";
//	Double_t distanceCameraCenterMax=5;	// [deg]
//	Double_t wobbleDist=0.;	// [deg]
//	Double_t normalizationPoint=8.8136e+19;
//
//	JDOptimization* QFactor = new JDOptimization(author, source, candidate, mySourcePath, myInstrumentPath, instrumentName, distanceCameraCenterMax, wobbleDist);
//	TF1* functionQFactorVsTheta= QFactor->GetTF1QFactorVsTheta(123,0.4);
//	TF1* functionEfficiency = QFactor->GetTF1EfficiencyVsTheta();
//
//	Double_t thetaMax = QFactor->GetThetaMax();
//	Double_t distCameraCenterMax = QFactor->GetDistCameraCenterMax();
//
//	functionQFactorVsTheta->SetLineColor(2);
//	functionQFactorVsTheta->SetLineStyle(1);
//
////	// This is for plotting purposes only
//	TString isBonnivard = (QFactor->GetIsBonnivard()? "arxiv:1504.02048" : "");
//	TString isGeringer = (QFactor->GetIsGeringer()? "arxiv:1408.0002" : "");
//	TString isIDEAL = (QFactor->GetIsIdeal()? "IDEAL" : "");
//
//	TLegend* leg=new TLegend(.18,.15,.88,.25);
//	leg->AddEntry(functionQFactorVsTheta, QFactor->GetSourceName()+" - "+isIDEAL+" - "+isBonnivard+isGeringer, "l");
//	leg->SetFillColor(0);
//	leg->SetLineColor(1);
//	leg->SetBorderSize(1);
//	leg->SetTextSize(0.037);
//
//	TCanvas* canvas2 = new TCanvas("canvas2","",600,550);
//	TH1I* dummy1 = new TH1I("dummy1","" ,1,1e-2,thetaMax);
//	dummy1->SetMaximum(3.);
//	dummy1->SetMinimum(0.);
//	dummy1->SetStats(0);
//	dummy1->SetXTitle(" #theta [deg]");
//	if(QFactor->GetCandidate()=="Annihilation"){	dummy1->SetYTitle(" QFactor_{Ann} ");}
//	else if(QFactor->GetCandidate()=="Decay"){	dummy1->SetYTitle(" QFactor_{Dec} ");}
//	dummy1->GetXaxis()->SetTitleOffset(1.3);
//	dummy1->GetYaxis()->SetTitleOffset(1.5);
//	dummy1->DrawCopy();
////	gPad->SetLogx();
////	gPad->SetLogy();
//	gPad->SetGridy();
//	gPad->SetGridx();
//	functionQFactorVsTheta->Draw("same");
//	leg->Draw();
//	gPad->Modified();
//	gPad->Update();
//
//
//}

//-------------------------------------
//  Draw QFactor (wit Acceptance+Uncertainties+Leakage effect) as a function of the integration angle (theta [deg])
//
//  TString author      	-> Name of the author to use as reference: Geringer-Sameth (2014), Bonnivard (2016);
//  TString source			-> Name of the dSph galaxy to use;
//  TString candidate; 		-> Type of DM candidate: Annihilation or Decay
//  TString instrumentName 	-> Instrument name
//  Double_t wobble			-> Wobble distance [deg]
void PlotQ1234Factor()
{
	TString myInstrumentPath = "/Users/palacio/Work/Software/ObservationOptimization/ObservationOptimization/macros";
	TString mySourcePath = "/Users/palacio/Work/Software/ObservationOptimization/ObservationOptimization/macros";
	TString author = "Bonnivard";
	TString source = "uma2";
	TString candidate = "Decay";
	TString instrumentName= "IDEAL";
	Double_t distanceCameraCenterMax=5;	// [deg]
	Double_t wobbleDist=0.;	// [deg]
	Double_t normalizationPoint=8.8136e+19;

	//JDOptimization* QFactor = new JDOptimization();
	JDOptimization* QFactor = new JDOptimization(author, source, candidate, mySourcePath, myInstrumentPath, instrumentName, distanceCameraCenterMax, wobbleDist);
	TF1* functionQFactorVsTheta= QFactor->GetTF1QFactorVsTheta(1234,0.4);
//	TF1* functionEfficiency = QFactor->GetTF1EfficiencyVsTheta();
//
//	Double_t thetaMax = QFactor->GetThetaMax();
//	Double_t distCameraCenterMax = QFactor->GetDistCameraCenterMax();
//
//	functionQFactorVsTheta->SetLineColor(2);
//	functionQFactorVsTheta->SetLineStyle(1);
//
////	// This is for plotting purposes only
//	TString isBonnivard = (QFactor->GetIsBonnivard()? "arxiv:1504.02048" : "");
//	TString isGeringer = (QFactor->GetIsGeringer()? "arxiv:1408.0002" : "");
//	TString isIDEAL = (QFactor->GetIsIdeal()? "IDEAL" : "");
//
//	TLegend* leg=new TLegend(.18,.15,.88,.25);
//	leg->AddEntry(functionQFactorVsTheta, QFactor->GetSourceName()+" - "+isIDEAL+" - "+isBonnivard+isGeringer, "l");
//	leg->SetFillColor(0);
//	leg->SetLineColor(1);
//	leg->SetBorderSize(1);
//	leg->SetTextSize(0.037);
//
//	TCanvas* canvas2 = new TCanvas("canvas2","",600,550);
//	TH1I* dummy1 = new TH1I("dummy1","" ,1,1e-2,thetaMax);
//	dummy1->SetMaximum(3.);
//	dummy1->SetMinimum(0.);
//	dummy1->SetStats(0);
//	dummy1->SetXTitle(" #theta [deg]");
//	if(QFactor->GetCandidate()=="Annihilation"){	dummy1->SetYTitle(" QFactor_{Ann} ");}
//	else if(QFactor->GetCandidate()=="Decay"){	dummy1->SetYTitle(" QFactor_{Dec} ");}
//	dummy1->GetXaxis()->SetTitleOffset(1.3);
//	dummy1->GetYaxis()->SetTitleOffset(1.5);
//	dummy1->DrawCopy();
////	gPad->SetLogx();
////	gPad->SetLogy();
//	gPad->SetGridy();
//	gPad->SetGridx();
//	functionQFactorVsTheta->Draw("same");
//	leg->Draw();
//	gPad->Modified();
//	gPad->Update();


}

void exampleJDOptimization()
{

//	PlotQ0Factor();		// 	J/theta
//	PlotQ1Factor();		//	J_on-J_off/theta
//	PlotQ2Factor();		//  J_1sm/theta
//	PlotQ3Factor();		//  J_eff/theta_eff
//
//	PlotQ12Factor();	//	J_on_1sm/Sqrt{theta^2 + J_off_1sm}
//	PlotQ13Factor();	//	J_on_eff/Sqrt{(theta_eff)^2 + J_off_eff}
//	PlotQ23Factor();	//	J_1sm_eff/theta_eff
//	PlotQ123Factor();	//	J_on_1sm_eff/Sqrt{(theta_eff)^2 + J_off_1sm_eff}
	PlotQ1234Factor();
}

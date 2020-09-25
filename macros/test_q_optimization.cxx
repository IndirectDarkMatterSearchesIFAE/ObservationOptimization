/*
 * JDDM.cc
 *
 *  Created on: 03/07/2017
 *  Authors: David Navarro Gironés 	<<david.navarro.girones@gmail.com>>
 *  		 Joaquim Palacio 		<<jpalacio@ifae.es>>
 *
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
#include <TF2.h>
#include <TGraph.h>
#include <TH1.h>
#include <TLegend.h>
#include <TPave.h>
#include <TString.h>
#include <TVirtualPad.h>
#include <iostream>
#include <TStyle.h>

//#include "../source/JDAstroProfile.cc"
#include "../source/JDDarkMatter.cc"
#include "../source/JDInstrument.cc"
#include "../source/JDOptimization.cc"

//#include "/home/jpalacio/Work/analysis/Software/scripts/tdrstyle.C"

using namespace std;

void test_q_optimization()
{

//	setDefaultStyle();


	TString mySourcePath = "/Users/palacio/Work/Software/ObservationOptimization/ObservationOptimization";
	TString myInstrumentPath = "/Users/palacio/Work/Software/ObservationOptimization/ObservationOptimization";
	TString pathPlots = "/Users/palacio/Work/Software/ObservationOptimization/ObservationOptimization/plots/loop";

	TString author = "Bonnivard";
	//TString sourceList[] = {"boo1", "car", "coma", "cvn1", "cvn2", "dra", "for", "her", "leo1", "leo2", "leo4", "leo5", "leot", "scl", "seg1", "seg2", "sex", "uma1", "uma2", "umi", "wil1"};
	TString sourceList[] = {"uma2", "uma1"}; //"{"boo1", "car", "coma", "cvn1", "cvn2", "dra", "for", "her", "leo1", "leo2", "leo4", "leo5", "leot", "scl", "seg1", "seg2", "sex", "uma1", "uma2", "umi", "wil1"};
	TString candidate= "Decay";
	TString instrumentNameList[] = {"MAGICPointLike", "CTANorth50To80GeV"};
	Double_t distCameraCenter=5.;	// [deg]
	Double_t wobble=0.;	// [deg]

	Int_t numSources =21;
	Int_t numInstruments=2;

	ofstream myfile;
	myfile.open("../plots/ResultatsBonnivardDecay_Q134_.txt");
	myfile <<"Source 	"<<"Author "<< "Candidate"<<"Instrument	"<<"Theta Optim	"<<"Wobble Optim	"<<endl;

	TCanvas* canvas = new TCanvas("canvas","",600,550);
	TH2I* dummy = new TH2I("dummy", " ",1,0.,3.0, 1, 0, 1.8);
	dummy->SetStats(0);
	dummy->SetXTitle(" #theta [deg]");
	dummy->SetYTitle(" w [deg]");
	dummy->GetXaxis()->SetTitleOffset(1.3);
	dummy->GetYaxis()->SetTitleOffset(1.4);
	dummy->SetMaximum(1.);
	dummy->SetMinimum(0.);

	Double_t  optimizationFactor = 1234;

	for(Int_t i=0; i<numSources;i++)
	{
		TString source=sourceList[i];

		for(Int_t j=0; j<numInstruments;j++)
		{
			TString instrumentName=instrumentNameList[j];

			JDOptimization* Q13Factor2D = new JDOptimization(author, source, candidate, mySourcePath, myInstrumentPath, instrumentName, distCameraCenter, wobble);
			TH2D* Histogram = Q13Factor2D->GetTH2QFactorVsThetaWobble(optimizationFactor);

			//Double_t thetaOpt=1, thetaOptRangMin=1, thetaOptRangMax=1, wobbleOpt=1, wobbleOptRangMin=1, wobbleOptRangMax=1;
			//Q13Factor2D->GetOptimalThetaAndWobble(thetaOpt, thetaOptRangMin, thetaOptRangMax, wobbleOpt, wobbleOptRangMin, wobbleOptRangMax, 134);

			//myfile <<std::setprecision(2) << source<<" & "<< author <<" & "<< candidate <<" & "<< instrumentName<<" & "<< thetaOpt << " (" << thetaOptRangMin<<  ", "<<thetaOptRangMax << ") & " << wobbleOpt << " (" << wobbleOptRangMin<<  ", "<<wobbleOptRangMax << ")\\\\"<<endl;

			canvas->cd();
			dummy->DrawCopy();
		//	gPad->SetLogx();
		//	gPad->SetLogy();
			gPad->SetGridy();
			gPad->SetGridx();
			Histogram->Draw("same colz");
			gPad->Modified();
			gPad->Update();

			canvas->Print(pathPlots+"/Bonnivard_Decay_"+source+"_"+instrumentName+"_Q"+optimizationFactor+"_.pdf");
			Q13Factor2D-> ~JDOptimization();

			break;
		}
		break;
	}
	myfile.close();
}


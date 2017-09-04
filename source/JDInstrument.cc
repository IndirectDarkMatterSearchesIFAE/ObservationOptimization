/*
 * JDInstrument.cc
 *
 *  Created on: 03/07/2017
 *  Authors: David Navarro Gironés 	<<david.navarrogir@e-campus.uab.cat>>
 *  		 Joaquim Palacio 		<<jpalacio@ifae.es>>
 *
 *  		 ADD A GENERAL DESCRIPTION ON THE CLASS, THE MAIN FUNCTIONS, THE VARIABLES
 *  		 AND MENTION THE "runExample#.C" THAT SHOWS HOW TO USE IT
 */

#include "JDInstrument.h"

#include <TGraph.h>
#include <TMath.h>
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

using namespace std;

//-----------------------------------------------
//
//	new (QUIM) for que IDEAL case
JDInstrument::JDInstrument():
		gCameraAcceptance(NULL),fEvaluateEfficiencyVsTheta(NULL),
		fEvaluateEpsilonVsThetaAndPhi(NULL),fEvaluateEpsilonThetaVsThetaAndPhi(NULL),
		bIsIdeal(0), bIsMagic(0), bIsCameraAcceptance(0)
{
	    cout << endl;
		cout << endl;
		cout << "   Constructor JDInstrument..." << endl;
		cout << endl;
		cout << endl;

		CreateFunctionsInstrument();

}

//-----------------------------------------------
//
//	new (QUIM) for que IDEAL case
JDInstrument::JDInstrument(Double_t distanceCameraCenterMax,Double_t wobbleDist):
		dWobbleDist(wobbleDist),
		gCameraAcceptance(NULL),fEvaluateEfficiencyVsTheta(NULL),
		fEvaluateEpsilonVsThetaAndPhi(NULL),fEvaluateEpsilonThetaVsThetaAndPhi(NULL),
		bIsIdeal(0), bIsMagic(0), bIsCameraAcceptance(0)
{
	    cout << endl;
		cout << endl;
		cout << "   Constructor JDInstrument..." << endl;
		cout << endl;
		cout << endl;

	if(SetCameraAcceptanceIdeal(distanceCameraCenterMax))
	{
		cout << "   *********************************************" << endl;
		cout << "   ***                                       ***" << endl;
		cout << "   ***   Camera Acceptance could not be set  ***" << endl;
		cout << "   ***                                       ***" << endl;
		cout << "   *********************************************" << endl;

		return;
	}
	CreateFunctionsInstrument();
}

//-----------------------------------------------
//
//	new (QUIM)
JDInstrument::JDInstrument(TGraph* cameraAcceptance, Double_t wobbleDist):
		dWobbleDist(wobbleDist),
		gCameraAcceptance(NULL),fEvaluateEfficiencyVsTheta(NULL),
		fEvaluateEpsilonVsThetaAndPhi(NULL),fEvaluateEpsilonThetaVsThetaAndPhi(NULL),
		bIsIdeal(0), bIsMagic(0), bIsCameraAcceptance(0)
{
	    cout << endl;
		cout << endl;
		cout << "   Constructor JDInstrument..." << endl;
		cout << endl;
		cout << endl;

	if(SetCameraAcceptanceFromTGraph(cameraAcceptance))
	{
		cout << "   *********************************************" << endl;
		cout << "   ***                                       ***" << endl;
		cout << "   ***   Camera Acceptance could not be set  ***" << endl;
		cout << "   ***                                       ***" << endl;
		cout << "   *********************************************" << endl;
		return;
	}
	CreateFunctionsInstrument();
}

//-----------------------------------------------
//
//	This is the constructor.
//
//	Possible variables are:
// 	sInstrumentName 	= (TString) name of the telescope
//	dWobble			 	= (Double_t) wobble distance (QUIM) UNITS!!!
//	sMyInstrumentPath	= (TString) name of the instrument path
JDInstrument::JDInstrument(TString instrumentName, Double_t wobble, TString instrumentPath):
		sInstrumentName(instrumentName), dWobbleDist(wobble), sInstrumentPath(instrumentPath),
		gCameraAcceptance(NULL),fEvaluateEfficiencyVsTheta(NULL),
		fEvaluateEpsilonVsThetaAndPhi(NULL),fEvaluateEpsilonThetaVsThetaAndPhi(NULL),
		bIsIdeal(0), bIsMagic(0), bIsCameraAcceptance(0)
{
	    cout << endl;
		cout << endl;
		cout << "   Constructor JDInstrument..." << endl;
		cout << endl;
		cout << endl;

	if(SetCameraAcceptanceFromInstrument())
	{
		cout << "   ***************************************************************" << endl;
		cout << "   ***                                                         ***" << endl;
		cout << "   ***   Cammera acceptance could not be read from references  ***" << endl;
		cout << "   ***                                                         ***" << endl;
		cout << "   ***************************************************************" << endl;
		return;
	}
	CreateFunctionsInstrument();
}

//-----------------------------------------------
// This is the destructor
// It deletes the functions in order not to be reused
JDInstrument::~JDInstrument()
{

	if (gCameraAcceptance)								delete gCameraAcceptance;
	if (fEvaluateEfficiencyVsTheta)						delete fEvaluateEfficiencyVsTheta;
	if (fEvaluateEpsilonVsThetaAndPhi)					delete fEvaluateEpsilonVsThetaAndPhi;
	if (fEvaluateEpsilonThetaVsThetaAndPhi)				delete fEvaluateEpsilonThetaVsThetaAndPhi;

		cout << endl;
		cout << endl;
		cout << "   Destructor JDInstrument..." << endl;
		cout << endl;
		cout << endl;
}

//-----------------------------------------------
// This function calls the important functions of this class
//
// dThetaMax (Double_t) is used in Efficiency and EpsilonPerTheta so as to stop the function as soon as the evaluation gets out of the camera
//
// SetEpsilon() redirects us to a function that fills a TGraph with the epsilon of the camera vs dcc (distance to the centre of the camera)
// fEvaluateEfficiencyVsTheta -> TF1 that evaluates the Efficiency vs Theta. The Efficieny is defined as the integral of epsilon multiplied by theta and divided by the area integrated; Efficiency [] theta [deg]
// fEvaluateEpsilonVsThetaAndPhi-> TF2 that evaluates the Epsilon vs theta and phi; theta[deg] phi[rad]
// fEvaluateEpsilonPerThetaVsThetaAndPhi-> TF2 that evaluates the Epsilon multiplied by theta vs theta and phi; theta[deg] phi[rad]
void JDInstrument::CreateFunctionsInstrument()
{
	SetIsCameraAcceptance(1);

	fEvaluateEpsilonVsTheta = new TF1("fEvaluateEpsilonVsTheta", this, &JDInstrument::EvaluateEpsilonVsTheta, 0., GetDistCameraCenterMax(), 0, "JDInstrument", "EvaluateEpsilonVsTheta");
	fEvaluateEfficiencyVsTheta = new TF1("fEvaluateEfficiencyVsTheta", this, &JDInstrument::EvaluateEfficiencyVsTheta, 0., GetDistCameraCenterMax(), 0, "JDInstrument", "EvaluateEfficiencyVsTheta");

	fEvaluateEpsilonVsThetaAndPhi = new TF2("fEvaluateEpsilonVsThetaAndPhi", this, &JDInstrument::EvaluateEpsilonVsThetaAndPhi, 0., GetDistCameraCenterMax(), -TMath::Pi(), TMath::Pi(), 1, "JDInstrument", "EvaluateEpsilonVsThetaAndPhi");
	fEvaluateEpsilonVsXAndY = new TF2("fEvaluateEpsilonVsXAndY", this, &JDInstrument::EvaluateEpsilonVsXAndY, -GetDistCameraCenterMax(), GetDistCameraCenterMax(), -GetDistCameraCenterMax(), GetDistCameraCenterMax(), 1, "JDInstrument", "EvaluateEpsilonVsXAndY");
	fEvaluateEpsilonThetaVsThetaAndPhi = new TF2("fEvaluateEpsilonThetaVsThetaAndPhi", this, &JDInstrument::EvaluateEpsilonThetaVsThetaAndPhi, 0., GetDistCameraCenterMax(), -TMath::Pi(), TMath::Pi(), 1, "JDInstrument", "EvaluateEpsilonThetaVsThetaAndPhi");
}

//-----------------------------------------------
//	New (QUIM)
Bool_t JDInstrument::SetCameraAcceptanceFromTGraph(TGraph* cameraAcceptance,Bool_t verbose)
{
	gCameraAcceptance = cameraAcceptance;

	SetNumPointsCameraAcceptanceGraph((Int_t)gCameraAcceptance->GetN());
	if(GetNumPointsJFactorGraph()<=0) return -1;

	Int_t numPoint = GetNumPointsJFactorGraph();
	SetDistCenterCameraMax(gCameraAcceptance->GetX()[numPoint-1]);

	if(verbose)
	{
		for(Int_t i=0;i<GetNumPointsJFactorGraph();i++)
			cout << gCameraAcceptance->GetY()[i] << endl;
	}

	return 0;
}


//-----------------------------------------------
// new (QUIM
Bool_t JDInstrument::SetCameraAcceptanceIdeal(Double_t distanceCenterCameraMax)
{

	cout << "   "<< endl;
	cout << "   Setting  camera acceptance to IDEAL "<< endl;
	cout << "   "<< endl;

	SetIsIdeal(1);

	SetDistCenterCameraMax(distanceCenterCameraMax);
	SetNumPointsCameraAcceptanceGraph(100);

	gCameraAcceptance = new TGraph();

	Double_t distMin=0;
	Double_t distMax=GetDistCameraCenterMax();
	for(Int_t i=0; i<GetNumPointsJFactorGraph();i++)
	{
		Double_t dist= distMin+(distMax-distMin)/(GetNumPointsJFactorGraph()*1.)*i;
		gCameraAcceptance->SetPoint(i,dist,1.);
	}

	return 0;
}
//-----------------------------------------------
// This function fills a TGraph with the information of the epsilon
// of the telescope normalized at the centre of the camera.
// We define epsilon as the % of the quality of the camera with respect
// to its centre.
//
// dDcc (Double_t) is the distance to the centre of the camera
//
Bool_t JDInstrument::SetCameraAcceptanceFromInstrument(Bool_t verbose)
{

	Int_t contador = 0;

	gCameraAcceptance = new TGraph();

	Double_t distanceCenterCamera;					// [deg]
	Double_t rateVsDistanceCC,rateVsDistanceCC0;	// [deg]

	if(GetInstrumentName()=="IDEAL")
	{
		cout << "   "<< endl;
		cout << "   ERROR: Setting camera acceptance to IDEAL: "<< endl;
		cout << "         -> No distance to camera center max defined." << endl;
		cout << "            use JDInstrument(Double_t distanceCameraCenterMax) instead" << endl;
		cout << "   "<< endl;
		return -1;
	}
	else if(GetInstrumentName()=="MAGICPointLike")
	{
		ifstream file (GetInstrumentPath()+"/references/IACTPerformance/MAGICPointLike/MAGICPointLikeRateVsOffset.txt");

		cout << "   "<< endl;
		cout << "   Reading camera acceptance from: "<< endl;
		cout << "   The major upgrade of the MAGIC telescopes, Part II: A performance study using observations of the Crab Nebula" << endl;
		cout << "   The MAGIC Collaboration, " << endl;
		cout << "   https://arxiv.org/abs/1409.5594" << endl;
		cout << "   "<< endl;
		SetIsMagic(1);

		while(file 	>> distanceCenterCamera >> rateVsDistanceCC)
			{
				if (contador==0){
					rateVsDistanceCC0=rateVsDistanceCC;
				}

				if(verbose)cout << distanceCenterCamera  << " " <<  rateVsDistanceCC << endl;

				gCameraAcceptance->SetPoint(contador,distanceCenterCamera,(rateVsDistanceCC/rateVsDistanceCC0));

				contador++;
			}
		SetDistCenterCameraMax(distanceCenterCamera);
		file.close();
		if(contador>0) return 0;
	}

	return -1;
}

//-----------------------------------------------
//	new (QUIM)
Double_t JDInstrument::EvaluateEpsilonVsTheta(Double_t* x, Double_t* par)
{

	if (x[0]<=GetDistCameraCenterMax()) {	return gCameraAcceptance->Eval(x[0]);}
	else							 	{  return 1.e-20;}							// To make integrals converge
}

//-----------------------------------------------
//	new (QUIM)
//	x[0] = x [deg]
//	x[1] = y [deg]
//  par[0] = wobble [deg]
Double_t JDInstrument::EvaluateEpsilonVsXAndY(Double_t* x, Double_t* par)
{
	Double_t theta = TMath::Sqrt(TMath::Power(x[0],2)+TMath::Power(x[1],2));
	Double_t phi = TMath::ATan2(x[1],x[0]);

	fEvaluateEpsilonVsThetaAndPhi->SetParameter(0,par[0]);
	return fEvaluateEpsilonVsThetaAndPhi->Eval(theta,phi);
}

//-----------------------------------------------
//	It evaluates the Epsilon of the camera vs theta and phi
//
//	dccR distance to the centre of the camera in radial components (QUIM) UNITS!!!
//	x[0] = theta [deg]
//  x[1] = phi [rad]
//  par[0] = wobble [deg]
Double_t JDInstrument::EvaluateEpsilonVsThetaAndPhi(Double_t* x, Double_t* par)
{
	//(QUIM) - La formula es xx+yy-2xy·cos(); el motiu per posar un signe més es perque en realitat es phi'=180-phi?
	//		(see triangle slide-2)
	//		 - Power(a,0.50) =? Sqrt()
	//Double_t dccR = TMath::Power(TMath::Power(dWobble,2)+TMath::Power(x[0],2)+2*dWobble*x[0]*TMath::Cos(x[1]),0.50);
	Double_t dccR = TMath::Power(TMath::Power(par[0],2)+TMath::Power(x[0],2)+2*par[0]*x[0]*TMath::Cos(x[1]),0.50);

	if (dccR<=GetDistCameraCenterMax()) {	return gCameraAcceptance->Eval(dccR);}
	else							 	{  return 1.e-20;}							// To make integrals converge
}

//-----------------------------------------------
//	It evaluates the Epsilon of the camera multiplied by theta vs theta and phi
//	(QUIM) - Why are you multiplying by theta? Area = int dphy int r·dr ; where theta is r.
//
//	x[0] = theta [deg]
//  x[1] = phi [rad]
Double_t JDInstrument::EvaluateEpsilonThetaVsThetaAndPhi(Double_t* x, Double_t* par)
{
	fEvaluateEpsilonVsThetaAndPhi->SetParameter(0,par[0]);

	return fEvaluateEpsilonVsThetaAndPhi->Eval(x[0], x[1])*x[0];
}

//-----------------------------------------------
//	It evaluates the Efficiency of the camera vs theta
//	(QUIM) what is efficiency, see slide 3
//
//	x[0] = theta [deg]
Double_t JDInstrument::EvaluateEfficiencyVsTheta(Double_t* x, Double_t* par)
{
	fEvaluateEpsilonThetaVsThetaAndPhi->SetParameter(0,GetWobbleDistance());

	return fEvaluateEpsilonThetaVsThetaAndPhi->Integral(0., x[0], -TMath::Pi(), TMath::Pi(), 1.e-4)/(TMath::Pi()*TMath::Power(x[0],2));
}

void JDInstrument::GetListOfInstruments()
{
	cout << " " << endl;
	cout << "    List of available instruments is:" << endl;
	cout << "    	- MAGICPointLike" << endl;
	cout << "    	- CTA (to be defined...)" << endl;
	cout << " " << endl;

}

void JDInstrument::GetUnits()
{
	cout << " " << endl;
	cout << "    All units are given in:" << endl;
	cout << "    	- Acceptance ~ %" << endl;
	cout << "    	- Efficiency ~ %" << endl;
	cout << "    	- ~deg" << endl;
	cout << " " << endl;
}

void JDInstrument::GetListOfConstructors()
{
	cout << " " << endl;
	cout << "    List of available constructors is:" << endl;
	cout << "    JDInstrument()" << endl;
	cout << "    JDInstrument(Double_t distanceCameraCenterMax, Double_t wobbleDist)" << endl;
	cout << "    JDInstrument(TGraph* cameraAcceptance,Double_t wobbleDist)" << endl;
	cout << "    JDInstrument(TString instrumentName, Double_t wobble, TString instrumentPath)" << endl;
	cout << " " << endl;
}

void JDInstrument::GetWarning()
{
	cout << "  *****************************" << endl;
	cout << "  ***" << endl;
	cout << "  ***  WARNING:" << endl;
	cout << "  ***" << endl;
	cout << "  ***  	- 	Camera Acceptance not defined..." << endl;
	cout << " " << endl;
}

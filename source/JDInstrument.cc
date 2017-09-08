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
//	This is the constructor used to show the possibilities that offers the JDInstrument class
JDInstrument::JDInstrument():
		gCameraAcceptance(NULL), fEvaluateEpsilonVsTheta(NULL), fEvaluateEfficiencyVsTheta(NULL),
		fEvaluateEpsilonVsThetaAndPhi(NULL), fEvaluateEpsilonVsXAndY(NULL), fEvaluateEpsilonThetaVsThetaAndPhi(NULL),
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
//	This is the constructor used to set an ideal camera acceptance
//	There are two values that have to be given:
//		distanceCameraCenterMax: Radius of the camera [deg]
//		wobbleDist: Distance from the centre of the camera where the object of interest is placed [deg]
JDInstrument::JDInstrument(Double_t distanceCameraCenterMax,Double_t wobbleDist):
		dWobbleDist(wobbleDist),
		gCameraAcceptance(NULL), fEvaluateEpsilonVsTheta(NULL), fEvaluateEfficiencyVsTheta(NULL),
		fEvaluateEpsilonVsThetaAndPhi(NULL), fEvaluateEpsilonVsXAndY(NULL), fEvaluateEpsilonThetaVsThetaAndPhi(NULL),
		bIsIdeal(0), bIsMagic(0), bIsCameraAcceptance(0)
{
	    cout << endl;
		cout << endl;
		cout << "   Constructor JDInstrument..." << endl;
		cout << endl;
		cout << endl;

	if(!SetCameraAcceptanceIdeal(distanceCameraCenterMax))
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
//	This is the constructor used to set a camera acceptance using data of a TGraph
//	There are two values that have to be given:
//		cameraAcceptance: Information of the acceptance of the camera [%]
//		wobbleDist: Distance from the centre of the camera where the object of interest is placed [deg]
JDInstrument::JDInstrument(TGraph* cameraAcceptance, Double_t wobbleDist):
		dWobbleDist(wobbleDist),
		gCameraAcceptance(NULL), fEvaluateEpsilonVsTheta(NULL), fEvaluateEfficiencyVsTheta(NULL),
		fEvaluateEpsilonVsThetaAndPhi(NULL), fEvaluateEpsilonVsXAndY(NULL), fEvaluateEpsilonThetaVsThetaAndPhi(NULL),
		bIsIdeal(0), bIsMagic(0), bIsCameraAcceptance(0)
{
	    cout << endl;
		cout << endl;
		cout << "   Constructor JDInstrument..." << endl;
		cout << endl;
		cout << endl;

	if(!SetCameraAcceptanceFromTGraph(cameraAcceptance))
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
//	This is the constructor used when the data is given by a txtFile
JDInstrument::JDInstrument(TString txtFile, Double_t wobbleDist):
		dWobbleDist(wobbleDist),
		gCameraAcceptance(NULL), fEvaluateEpsilonVsTheta(NULL), fEvaluateEfficiencyVsTheta(NULL),
		fEvaluateEpsilonVsThetaAndPhi(NULL), fEvaluateEpsilonVsXAndY(NULL), fEvaluateEpsilonThetaVsThetaAndPhi(NULL),
		bIsIdeal(0), bIsMagic(0), bIsCameraAcceptance(0)
{
	cout << endl;
	cout << endl;
	cout << "   Constructor JDDarkMatter..." << endl;
	cout << endl;
	cout << endl;

	if(!SetCameraAcceptanceFromTxtFile(txtFile))
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
//	dWobble			 	= (Double_t) wobble distance [deg]
//	sMyInstrumentPath	= (TString) name of the instrument path
JDInstrument::JDInstrument(TString instrumentName, Double_t wobble, TString instrumentPath):
		sInstrumentName(instrumentName), dWobbleDist(wobble), sInstrumentPath(instrumentPath),
		gCameraAcceptance(NULL), fEvaluateEpsilonVsTheta(NULL), fEvaluateEfficiencyVsTheta(NULL),
		fEvaluateEpsilonVsThetaAndPhi(NULL), fEvaluateEpsilonVsXAndY(NULL), fEvaluateEpsilonThetaVsThetaAndPhi(NULL),
		bIsIdeal(0), bIsMagic(0), bIsCameraAcceptance(0)
{
	    cout << endl;
		cout << endl;
		cout << "   Constructor JDInstrument..." << endl;
		cout << endl;
		cout << endl;

	if(!SetCameraAcceptanceFromInstrument())
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
	if (fEvaluateEpsilonVsTheta)						delete fEvaluateEpsilonVsTheta;
	if (fEvaluateEfficiencyVsTheta)						delete fEvaluateEfficiencyVsTheta;
	if (fEvaluateEpsilonVsThetaAndPhi)					delete fEvaluateEpsilonVsThetaAndPhi;
	if (fEvaluateEpsilonVsXAndY)						delete fEvaluateEpsilonVsXAndY;
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
// WARNING:ISSUE
// dThetaMax (Double_t) is used in Efficiency and EpsilonPerTheta so as to stop the function as soon as the evaluation gets out of the camera
//
// fEvaluateEpsilonVsTheta -> TF1 that evaluates the Epsilon vs Theta. The Epsilon is a short way to call the camera acceptance; Epsilon [%] theta [deg]
// fEvaluateEfficiencyVsTheta -> TF1 that evaluates the Efficiency vs Theta. The Efficieny is defined as the integral of epsilon multiplied by theta and divided by the area integrated; Efficiency [%] theta [deg]
// fEvaluateEpsilonVsThetaAndPhi-> TF2 that evaluates the Epsilon vs theta and phi; Epsilon [%] theta[deg] phi[rad]
// fEvaluateEpsilonVsXAndY-> TF2 that evaluates the Epsilon vs x and y; Epsilon [%] x[deg] y[deg]
// fEvaluateEpsilonPerThetaVsThetaAndPhi-> TF2 that evaluates the Epsilon multiplied by theta vs theta and phi; Epsilon [%] theta[deg] phi[rad]
void JDInstrument::CreateFunctionsInstrument()
{

	fEvaluateEpsilonVsTheta = new TF1("fEvaluateEpsilonVsTheta", this, &JDInstrument::EvaluateEpsilonVsTheta, 0., GetDistCameraCenterMax(), 0, "JDInstrument", "EvaluateEpsilonVsTheta");
	fEvaluateEfficiencyVsTheta = new TF1("fEvaluateEfficiencyVsTheta", this, &JDInstrument::EvaluateEfficiencyVsTheta, 0., GetDistCameraCenterMax(), 0, "JDInstrument", "EvaluateEfficiencyVsTheta");

	fEvaluateEpsilonVsThetaAndPhi = new TF2("fEvaluateEpsilonVsThetaAndPhi", this, &JDInstrument::EvaluateEpsilonVsThetaAndPhi, 0., GetDistCameraCenterMax(), -TMath::Pi(), TMath::Pi(), 1, "JDInstrument", "EvaluateEpsilonVsThetaAndPhi");
	fEvaluateEpsilonVsXAndY = new TF2("fEvaluateEpsilonVsXAndY", this, &JDInstrument::EvaluateEpsilonVsXAndY, -GetDistCameraCenterMax(), GetDistCameraCenterMax(), -GetDistCameraCenterMax(), GetDistCameraCenterMax(), 1, "JDInstrument", "EvaluateEpsilonVsXAndY");
	fEvaluateEpsilonThetaVsThetaAndPhi = new TF2("fEvaluateEpsilonThetaVsThetaAndPhi", this, &JDInstrument::EvaluateEpsilonThetaVsThetaAndPhi, 0., GetDistCameraCenterMax(), -TMath::Pi(), TMath::Pi(), 1, "JDInstrument", "EvaluateEpsilonThetaVsThetaAndPhi");
}

//-----------------------------------------------
//
//	This boolean is TRUE(1) if the ideal camera acceptance can be read and FALSE(0) if the ideal camera acceptance can not be read
//	It fills a TGraph with the data corresponding to an ideal camera acceptance: Everywhere the acceptance is 100%
//	It sets the number of iterations that the loop will make
//	If the process is correct, the boolean SetIsCameraAcceptance is TRUE(1)
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
	for(Int_t i=0; i<GetNumPointsCameraAcceptanceGraph();i++)
	{
		Double_t dist= distMin+(distMax-distMin)/(GetNumPointsCameraAcceptanceGraph()*1.)*i;
		gCameraAcceptance->SetPoint(i,dist,1.);
	}

	SetIsCameraAcceptance(1);
	return 1;
}

//-----------------------------------------------
//
//	This boolean is TRUE(1) if the camera acceptance of a TGraph can be read and FALSE(0) if the camera acceptance of a TGraph can not be read
//	It fills a TGraph with the data given by the user with another TGraph
//	It sets the maximum distance to the center of the camera
//	If the process is correct, the boolean SetIsCameraAcceptance is TRUE(1)
Bool_t JDInstrument::SetCameraAcceptanceFromTGraph(TGraph* cameraAcceptance,Bool_t verbose)
{
	gCameraAcceptance = cameraAcceptance;

	SetNumPointsCameraAcceptanceGraph((Int_t)gCameraAcceptance->GetN());
	if(GetNumPointsCameraAcceptanceGraph()<=0) return 0;

	Int_t numPoint = GetNumPointsCameraAcceptanceGraph();
	SetDistCenterCameraMax(gCameraAcceptance->GetX()[numPoint-1]);

	if(verbose)
	{
		for(Int_t i=0;i<GetNumPointsCameraAcceptanceGraph();i++)
			cout << gCameraAcceptance->GetY()[i] << endl;
	}

	SetIsCameraAcceptance(1);
	return 1;
}

Bool_t JDInstrument::SetCameraAcceptanceFromTxtFile(TString txtFile, Bool_t verbose)
{
	Double_t DistCenterCamera, Epsilon;
	Int_t contador=0;

	gCameraAcceptance = new TGraph();

	ifstream file (txtFile);
	while(file >> DistCenterCamera >> Epsilon)
	{
		// only for Tests
		if (verbose==1)	cout << DistCenterCamera << " " << Epsilon << endl;

		gCameraAcceptance->SetPoint(contador,DistCenterCamera,Epsilon);

		contador ++;
	}

	SetNumPointsCameraAcceptanceGraph(contador);
	SetDistCenterCameraMax(DistCenterCamera);
	file.close();

	if(GetNumPointsCameraAcceptanceGraph()<=0) return 0;

	SetIsCameraAcceptance(1);
	return 1;
}


//-----------------------------------------------
//
//	This boolean is TRUE(1) if the camera acceptance of the instrument can be read and FALSE(0) if the camera acceptance of the instrument can not be read
//	It fills a TGraph with the information of the camera acceptance of the telescope normalized at the centre of the camera.
//  We define camera acceptance (epsilon) as the % of the quality of the camera with respect to its centre.
//	It sets the maximum distance to the center of the camera
//	If the process is correct, the boolean SetIsCameraAcceptance is TRUE(1)
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
		return 0;
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
		SetIsCameraAcceptance(1);
		if(contador>0) return 1;
	}

	return 0;
}

//-----------------------------------------------
// It evaluates the Epsilon [%] vs Theta [deg] as long as Theta is smaller than the maximum distance to the center of the camera
//
// x[0] 	= dTheta [deg]
Double_t JDInstrument::EvaluateEpsilonVsTheta(Double_t* x, Double_t* par)
{

	if (x[0]<=GetDistCameraCenterMax()) {	return gCameraAcceptance->Eval(x[0]);}
	else							 	{  return 1.e-20;}							// To make integrals converge
}

//-----------------------------------------------
// It evaluates the Epsilon [%] vs X [deg] and Y [deg]
//
//	x[0] = x [deg]
//	x[1] = y [deg]
Double_t JDInstrument::EvaluateEpsilonVsXAndY(Double_t* x, Double_t* par)
{
	Double_t theta = TMath::Sqrt(TMath::Power(x[0],2)+TMath::Power(x[1],2));
	Double_t phi = TMath::ATan2(x[1],x[0]);

	fEvaluateEpsilonVsThetaAndPhi->SetParameter(0,par[0]);
	return fEvaluateEpsilonVsThetaAndPhi->Eval(theta,phi);
}

//-----------------------------------------------
//	It evaluates the Epsilon vs Theta [deg] and Phi [rad]
//
//	dccR distance to the centre of the camera in radial components [deg]
//
//	x[0] = theta [deg]
//  x[1] = phi [rad]
//  par[0] = wobble [deg]
Double_t JDInstrument::EvaluateEpsilonVsThetaAndPhi(Double_t* x, Double_t* par)
{
	//(QUIM) - La formula es xx+yy-2xy·cos(); el motiu per posar un signe més es perque en realitat es phi'=180-phi?
	//		(see triangle slide-2)
	// ISSUE: I think the reason why is +, is what you have said.
	//		 - Power(a,0.50) =? Sqrt()
	Double_t dccR = TMath::Power(TMath::Power(par[0],2)+TMath::Power(x[0],2)+2*par[0]*x[0]*TMath::Cos(x[1]),0.50);

	if (dccR<=GetDistCameraCenterMax()) {	return gCameraAcceptance->Eval(dccR);}
	else							 	{  return 1.e-20;}							// To make integrals converge
}

//-----------------------------------------------
//	It evaluates the Epsilon of the camera [%] multiplied by theta [deg] vs theta [deg] and phi [rad]
//	(QUIM) - Why are you multiplying by theta? Area = int dphy int r·dr ; where theta is r.
//	ISSUE: Discuss about that, I don't get your point, if theta is r then is correct what I have done, isn't it?
//
//	x[0] = theta [deg]
//  x[1] = phi [rad]
//  par[0] = wobble [deg]
Double_t JDInstrument::EvaluateEpsilonThetaVsThetaAndPhi(Double_t* x, Double_t* par)
{
	fEvaluateEpsilonVsThetaAndPhi->SetParameter(0,par[0]);

	return fEvaluateEpsilonVsThetaAndPhi->Eval(x[0], x[1])*x[0];
}

//-----------------------------------------------
//	It evaluates the Efficiency [%] vs theta [deg]
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
	cout << "    JDInstrument(txtFile cameraAcceptance,Double_t wobbleDist) (FALTA PER FER, EASY)" << endl;
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

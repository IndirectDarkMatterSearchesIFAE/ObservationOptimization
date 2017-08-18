/*
 * JDInstrument.cc
 *
 *  Created on: 03/07/2017
 *      Author: david
 */

#include "JDInstrument.h"
#include <TGraph.h>
#include <TMath.h>

using namespace std;

//-----------------------------------------------
//
//	This is the constructor.
//
//	Possible variables are:
// 	sInstrumentName 	= (TString) name of the telescope
//	dWobble			 	= (Double_t) wobble distance
//	sMyInstrumentPath	= (TString) name of the instrument path
JDInstrument::JDInstrument(TString instrumentName, Double_t wobble, TString myInstrumentPath):
		sInstrumentName(instrumentName), dWobble(wobble), sMyInstrumentPath(myInstrumentPath),
		gEpsilon(NULL),fEvaluateEfficiencyVsTheta(NULL),
		fEvaluateEpsilonVsThetaAndPhi(NULL),fEvaluateEpsilonPerThetaVsThetaAndPhi(NULL)
{
	    cout << endl;
		cout << endl;
		cout << "Constructor Instrument..." << endl;
		cout << endl;
		cout << endl;

	CreateFunctionsInstrument();
}

//-----------------------------------------------
// This is the destructor
// It deletes the functions in order not to be reused
JDInstrument::~JDInstrument()
{

	if (gEpsilon)										delete gEpsilon;
	if (fEvaluateEfficiencyVsTheta)						delete fEvaluateEfficiencyVsTheta;
	if (fEvaluateEpsilonVsThetaAndPhi)					delete fEvaluateEpsilonVsThetaAndPhi;
	if (fEvaluateEpsilonPerThetaVsThetaAndPhi)			delete fEvaluateEpsilonPerThetaVsThetaAndPhi;

		cout << endl;
		cout << endl;
		cout << "Destructor Instrument..." << endl;
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
SetEpsilon();

dThetaMax= dDcc-dWobble;

fEvaluateEfficiencyVsTheta = new TF1("fEvaluateEfficiencyVsTheta", this, &JDInstrument::EvaluateEfficiencyVsTheta, 0., dThetaMax, 0, "JDInstrument", "EvaluateEfficiencyVsTheta");

fEvaluateEpsilonVsThetaAndPhi = new TF2("fEvaluateEpsilonVsThetaAndPhi", this, &JDInstrument::EvaluateEpsilonVsThetaAndPhi, 0., dThetaMax, -TMath::Pi(), TMath::Pi(), 0, "JDInstrument", "EvaluateEpsilonVsThetaAndPhi");
fEvaluateEpsilonPerThetaVsThetaAndPhi = new TF2("fEvaluateEpsilonPerThetaVsThetaAndPhi", this, &JDInstrument::EvaluateEpsilonPerThetaVsThetaAndPhi, 0., dThetaMax, -TMath::Pi(), TMath::Pi(), 0, "JDInstrument", "EvaluateEpsilonPerThetaVsThetaAndPhi");

}

//-----------------------------------------------
// This function fulfills a TGraph with the information  of the epsilon of the telescope normalized at the centre of the camera. We define epsilon as the % of the quality of the camera with respect to its centre.
// dDcc (Double_t) is the distance to the centre of the camera
void JDInstrument::SetEpsilon()
{
	Double_t Y,Y0;
	Int_t contador = 0;

	gEpsilon = new TGraph();

	ifstream file (sMyInstrumentPath);
	while(file 	>> dDcc >> Y)
		{
			if (contador==0){
			Y0=Y;
			}

			gEpsilon->SetPoint(contador,dDcc,(Y/Y0));

			contador++;
		}
	file.close();
}

//-----------------------------------------------
//	It evaluates the Epsilon of the camera vs theta and phi
//
//	dccR distance to the centre of the camera in radial components
//	x[0] = theta [deg]
//  x[1] = phi [rad]
//  par[0] = wobble [deg]
Double_t JDInstrument::EvaluateEpsilonVsThetaAndPhi(Double_t* x, Double_t* par)
{
	Double_t dccR = TMath::Power(TMath::Power(dWobble,2)+TMath::Power(x[0],2)+2*dWobble*x[0]*TMath::Cos(x[1]),0.50);

	if (dccR<=dDcc)
	{
		return gEpsilon->Eval(dccR);
	}

	else
	{
		return 1.e-20;
	}
}

//-----------------------------------------------
//	It evaluates the Epsilon of the camera multiplied by theta vs theta and phi
//
//	x[0] = theta [deg]
//  x[1] = phi [rad]
Double_t JDInstrument::EvaluateEpsilonPerThetaVsThetaAndPhi(Double_t* x, Double_t* par)
{
	return fEvaluateEpsilonVsThetaAndPhi->Eval(x[0], x[1])*x[0];
}

//-----------------------------------------------
//	It evaluates the Efficiency of the camera vs theta
//
//	x[0] = theta [deg]
Double_t JDInstrument::EvaluateEfficiencyVsTheta(Double_t* x, Double_t* par)
{
	return fEvaluateEpsilonPerThetaVsThetaAndPhi->Integral(0., x[0], -TMath::Pi(), TMath::Pi(), 1.e-4)/(TMath::Pi()*TMath::Power(x[0],2));
}

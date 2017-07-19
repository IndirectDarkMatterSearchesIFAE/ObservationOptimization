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
JDInstrument::JDInstrument(TString instrumentName, Double_t wobble):
		sInstrumentName(instrumentName), dWobble(wobble),
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
//This is the destructor
//It deletes the functions in order not to be reused
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
//This function calls the important functions of this class
//
//dThetaMax (Double_t) is used in Efficiency and EpsilonPerTheta so as to stop the function as soon as the evaluation gets out of the camera
void JDInstrument::CreateFunctionsInstrument()
{
SetEpsilon();

dThetaMax= dDcc-dWobble;

fEvaluateEfficiencyVsTheta = new TF1("fEvaluateEfficiencyVsTheta", this, &JDInstrument::EvaluateEfficiencyVsTheta, 0., dThetaMax, 0, "JDInstrument", "EvaluateEfficiencyVsTheta");

fEvaluateEpsilonVsThetaAndPhi = new TF2("fEvaluateEpsilonVsThetaAndPhi", this, &JDInstrument::EvaluateEpsilonVsThetaAndPhi, 0., dDcc, -TMath::Pi(), TMath::Pi(), 0, "JDInstrument", "EvaluateEpsilonVsThetaAndPhi");
fEvaluateEpsilonPerThetaVsThetaAndPhi = new TF2("fEvaluateEpsilonPerThetaVsThetaAndPhi", this, &JDInstrument::EvaluateEpsilonPerThetaVsThetaAndPhi, 0., dThetaMax, -TMath::Pi(), TMath::Pi(), 0, "JDInstrument", "EvaluateEpsilonPerThetaVsThetaAndPhi");

}

//-----------------------------------------------
//This function fulfills a TGraph with the information  of the epsilon of the telescope normalized at the centre of the camera. We define epsilon as the % of the quality of the camera with respect to its centre.
//dDcc (Double_t) is the distance to the centre of the camera
void JDInstrument::SetEpsilon()
{
	TString myPath= "/home/david/Documents/DarkMatter/Epsilon/"+sInstrumentName+".txt";
	Double_t Y,Y0;
	Int_t contador = 0;

	gEpsilon = new TGraph();

	ifstream file (myPath);
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
//	dccR distance to the centre of the camera in radial components
//	x[0] = theta
//  x[1] = phi
//  par[0] = wobble
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
//	x[0] = theta
//  x[1] = phi
Double_t JDInstrument::EvaluateEpsilonPerThetaVsThetaAndPhi(Double_t* x, Double_t* par)
{
	return fEvaluateEpsilonVsThetaAndPhi->Eval(x[0], x[1])*x[0];
}

//-----------------------------------------------
//	x[0] = theta
Double_t JDInstrument::EvaluateEfficiencyVsTheta(Double_t* x, Double_t* par)
{
	return fEvaluateEpsilonPerThetaVsThetaAndPhi->Integral(0., x[0], -TMath::Pi(), TMath::Pi(), 1.e-6)/(TMath::Pi()*TMath::Power(x[0],2));
}



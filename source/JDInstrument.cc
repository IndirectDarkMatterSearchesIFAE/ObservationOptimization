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

using namespace std;

//-----------------------------------------------
//
//	This is the constructor.
//
//	Possible variables are:
// 	sInstrumentName 	= (TString) name of the telescope
//	dWobble			 	= (Double_t) wobble distance (QUIM) UNITS!!!
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

// (QUIM) dDcc no te un valor definit...[segueix a **1]
dThetaMax= dDcc-dWobble;

fEvaluateEfficiencyVsTheta = new TF1("fEvaluateEfficiencyVsTheta", this, &JDInstrument::EvaluateEfficiencyVsTheta, 0., dThetaMax, 0, "JDInstrument", "EvaluateEfficiencyVsTheta");

fEvaluateEpsilonVsThetaAndPhi = new TF2("fEvaluateEpsilonVsThetaAndPhi", this, &JDInstrument::EvaluateEpsilonVsThetaAndPhi, 0., dThetaMax, -TMath::Pi(), TMath::Pi(), 0, "JDInstrument", "EvaluateEpsilonVsThetaAndPhi");
fEvaluateEpsilonPerThetaVsThetaAndPhi = new TF2("fEvaluateEpsilonPerThetaVsThetaAndPhi", this, &JDInstrument::EvaluateEpsilonPerThetaVsThetaAndPhi, 0., dThetaMax, -TMath::Pi(), TMath::Pi(), 0, "JDInstrument", "EvaluateEpsilonPerThetaVsThetaAndPhi");

}

//-----------------------------------------------
// This function fills a TGraph with the information of the epsilon
// of the telescope normalized at the centre of the camera.
// We define epsilon as the % of the quality of the camera with respect
// to its centre.
//
// dDcc (Double_t) is the distance to the centre of the camera
//
// (QUIM) Epsilon és un concepte que nosaltre inventem (hauriem de parlar sempre
// d'acceptancia, i epsilon es la lletra que utilitzem per definir la acceptancia)
// Quan parlem d'acceptancies, parlarem de la de MAGIC, CTA i ideal (i potser més endavant
// d'altres. Per tant, dintre de SetEpsilon, hi hauria d'haver diferents casos, que
// segons l'instrument, hi hagués una acceptancia different.
void JDInstrument::SetEpsilon()
{
	Double_t Y,Y0;
	Int_t contador = 0;

	gEpsilon = new TGraph();

	ifstream file (sMyInstrumentPath);
	// (QUIM) [**1] va canviant de valor dintre del while fins a la última iteració...
	// segurament aquí vols definir una variable local "distanceCameraCenter" que es destrueixi
	// després de SetEpsilon

	// (QUIM) This is the MAGIC case.
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


	if (dccR<=dDcc)
	{
		return gEpsilon->Eval(dccR);
	}

	else	// (QUIM) this is just to make integrals over the camera to converge right?
	{
		return 1.e-20;
	}
}

//-----------------------------------------------
//	It evaluates the Epsilon of the camera multiplied by theta vs theta and phi
//	(QUIM) - Why are you multiplying by theta? Area = int dphy int r·dr ; where theta is r.
//
//	x[0] = theta [deg]
//  x[1] = phi [rad]
Double_t JDInstrument::EvaluateEpsilonPerThetaVsThetaAndPhi(Double_t* x, Double_t* par)
{
	return fEvaluateEpsilonVsThetaAndPhi->Eval(x[0], x[1])*x[0];
}

//-----------------------------------------------
//	It evaluates the Efficiency of the camera vs theta
//	(QUIM) what is efficiency, see slide 3
//
//	x[0] = theta [deg]
Double_t JDInstrument::EvaluateEfficiencyVsTheta(Double_t* x, Double_t* par)
{
	return fEvaluateEpsilonPerThetaVsThetaAndPhi->Integral(0., x[0], -TMath::Pi(), TMath::Pi(), 1.e-4)/(TMath::Pi()*TMath::Power(x[0],2));
}

/*
 * Instrument.cc
 *
 *  Created on: 03/07/2017
 *      Author: david
 */

//#include "Instrument.h" This works home
#include "Instrument.h"
#include <TGraph.h>
#include <TMath.h>

using namespace std;

JDInstrument::JDInstrument(TString instrumentName, Double_t wobble):
		sInstrumentName(instrumentName), dWobble(wobble)
{
	    cout << endl;
		cout << endl;
		cout << "Constructor Instrument..." << endl;
		cout << endl;
		cout << endl;

	CreateFunctionsInstrument();
}

JDInstrument::~JDInstrument()
{

		cout << endl;
		cout << endl;
		cout << "Destructor Instrument..." << endl;
		cout << endl;
		cout << endl;
}

void JDInstrument::CreateFunctionsInstrument()
{
SetEpsilon();

dThetaMax= dDcc-dWobble;

fEvaluateEfficiencyVsTheta = new TF1("fEvaluateEfficiencyVsTheta", this, &JDInstrument::EvaluateEfficiencyVsTheta, 0., dThetaMax, 0, "JDInstrument", "EvaluateEfficiencyVsTheta");

fEvaluateEpsilonVsTheta = new TF2("fEvaluateEpsilonVsTheta", this, &JDInstrument::EvaluateEpsilonVsTheta, 0., dDcc, -TMath::Pi(), TMath::Pi(), 0, "JDInstrument", "EvaluateEpsilonVsTheta");
fEvaluateEpsilonPerThetaVsTheta = new TF2("fEvaluateEpsilonPerThetaVsTheta", this, &JDInstrument::EvaluateEpsilonPerThetaVsTheta, 0., dThetaMax, -TMath::Pi(), TMath::Pi(), 0, "JDInstrument", "EvaluateEpsilonPerThetaVsTheta");

}

void JDInstrument::SetEpsilon()
{

	TString myPath= "/home/david/Documents/DarkMatter/Epsilon/"+sInstrumentName+".txt";
	Double_t Y,Y0;
	Int_t contador = 0;


	gEpsilon = new TGraph();

	ifstream file (myPath);
	while(!file.eof())
		{
			file 	>> dDcc >> Y;

			if (contador==0){
			Y0=Y;
			}

			gEpsilon->SetPoint(contador,dDcc,(Y/Y0));

			contador++;
		}
	file.close();

}

Double_t JDInstrument::EvaluateEpsilonVsTheta(Double_t* x, Double_t* par)
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

Double_t JDInstrument::EvaluateEpsilonPerThetaVsTheta(Double_t* x, Double_t* par)
{
	return fEvaluateEpsilonVsTheta->Eval(x[0], x[1])*x[0];
}

Double_t JDInstrument::EvaluateEfficiencyVsTheta(Double_t* x, Double_t* par)
{

	return fEvaluateEpsilonPerThetaVsTheta->Integral(0., x[0], -TMath::Pi(), TMath::Pi(), 1.e-6)/(TMath::Pi()*TMath::Power(x[0],2));

}



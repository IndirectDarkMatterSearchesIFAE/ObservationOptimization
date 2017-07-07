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

Instrument::Instrument(TString InstrumentName, Double_t Wobble):
		sInstrumentName(InstrumentName), dWobble(Wobble)
{
	    cout << endl;
		cout << endl;
		cout << "Constructor Instrument..." << endl;
		cout << endl;
		cout << endl;

	CreateFunctionsInstrument();
}

Instrument::~Instrument()
{

		cout << endl;
		cout << endl;
		cout << "Destructor Instrument..." << endl;
		cout << endl;
		cout << endl;
}

void Instrument::CreateFunctionsInstrument()
{
SetEpsilon();

dThetaMax= dDcc-dWobble;

fEfficiency = new TF1("fEfficiency", this, &Instrument::dEfficiency, 0., dThetaMax, 0, "Instrument", "dEfficiency");

fEpsilon = new TF2("fEpsilon", this, &Instrument::dEpsilon, 0., dDcc, -TMath::Pi(), TMath::Pi(), 0, "Instrument", "dEpsilon");
fEpsilonPerTheta = new TF2("fEpsilonPerTheta", this, &Instrument::dEpsilonPerTheta, 0., dThetaMax, -TMath::Pi(), TMath::Pi(), 0, "Instrument", "dEpsilonPerTheta");

}

void Instrument::SetEpsilon()
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

Double_t Instrument::dEpsilon(Double_t* x, Double_t* par)
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

Double_t Instrument::dEpsilonPerTheta(Double_t* x, Double_t* par)
{
	return fEpsilon->Eval(x[0], x[1])*x[0];
}

Double_t Instrument::dEfficiency(Double_t* x, Double_t* par)
{

	return fEpsilonPerTheta->Integral(0., x[0], -TMath::Pi(), TMath::Pi(), 1.e-6)/(TMath::Pi()*TMath::Power(x[0],2));

}



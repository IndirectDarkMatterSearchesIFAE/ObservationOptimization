/*
 * Instrument.cc
 *
 *  Created on: 03/07/2017
 *      Author: david
 */

//#include "Instrument.h" This works home
#include <Instrument.h>
#include <TGraph.h>
#include <TMath.h>

using namespace std;

Instrument::Instrument()
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

fEpsilon = new TF1("fEpsilon", this, &Instrument::dEpsilon, 0., dDccMax, 0, "Instrument", "dEpsilon");
fEfficiency = new TF1("fEfficiency", this, &Instrument::dEfficiency, 0., dDccMax, 0, "Instrument", "dEfficiency");

}

void Instrument::SetEpsilon()
{

	sInstrumentName="CrabNebula,Post-upgrade";
	dWobble=0.;

	TString myPath="/home/david/Documents/DarkMatter/Eficiència/"+sInstrumentName+".txt";
//	TString myPath="/home/david/Escriptori/IFAE/DarkMatter/Eficiència/"+sInstrumentName+".txt";    This works home
	Double_t X,Y,Y0;
	Int_t contador = 0;


	gEpsilon = new TGraph();

	ifstream file (myPath);
	while(!file.eof())
		{
			file 	>> X >> Y;

			if (contador==0){
			Y0=Y;
			}

			gEpsilon->SetPoint(contador,X,(Y/Y0));

			contador++;
		}
	file.close();
	dDccMax=X;
}

Double_t Instrument::dEpsilon(Double_t* x, Double_t* par)
{
	return gEpsilon->Eval(x[0]);
}

Double_t Instrument::dEfficiency(Double_t* x, Double_t* par)
{
	Double_t dccR = TMath::Power(TMath::Power(dWobble,2)+TMath::Power(x[0],2)+2*dWobble*x[0]*TMath::Cos(x[1]),0.50);
	return dccR;
}



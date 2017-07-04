/*
 * Instrument.cc
 *
 *  Created on: 03/07/2017
 *      Author: david
 */

#include "Instrument.h"
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
SetEfficiency();

fEfficiency = new TF1("fEfficiency", this, &Instrument::dEfficiency, 0., dDccMax,0, "Instrument", "dEfficiency");

}

void Instrument::SetEfficiency()
{

	sInstrumentName="CrabNebula,Post-upgrade";

	TString myPath="/home/david/Escriptori/IFAE/DarkMatter/Eficiència/"+sInstrumentName+".txt";
	Double_t X,Y,Y0;
	Int_t contador = 0;


	gEfficiency = new TGraph();

	ifstream file (myPath);
	while(!file.eof())
		{
			file 	>> X >> Y;

			if (contador==0){
			Y0=Y;
			}

			gEfficiency->SetPoint(contador,X,(Y/Y0));

			contador++;
		}
	file.close();
	dDccMax=X;	// dDistMaxCentreCam
}

Double_t Instrument::dEfficiency(Double_t* x, Double_t* par)
{
	return gEfficiency->Eval(x[0]);
}



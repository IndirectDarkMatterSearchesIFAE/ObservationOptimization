/*
 * DM.cc
 *
 *  Created on: 03/07/2017
 *      Author: david
 */

#include "DM.h"
#include <TGraph.h>
#include <TMath.h>

using namespace std;

static const Double_t SolarMass2GeV = 1.1154e57;  			// [GeV/SolarM]
static const Double_t kpc2cm        = 3.08568e21; 			// [cm/kpc]

DM::DM()
{
	    cout << endl;
		cout << endl;
		cout << "Constructor DM..." << endl;
		cout << endl;
		cout << endl;

	CreateFunctionsDM();
}

DM::~DM()
{
		cout << endl;
		cout << endl;
		cout << "Destructor DM..." << endl;
		cout << endl;
		cout << endl;
}

void DM::CreateFunctionsDM()
{
SetJFactor();
}

void DM::SetJFactor()
{
	//Define a candidate and a source

	sCandidate = "UMa2";
	sSource = "Bonnivard";

	const TString myPath = "/home/david/Escriptori/IFAE/DarkMatter/Eficiència/"+sSource+"/"+sCandidate+".txt";

	Double_t theta, J, J_m1, J_p1, J_m2, J_p2;
	Int_t contador = 0;

	gJFactor = new TGraph();

	if (sSource == "Bonnivard")
	{

	ifstream file (myPath);
	while(!file.eof())
		{
			file >> theta >> J >> J_m1 >> J_p1 >> J_m2 >> J_p2;

			gJFactor->SetPoint(contador,theta,(J*(TMath::Power(SolarMass2GeV,1)/TMath::Power(kpc2cm,2))));
			contador ++;
		}

	file.close();

	}
}


//Provaré a fer el LOS amb una TF1 directament


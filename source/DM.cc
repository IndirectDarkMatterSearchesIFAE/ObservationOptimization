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

fJFactor = new TF1("fJFactor",this,&DM::dJFactor,0.,dTheta,1,"DM","JFactor");
fQFactor = new TF1("fQFactor", this, &DM::dQFactor, 0., dTheta, 1, "DM", "QFactor");

}

void DM::SetJFactor()
{
	//Define a candidate and a source

	sCandidate = "UMa2all";
	sSource = "Bonnivard";

	const TString myPath = "/home/david/Escriptori/IFAE/DarkMatter/Eficiència/"+sSource+"/"+sCandidate+".txt";

	Double_t dJ, dJ_m1, dJ_p1, dJ_m2, dJ_p2;
	Int_t contador = 0;

	gJFactor = new TGraph();

	if (sSource == "Bonnivard")
	{

	ifstream file (myPath);
	while(!file.eof())
		{
			file >> dTheta >> dJ >> dJ_m1 >> dJ_p1 >> dJ_m2 >> dJ_p2;

			gJFactor->SetPoint(contador,dTheta,(dJ*(TMath::Power(SolarMass2GeV,1)/TMath::Power(kpc2cm,2))));

			contador ++;
		}

	file.close();

	}
}


Double_t DM::dJFactor(Double_t* x, Double_t* par)
{
 return gJFactor->Eval(x[0]);
}

Double_t DM::dQFactor(Double_t* x, Double_t* par)
{
 return (gJFactor->Eval(x[0])/x[0])/(gJFactor->Eval(0.1)/0.1);
}






//Provaré a fer el LOS amb una TF1 directament


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

fJFactor = new TF1("fJFactor",this,&DM::dJFactor,0.,dTheta,0,"DM","dJFactor");
fQFactor = new TF1("fQFactor", this, &DM::dQFactor, 0., dTheta, 0, "DM", "dQFactor");
fLOS = new TF1("fLOS", this, &DM::dLOS, 0., dTheta, 0, "DM", "dLOS");
fLOSPerSin = new TF1("fLOSPerSin", this, &DM::dLOSPerSin, 0., dTheta, 0, "DM", "dLOSPerSin");
fJFactorFromLOS = new TF1("fJFactorFromLOS", this, &DM::dJFactorFromLOS, 0., dTheta, 0, "DM", "dJFactorFromLOS");

}

void DM::SetJFactor()
{
	//Define a candidate and a source

	sCandidate = "UMa2all";
	sSource = "Bonnivard";

//	const TString myPath = "/home/david/Escriptori/IFAE/DarkMatter/Eficiència/"+sSource+"/"+sCandidate+".txt";
	const TString myPath = "/home/david/Documents/DarkMatter/Eficiència/"+sSource+"/"+sCandidate+".txt";

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

Double_t DM::dLOS(Double_t* x, Double_t* par)
{
	return fJFactor->Derivative(x[0])/(2*TMath::Pi()*TMath::Sin(x[0]));
}

Double_t DM::dLOSPerSin(Double_t* x, Double_t* par)
{

	Double_t thetaRad = x[0]*(TMath::Pi()/180);

	return fLOS->Eval(x[0])*TMath::Sin(thetaRad);
}


Double_t DM::dJFactorFromLOS(Double_t* x, Double_t* par)
{
	return 2*TMath::Pi()*fLOSPerSin->Integral(0., x[0]);

}




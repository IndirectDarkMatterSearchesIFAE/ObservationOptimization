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

}

void DM::SetJFactor()
{
	//Define a candidate, a source and a form

	sCandidate = "uma2";
	sSource = "Bonnivard";
	sForm = "Decay";

	Int_t contador = 0;

	gJFactor = new TGraph();

	if (sForm == "Decay"){

		if (sSource == "Bonnivard")
		{

			const TString myPath = "/home/david/Documents/DarkMatter/"+sSource+"/"+sCandidate+"_Dalphaint_cls_READ.output";

			Double_t dJ, dJ_m1, dJ_p1, dJ_m2, dJ_p2;

			ifstream file (myPath);
			while(!file.eof())
				{
					file >> dTheta >> dJ >> dJ_m1 >> dJ_p1 >> dJ_m2 >> dJ_p2;

					gJFactor->SetPoint(contador,dTheta,(dJ*(TMath::Power(SolarMass2GeV,1)/TMath::Power(kpc2cm,2))));

					contador ++;
				}

			file.close();

		}

		else if (sSource == "Geringer")
		{

			const TString myPath = "/home/david/Documents/DarkMatter/"+sSource+"/GeringerSamethTable_"+sCandidate+".txt";

			TString name;
			Double_t LogJann2m, LogJann1m, LogJann, LogJann1p, LogJann2p;
			Double_t LogJdec2m, LogJdec1m, LogJdec, LogJdec1p, LogJdec2p;
			Double_t a,b,c,d,e,f,g,h,i,j;

			ifstream file (myPath);
			while(!file.eof())
			{
				file	>> name >> dTheta
						>> LogJann2m >> LogJann1m >> LogJann >> LogJann1p >> LogJann2p
						>> LogJdec2m >> LogJdec1m >> LogJdec >> LogJdec1p >> LogJdec2p
						>> a >> b >> c >> d >> e >> f >> g >> h >> i >> j;

				gJFactor->SetPoint(contador, dTheta, TMath::Power(10., LogJdec));

				contador ++;
			}
			file.close();
		}
	}

	else if(sForm =="Annihilation")
	{
		if (sSource == "Bonnivard")
		{

			const TString myPath = "/home/david/Documents/DarkMatter/"+sSource+"/"+sCandidate+"_Jalphaint_cls_READ.output";

			Double_t dJ, dJ_m1, dJ_p1, dJ_m2, dJ_p2;

			ifstream file (myPath);
			while(!file.eof())
				{
					file >> dTheta >> dJ >> dJ_m1 >> dJ_p1 >> dJ_m2 >> dJ_p2;

					gJFactor->SetPoint(contador,dTheta,(dJ*(TMath::Power(SolarMass2GeV,2)/TMath::Power(kpc2cm,5))));

					contador ++;
				}

			file.close();

		}

		else if (sSource == "Geringer")
		{

			const TString myPath = "/home/david/Documents/DarkMatter/"+sSource+"/GeringerSamethTable_"+sCandidate+".txt";

			TString name;
			Double_t LogJann2m, LogJann1m, LogJann, LogJann1p, LogJann2p;
			Double_t LogJdec2m, LogJdec1m, LogJdec, LogJdec1p, LogJdec2p;
			Double_t a,b,c,d,e,f,g,h,i,j;

			ifstream file (myPath);
			while(!file.eof())
			{
				file	>> name >> dTheta
						>> LogJann2m >> LogJann1m >> LogJann >> LogJann1p >> LogJann2p
						>> LogJdec2m >> LogJdec1m >> LogJdec >> LogJdec1p >> LogJdec2p
						>> a >> b >> c >> d >> e >> f >> g >> h >> i >> j;

				gJFactor->SetPoint(contador, dTheta, TMath::Power(10., LogJann));

				contador ++;
			}
			file.close();
		}
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




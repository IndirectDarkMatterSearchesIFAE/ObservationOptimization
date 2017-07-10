/*
 * DM.cc
 *
 *  Created on: 03/07/2017
 *      Author: david
 */

#include "JDDarkMatter.h"
#include <TGraph.h>
#include <TMath.h>

using namespace std;

static const Double_t SolarMass2GeV = 1.1154e57;  			// [GeV/SolarM]
static const Double_t kpc2cm        = 3.08568e21; 			// [cm/kpc]

// NOMENCLATURA
//	1) que fa la funcio
//	2) que necessita com a input
//	3) que dona com a output
//	4) definir variables/paràmetres interns
//		x[0], x[1],..., x[n] -> variables
//		par[0], par[1],..., par[n] -> parameters
//


//-----------------------------------------------
//
//	This is the constructor.
//
//	Possible variables are:
//	sCandiate 	= (TString) name of dark matter halo
// 	sSource 	= (TString) name of author (puto david canvieu!)
//
JDDarkMatter::JDDarkMatter(TString candidate, TString source, TString form):
		sCandidate(candidate), sSource(source), sForm(form)
//afegir totes les variables variable(NULL)
{
	    cout << endl;
		cout << endl;
		cout << "Constructor DM..." << endl;
		cout << endl;
		cout << endl;

	CreateFunctionsDM();
}

//-----------------------------------------------
JDDarkMatter::~JDDarkMatter()
{
	// delete everything

		cout << endl;
		cout << endl;
		cout << "Destructor DM..." << endl;
		cout << endl;
		cout << endl;

}

//-----------------------------------------------
//fJFactor -> fTF1EvaluateJFactorVsTheta; JFactor [~GeV,~cm] theta [deg]
void JDDarkMatter::CreateFunctionsDM()
{

	SetJFactor(); // [Units]

	fJFactor = new TF1("fJFactor",this,&JDDarkMatter::dJFactor,0.,dTheta,0,"DM","dJFactor");
	fQFactor = new TF1("fQFactor", this, &JDDarkMatter::dQFactor, 0., dTheta, 0, "DM", "dQFactor");

}

//-----------------------------------------------
// [Units]
// Form -> Bool_t IsAnnihilation = 0 or 1
void JDDarkMatter::SetJFactor()
{
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
			// Una funcio apart
			// ReadJDecGeringer = ReadJAnnGeringer
			// ReadJDecBonnivard = ReadJAnnBonnivard


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

//-----------------------------------------------
// x[0] = dTheta
// nOM HORRIBLE
// dJFactor -> TGraphEvaluateJFactorVsTheta
Double_t JDDarkMatter::dJFactor(Double_t* x, Double_t* par)
{
 return gJFactor->Eval(x[0]);
}

//----------------------------------------------------
// x[0] = dTheta
Double_t JDDarkMatter::dQFactor(Double_t* x, Double_t* par)
{
 return (gJFactor->Eval(x[0])/x[0])/(gJFactor->Eval(0.1)/0.1);
}

//----------------------------------------------------
// Line of sight
// x[0]
//Double_t DM::dQFactor(Double_t* x, Double_t* par)
//{
// return (gJFactor->Eval(x[0])/x[0])/(gJFactor->Eval(0.1)/0.1);
//}



/*
 * JDDM.cc
 *
 *  Created on: 03/07/2017
 *      Author: david
 */

#include "JDDarkMatter.h"
#include <TGraph.h>
#include <TMath.h>

using namespace std;

const static double Deg2Rad = TMath::Pi()/180.;
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
// 	sAuthor 	= (TString) name of author
//	sSource 	= (TString) name of dark matter halo
// 	sCandidate 	= (TString) type of signal
JDDarkMatter::JDDarkMatter(TString author, TString source, TString candidate):
		sAuthor(author), sSource(source), sCandidate(candidate),
		gJFactor(NULL), fEvaluateJFactorVsTheta(NULL), fEvaluateQFactorVsTheta(NULL)
{
	    cout << endl;
		cout << endl;
		cout << "Constructor DM..." << endl;
		cout << endl;
		cout << endl;

	CreateFunctionsDM();
}

//-----------------------------------------------
//
//	This is the destructor.
//It deletes the functions in order not to be reused
JDDarkMatter::~JDDarkMatter()
{
	if (gJFactor)								delete gJFactor;
	if (fEvaluateJFactorVsTheta)				delete fEvaluateJFactorVsTheta;
	if (fEvaluateQFactorVsTheta)				delete fEvaluateQFactorVsTheta;


		cout << endl;
		cout << endl;
		cout << "Destructor DM..." << endl;
		cout << endl;
		cout << endl;

}

//-----------------------------------------------
//	This function creates the important functions of this class. The functions are:
//	SetJFactor() Let us choose which ReadJFactor use depending on the chosen author
//	fEvaluateJFactorVsTheta -> TF1 that evaluates the JFactor vs Theta; JFactor [~GeV,~cm]     theta [deg]
//	fEvaluateQFactorVsTheta -> TF1 that evaluates the QFactor vs Theta; QFactor [~GeV,~cm deg] theta [deg]
void JDDarkMatter::CreateFunctionsDM()
{
	SetJFactor(); // [Units]

	fEvaluateJFactorVsTheta = new TF1("fEvaluateJFactorVsTheta",this,&JDDarkMatter::TGraphEvaluateJFactorVsTheta,0.,dTheta,0,"JDDarkMatter","TGraphEvaluateJFactorVsTheta");
	fEvaluateQFactorVsTheta = new TF1("fEvaluateQFactorVsTheta", this, &JDDarkMatter::EvaluateQFactorVsTheta, 0., dTheta, 1, "JDDarkMatter", "EvaluateQFactorVsTheta");

	fEvaluateLOSVsTheta = new TF1("fEvaluateLOSVsTheta", this, &JDDarkMatter::EvaluateLOSVsTheta, 0., dTheta, 0, "JDDarkMatter", "EvaluateLOSVsTheta");
	fEvaluateLOSPerSinusVsTheta = new TF1("fEvaluateLOSPerSinusVsTheta", this, &JDDarkMatter::EvaluateLOSPerSinusVsTheta, 0., dTheta, 0, "JDDarkMatter", "EvaluateLOSPerSinusVsTheta");
	fEvaluateJFactorFromLOSVsTheta= new TF1("fEvaluateJFactorFromLOSVsTheta", this, &JDDarkMatter::EvaluateJFactorFromLOSVsTheta, 0., dTheta, 0, "JDDarkMatter", "EvaluateJFactorFromLOSVsTheta");
}

//-----------------------------------------------
//	This function creates the two ReadJFactor functions.
// 	It redirects to other function depending on which author we are taking into account.
void JDDarkMatter::SetJFactor()
{

	if (sAuthor == "Bonnivard")
	{
		ReadJFactorBonnivard();
	}

	else if (sAuthor == "Geringer")
	{
		ReadJFactorGeringer();
	}

	else
	{
		cout<<"ERROR: Author not valid"<<endl;
	}
}


//-----------------------------------------------
//	This function reads the JFactor data of Bonnivard
//	It fulfills a TGraph with this data
void JDDarkMatter::ReadJFactorBonnivard()
{

	Int_t contador = 0;

	gJFactor = new TGraph();

	TString myPath;
	Double_t dJ, dJ_m1, dJ_p1, dJ_m2, dJ_p2;
	Double_t exp, exp1;

		if (sCandidate == "Decay")
		{
			myPath = "/home/david/Documents/DarkMatter/"+sAuthor+"/"+sSource+"_Dalphaint_cls_READ.output";
			exp = 1.;
			exp1 = 2.;
		}

		else if(sCandidate =="Annihilation")
		{
			myPath = "/home/david/Documents/DarkMatter/"+sAuthor+"/"+sSource+"_Jalphaint_cls_READ.output";
			exp = 2.;
			exp1 = 5.;
		}

		else
		{
			cout<<"ERROR: Candidate not valid"<<endl;
			cout<<"Possibilities are: DECAY or ANNIHILATION"<<endl;
		}

		ifstream file (myPath);
		while(file >> dTheta >> dJ >> dJ_m1 >> dJ_p1 >> dJ_m2 >> dJ_p2)
			{
			gJFactor->SetPoint(contador,dTheta,(dJ*(TMath::Power(SolarMass2GeV,exp)/TMath::Power(kpc2cm,exp1))));

			contador ++;
			}

		file.close();
}

//-----------------------------------------------
//	This function reads the JFactor data of Geringer
//	It fulfills a TGraph with this data
void JDDarkMatter::ReadJFactorGeringer()
{
	Int_t contador = 0;

	gJFactor = new TGraph();

	TString name;
	Double_t LogJann2m, LogJann1m, LogJann, LogJann1p, LogJann2p;
	Double_t LogJdec2m, LogJdec1m, LogJdec, LogJdec1p, LogJdec2p;
	Double_t a,b,c,d,e,f,g,h,i,j;

	const TString myPath = "/home/david/Documents/DarkMatter/"+sAuthor+"/GeringerSamethTable_"+sSource+".txt";

	if (sCandidate == "Decay")
	{
		ifstream file (myPath);
		while(file	>> name >> dTheta
				>> LogJann2m >> LogJann1m >> LogJann >> LogJann1p >> LogJann2p
				>> LogJdec2m >> LogJdec1m >> LogJdec >> LogJdec1p >> LogJdec2p
				>> a >> b >> c >> d >> e >> f >> g >> h >> i >> j)
			{
				gJFactor->SetPoint(contador, dTheta, TMath::Power(10., LogJdec));

				contador ++;
			}
			file.close();
	}

	else if (sCandidate == "Annihilation")
	{
		ifstream file (myPath);
		while(file	>> name >> dTheta
				>> LogJann2m >> LogJann1m >> LogJann >> LogJann1p >> LogJann2p
				>> LogJdec2m >> LogJdec1m >> LogJdec >> LogJdec1p >> LogJdec2p
				>> a >> b >> c >> d >> e >> f >> g >> h >> i >> j)
			{
				gJFactor->SetPoint(contador, dTheta, TMath::Power(10., LogJann));

				contador ++;
			}
			file.close();	}

	else
	{
		cout<<"ERROR: Candidate is not valid"<<endl;
		cout<<"Possibilities are: DECAY or ANNIHILATION"<<endl;
	}
}

//-----------------------------------------------
// x[0] 	= dTheta
Double_t JDDarkMatter::TGraphEvaluateJFactorVsTheta(Double_t* x, Double_t* par)
{
	return gJFactor->Eval(x[0]);

}

//----------------------------------------------------
// x[0] 	= dTheta
// par[0] 	= point of normalization
Double_t JDDarkMatter::EvaluateQFactorVsTheta(Double_t* x, Double_t* par)
{
 return (gJFactor->Eval(x[0])/x[0])/(gJFactor->Eval(par[0])/par[0]);
}

//----------------------------------------------------
// Line of sight
// x[0]
Double_t JDDarkMatter::EvaluateLOSVsTheta(Double_t* x, Double_t* par)
{
	return fEvaluateJFactorVsTheta->Derivative(x[0])/(2*TMath::Pi()*TMath::Sin(x[0]*Deg2Rad));
}

//----------------------------------------------------
// Line of sight
// x[0]
Double_t JDDarkMatter::EvaluateLOSPerSinusVsTheta(Double_t* x, Double_t* par)
{
	return  fEvaluateLOSVsTheta->Eval(x[0])*TMath::Sin(x[0]*Deg2Rad);
}
//----------------------------------------------------
// Line of sight
// x[0]

Double_t JDDarkMatter::EvaluateJFactorFromLOSVsTheta(Double_t* x, Double_t* par)
{
	return 2*TMath::Pi()*(fEvaluateLOSPerSinusVsTheta->Integral(0., x[0], 1.e-4));
}



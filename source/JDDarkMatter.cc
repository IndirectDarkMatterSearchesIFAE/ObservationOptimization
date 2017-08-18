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


//-----------------------------------------------
//
//	This is the constructor.
//
//	Possible variables are:
// 	sAuthor 	= (TString) name of author
//	sSource 	= (TString) name of dark matter halo
// 	sCandidate 	= (TString) type of signal
//  sMyPath     = (TString) name of the path
//
//	It redirects us to CreateFunctionDM()
//
JDDarkMatter::JDDarkMatter(TString author, TString source, TString candidate, TString mySourcePath):
		sAuthor(author), sSource(source), sCandidate(candidate), sMySourcePath (mySourcePath),
		gJFactor(NULL), fEvaluateJFactorVsTheta(NULL), fEvaluateLOSVsTheta(NULL)
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
//  It deletes the functions in order not to be reused
JDDarkMatter::~JDDarkMatter()
{
	if (gJFactor)								delete gJFactor;
	if (fEvaluateJFactorVsTheta)				delete fEvaluateJFactorVsTheta;
	if (fEvaluateLOSVsTheta)					delete fEvaluateLOSVsTheta;

		cout << endl;
		cout << endl;
		cout << "Destructor DM..." << endl;
		cout << endl;
		cout << endl;
}

//-----------------------------------------------
//	This function creates the important functions of this class. The functions are:
//	SetJFactor() Depending on the chosen author redirects us to different ReadJFactor functions
//	fEvaluateJFactorVsTheta -> TF1 that evaluates the JFactor vs Theta; JFactor [~GeV,~cm]     theta [deg]
//	fEvaluateLOSVsTheta -> TF1 that evaluates the LOS vs Theta; LOS [~GeV, ~cm] theta [deg]
void JDDarkMatter::CreateFunctionsDM()
{
	SetJFactor();

	fEvaluateJFactorVsTheta = new TF1("fEvaluateJFactorVsTheta",this,&JDDarkMatter::TGraphEvaluateJFactorVsTheta,0.,dTheta,0,"JDDarkMatter","TGraphEvaluateJFactorVsTheta");
	fEvaluateLOSVsTheta = new TF1("fEvaluateLOSVsTheta", this, &JDDarkMatter::EvaluateLOSVsTheta, 0., dTheta, 0, "JDDarkMatter", "EvaluateLOSVsTheta");

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

}


//-----------------------------------------------
//	This function reads the JFactor data of Bonnivard
//	It fulfills a TGraph with this data
//	It allows to distinguish between Decay or Annihilation
void JDDarkMatter::ReadJFactorBonnivard()
{

	Int_t contador = 0;

	gJFactor = new TGraph();

	Double_t dJ, dJ_m1, dJ_p1, dJ_m2, dJ_p2;

		if (sCandidate == "Decay")
		{
			ifstream file (sMySourcePath);
					while(file >> dTheta >> dJ >> dJ_m1 >> dJ_p1 >> dJ_m2 >> dJ_p2)
						{
						gJFactor->SetPoint(contador,dTheta,(dJ*(TMath::Power(SolarMass2GeV,1.)/TMath::Power(kpc2cm,2.))));

						contador ++;
						}

					file.close();
		}

		else if(sCandidate =="Annihilation")
		{
			ifstream file (sMySourcePath);
				while(file >> dTheta >> dJ >> dJ_m1 >> dJ_p1 >> dJ_m2 >> dJ_p2)
					{
					gJFactor->SetPoint(contador,dTheta,(dJ*(TMath::Power(SolarMass2GeV,2.)/TMath::Power(kpc2cm,5.))));

					contador ++;
					}

				file.close();
		}

		else
		{
			cout<<"ERROR: Candidate not valid"<<endl;
			cout<<"Possibilities are: DECAY or ANNIHILATION"<<endl;
		}
}

//-----------------------------------------------
//	This function reads the JFactor data of Geringer
//	It fulfills a TGraph with this data
//	It allows to distinguish between Decay or Annihilation
void JDDarkMatter::ReadJFactorGeringer()
{
	Int_t contador = 0;

	gJFactor = new TGraph();

	TString name;
	Double_t LogJann2m, LogJann1m, LogJann, LogJann1p, LogJann2p;
	Double_t LogJdec2m, LogJdec1m, LogJdec, LogJdec1p, LogJdec2p;
	Double_t a,b,c,d,e,f,g,h,i,j;

	if (sCandidate == "Decay")
	{
		ifstream file (sMySourcePath);
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
		ifstream file (sMySourcePath);
		while(file	>> name >> dTheta
				>> LogJann2m >> LogJann1m >> LogJann >> LogJann1p >> LogJann2p
				>> LogJdec2m >> LogJdec1m >> LogJdec >> LogJdec1p >> LogJdec2p
				>> a >> b >> c >> d >> e >> f >> g >> h >> i >> j)
			{
				gJFactor->SetPoint(contador, dTheta, TMath::Power(10., LogJann));

				contador ++;
			}
			file.close();
	}

	else
	{
		cout<<"ERROR: Candidate is not valid"<<endl;
		cout<<"Possibilities are: DECAY or ANNIHILATION"<<endl;
	}
}

//-----------------------------------------------
// It evaluates the JFactor TGraph vs Theta
//
// x[0] 	= dTheta
Double_t JDDarkMatter::TGraphEvaluateJFactorVsTheta(Double_t* x, Double_t* par)
{
	return gJFactor->Eval(x[0]);
}

//----------------------------------------------------
// It evaluates the LOS vs Theta. The LOS is calculated from the derivative of the JFactor divided by 2*PI*Sin(theta)
//
// x[0]		= dTheta
Double_t JDDarkMatter::EvaluateLOSVsTheta(Double_t* x, Double_t* par)
{
	return fEvaluateJFactorVsTheta->Derivative(x[0])/(2*TMath::Pi()*TMath::Sin(x[0]*Deg2Rad));
}

/*
 * JDDM.cc
 *
 *  Created on: 03/07/2017
 *  Authors: David Navarro Gironés 	<<david.navarrogir@e-campus.uab.cat>>
 *  		 Joaquim Palacio 		<<jpalacio@ifae.es>>
 *
 *  		 ADD A GENERAL DESCRIPTION ON THE CLASS, THE MAIN FUNCTIONS, THE VARIABLES
 *  		 AND MENTION THE "runExample#.C" THAT SHOWS HOW TO USE IT
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
// 	sAuthor 	= (TString) name of author 				### (QUIM) Geringer or Bonnivard
//	sSource 	= (TString) name of dark matter halo	### (QUIM) provide a list of the possible sources¿?
// 	sCandidate 	= (TString) type of signal				### (QUIM) Annihilation or Decay
//  sMyPath     = (TString) name of the path			### (QUIM) Explain what is this path used for...
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

	// (QUIM) couts are only shown if there are errors.
	// maybe some information could be given to the user, examples:
	// "Bonnivard JFactor for decay read correctly!!"
}

//-----------------------------------------------
//
//	This is the destructor.
//  It deletes the functions in order not to be reused
JDDarkMatter::~JDDarkMatter()
{
	//(QUIM) Javi deletes all TF(?), not sure whether we need to delete gJactor,
	// but probably doesn't hurt.
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
//	void SetJFactor(): 				redirects us to "void ReadJFactor...()" (different functions depending on the chosen author)
//	TF1 fEvaluateJFactorVsTheta: 	evaluates the JFactor vs Theta; JFactor [~GeV,~cm]     theta [deg]
//	TF1 fEvaluateLOSVsTheta: 		evaluates the LOS vs Theta; LOS [~GeV, ~cm] theta [deg]
void JDDarkMatter::CreateFunctionsDM()
{
	SetJFactor();

	fEvaluateJFactorVsTheta = new TF1("fEvaluateJFactorVsTheta",this,&JDDarkMatter::TGraphEvaluateJFactorVsTheta,0.,dTheta,0,"JDDarkMatter","TGraphEvaluateJFactorVsTheta");
	// (QUIM) intenta que TF1 y Double_t es diguin igual (example TF1* fNomDeLaFuncio -> Double_T dNomDeLaFuncio)
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
//
// 	### (QUIM) Specify Bonnivard reference
// 	### (QUIM) Specify details of the TGraph (for example TGraph(npoint,theta[deg],Jfactor [~GeV,~cm])
//	### (QUIM) The number of points of the TGraph is it automatically by the reference. Would it be usefull to safe it?
//	### (QUIM) Declare dTheta, dJ, dJ_m1, dJ_p1, dJ_m2, dJ_p2

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

					// (QUIM) SetContadorValue(contador)¿?
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

				// (QUIM) SetContadorValue(contador)¿?
				file.close();
		}

		else
		{
			cout<<"ERROR: Candidate not valid"<<endl;
			cout<<"Possibilities are: DECAY or ANNIHILATION"<<endl;
			// (QUIM) Is not a priority, but if this happens, the code should stop.
			//	maybe defining this function as Int_t and return 0 if ok, or -1 if bad...
		}
}

//-----------------------------------------------
//	This function reads the JFactor data of Geringer
//	It fulfills a TGraph with this data
//	It allows to distinguish between Decay or Annihilation
// 	### (QUIM) Specify Geringer reference
// 	### (QUIM) Specify details of the TGraph (for example TGraph(npoint,theta[deg],Jfactor [~GeV,~cm])
//	### (QUIM) The number of points of the TGraph is it automatically by the reference. Would it be usefull to safe it?
//	### (QUIM) Declare dTheta, LogJann2m, LogJann1m, LogJann, LogJann1p, LogJann2p
//	### (QUIM) Declare LogJdec2m, LogJdec1m, LogJdec, LogJdec1p, LogJdec2p
//	### (QUIM) Declare a,b,c,d,e,f,g,h,i,j
void JDDarkMatter::ReadJFactorGeringer()
{
	Int_t contador = 0;

	// (QUIM) both cases Decay/Annihilation are the same,
	// it would be better to add the if inside one of the whiles (**1),
	// and to reduce the number of lines of the function.

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
				// (**1) if((sCandidate == "Decay")
				gJFactor->SetPoint(contador, dTheta, TMath::Power(10., LogJdec));
				// else
				// gJFactor->SetPoint(contador, dTheta, TMath::Power(10., LogJann));

				contador ++;
			}
			// (QUIM) SetContadorValue(contador)¿?
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
			// (QUIM) SetContadorValue(contador)¿?
			// (QUIM) Why is dTheta a variable of the class? Probably better "SetThetaMax(dTheta); {dThetaMax=dTheta};"¿?
			file.close();
	}

	else
	{
		cout<<"ERROR: Candidate is not valid"<<endl;
		cout<<"Possibilities are: DECAY or ANNIHILATION"<<endl;
	}
}

//-----------------------------------------------
// It evaluates the TGraph JFactor [(QUIM) units] vs Theta
//
// x[0] 	= dTheta [(QUIM) units]
Double_t JDDarkMatter::TGraphEvaluateJFactorVsTheta(Double_t* x, Double_t* par)
{
	return gJFactor->Eval(x[0]);
}

//----------------------------------------------------
// It evaluates the LOS vs Theta. The LOS is calculated from the derivative of the JFactor divided by 2*PI*Sin(theta)
//
// x[0]		= dTheta
Double_t JDDarkMatter::EvaluateLOSVsTheta(Double_t* x, Double_t* par)
// (QUIM) Double_t JDDarkMatter::EvaluateLOSVsThetaDerivative(Double_t* x, Double_t* par)
{
	return fEvaluateJFactorVsTheta->Derivative(x[0])/(2*TMath::Pi()*TMath::Sin(x[0]*Deg2Rad));
	// (QUIM) Proof that Derivative() works fine [see comments in document]
}

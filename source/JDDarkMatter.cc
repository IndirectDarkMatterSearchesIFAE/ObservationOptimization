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
#include <Rtypes.h>
#include <stddef.h>
#include <fstream>
#include <iostream>
using namespace std;

const static double Deg2Rad = TMath::Pi()/180.;
static const Double_t SolarMass2GeV = 1.1154e57;  			// [GeV/SolarM]
static const Double_t kpc2cm        = 3.08568e21; 			// [cm/kpc]


//-----------------------------------------------
//
//	This is the constructor is new (QUIM)
JDDarkMatter::JDDarkMatter(TGraph* jfactor):
		sAuthor(""), sSource(""), sCandidate(""), sMySourcePath (""),
		gJFactor(NULL), fEvaluateJFactorVsTheta(NULL), fEvaluateLOSVsTheta(NULL)
{
	cout << endl;
	cout << endl;
	cout << "Constructor DM..." << endl;
	cout << endl;
	cout << endl;

	if(SetJFactorFromTGraph(jfactor))
	{
		cout << "   ***********************************" << endl;
		cout << "   ***                             ***" << endl;
		cout << "   ***   JFactor could not be set  ***" << endl;
		cout << "   ***                             ***" << endl;
		cout << "   ***********************************" << endl;
		return;
	}
	CreateFunctionsDM();
}


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

	if(SetJFactorFromReferences())
	{
		cout << "   ****************************************************" << endl;
		cout << "   ***                                              ***" << endl;
		cout << "   ***   JFactor could not be read from references  ***" << endl;
		cout << "   ***                                              ***" << endl;
		cout << "   ****************************************************" << endl;
		return;
	}
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
//	SetJFactorFromTGraph();

	fEvaluateJFactorVsTheta = new TF1("fEvaluateJFactorVsTheta",this,&JDDarkMatter::TGraphEvaluateJFactorVsTheta,0.,GetThetaMax(),0,"JDDarkMatter","TGraphEvaluateJFactorVsTheta");
	// (QUIM) intenta que TF1 y Double_t es diguin igual (example TF1* fNomDeLaFuncio -> Double_T dNomDeLaFuncio)
	fEvaluateLOSVsTheta = new TF1("fEvaluateLOSVsTheta", this, &JDDarkMatter::EvaluateLOSVsTheta, 0., GetThetaMax() , 0, "JDDarkMatter", "EvaluateLOSVsTheta");
	fEvaluateNormLOSVsTheta = new TF1("fEvaluateNormLOSVsTheta", this, &JDDarkMatter::EvaluateNormLOSVsTheta, 0., GetThetaMax() , 1, "JDDarkMatter", "EvaluateNormLOSVsTheta");

}

//-----------------------------------------------
//	New (QUIM)
Bool_t JDDarkMatter::SetJFactorFromTGraph(TGraph* jfactor)
{
	gJFactor =jfactor;

	SetNumPointsJFactorGraph((Int_t)gJFactor->GetN());
	if(GetNumPointsJFactorGraph()<=0) return -1;

	Int_t numPoint = GetNumPointsJFactorGraph();
	SetJFactorMin(gJFactor->GetY()[0]);
	SetJFactorMax(gJFactor->GetY()[numPoint-1]);
	SetThetaMax(gJFactor->GetX()[numPoint-1]);

	return 0;
}

//-----------------------------------------------
//	This function creates the two ReadJFactor functions.
// 	It redirects to other function depending on which author we are taking into account.
Bool_t JDDarkMatter::SetJFactorFromReferences(Bool_t verbose)
{

	if (GetAuthor() == "Bonnivard")
	{
		cout << "   "<< endl;
		cout << "   Reading JFactor from: "<< endl;
		cout << "   Dark matter annihilation and decay in dwarf spheroidal galaxies: The classical and ultrafaint dSphs" << endl;
		cout << "   Bonnivard et al., " << endl;
		cout << "   https://arxiv.org/abs/1504.02048" << endl;
		cout << "   "<< endl;

		SetIsBonnivard(1);

		ReadJFactorBonnivard(verbose);
		return 0;
	}
	else if (GetAuthor() == "Geringer")
	{
		cout << "   "<< endl;
		cout << "   Reading JFactor from: "<< endl;
		cout << "   Dwarf galaxy annihilation and decay emission profiles for dark matter experiments" << endl;
		cout << "   Geringer-Sameth et al., " << endl;
		cout << "   https://arxiv.org/abs/1408.0002" << endl;
		cout << "   "<< endl;

		SetIsGeringer(1);

		ReadJFactorGeringer(verbose);
		return 0;
	}

	return -1;
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

void JDDarkMatter::ReadJFactorBonnivard(Bool_t verbose)
{
	Int_t contador = 0;

	gJFactor = new TGraph();

	Double_t dJ, dJ_m1, dJ_p1, dJ_m2, dJ_p2;
	Double_t theta; // [deg]

	if (verbose==1)
		{
			cout << " " << endl;
			cout << "    GetSourcePath() = " << GetSourcePath() << endl;
			cout << " " << endl;
		}


		if (GetCandidate() == "Decay")
		{
			ifstream file (GetSourcePath()+"references/JFactor/Bonnivard/"+GetSourceName()+"_Dalphaint_cls_READ.output");
					while(file >> theta >> dJ >> dJ_m1 >> dJ_p1 >> dJ_m2 >> dJ_p2)
					{
						gJFactor->SetPoint(contador,theta,(dJ*(TMath::Power(SolarMass2GeV,1.)/TMath::Power(kpc2cm,2.))));

						// only for Tests
						if (verbose==1) cout << theta << " " << dJ*(TMath::Power(SolarMass2GeV,1.)/TMath::Power(kpc2cm,2.)) << endl;
						if(contador==0) SetJFactorMin(dJ*(TMath::Power(SolarMass2GeV,1.)/TMath::Power(kpc2cm,2.)));

						contador ++;
					}

					SetNumPointsJFactorGraph(contador);
					SetJFactorMax(dJ*(TMath::Power(SolarMass2GeV,1.)/TMath::Power(kpc2cm,2.)));
					SetThetaMax(theta);
					file.close();
		}

		else if(GetCandidate() == "Annihilation")
		{
			ifstream file (GetSourcePath()+"/references/JFactor/Bonnivard/"+GetSourceName()+"_Jalphaint_cls_READ.output");
				while(file >> theta >> dJ >> dJ_m1 >> dJ_p1 >> dJ_m2 >> dJ_p2)
				{
					gJFactor->SetPoint(contador,theta,(dJ*(TMath::Power(SolarMass2GeV,2.)/TMath::Power(kpc2cm,5.))));

					// only for Tests
					if (verbose==1) cout << theta << " " << dJ*(TMath::Power(SolarMass2GeV,2.)/TMath::Power(kpc2cm,5.)) << endl;
					if(contador==0) SetJFactorMin(dJ*(TMath::Power(SolarMass2GeV,2.)/TMath::Power(kpc2cm,5.)));

					contador ++;
				}

				SetNumPointsJFactorGraph(contador);
				SetJFactorMax(dJ*(TMath::Power(SolarMass2GeV,2.)/TMath::Power(kpc2cm,5.)));
				SetThetaMax(theta);
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
void JDDarkMatter::ReadJFactorGeringer(Bool_t verbose)
{
	Int_t contador = 0;

	if (verbose==1)
		{
			cout << " " << endl;
			cout << "    GetSourcePath() = " << GetSourcePath() << endl;
			cout << " " << endl;
		}

	// (QUIM) both cases Decay/Annihilation are the same,
	// it would be better to add the if inside one of the whiles (**1),
	// and to reduce the number of lines of the function.

	gJFactor = new TGraph();

	TString name;
	Double_t LogJann2m, LogJann1m, LogJann, LogJann1p, LogJann2p;
	Double_t LogJdec2m, LogJdec1m, LogJdec, LogJdec1p, LogJdec2p;
	Double_t a,b,c,d,e,f,g,h,i,j;
	Double_t theta; // [deg]

	ifstream file (GetSourcePath()+"references/JFactor/GeringerSameth/GeringerSamethTable_"+GetSourceName()+".txt");
	while(file	>> name >> theta
			>> LogJann2m >> LogJann1m >> LogJann >> LogJann1p >> LogJann2p
			>> LogJdec2m >> LogJdec1m >> LogJdec >> LogJdec1p >> LogJdec2p
			>> a >> b >> c >> d >> e >> f >> g >> h >> i >> j)
		{
			if(GetCandidate() == "Decay")				gJFactor->SetPoint(contador, theta, TMath::Power(10., LogJdec));
			else if (GetCandidate() == "Annihilation")	gJFactor->SetPoint(contador, theta, TMath::Power(10., LogJann));
			else
				{
					cout<<"ERROR: Candidate is not valid"<<endl;
					cout<<"Possibilities are: DECAY or ANNIHILATION"<<endl;
					break;
				}
			contador ++;
		}
		SetNumPointsJFactorGraph(contador);
		SetThetaMax(theta);
		file.close();

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

//----------------------------------------------------
// new (QUIM
Double_t JDDarkMatter::EvaluateNormLOSVsTheta(Double_t* x, Double_t* par)
{
	return fEvaluateJFactorVsTheta->Derivative(x[0])/fEvaluateJFactorVsTheta->Derivative(par[0]);
}

void JDDarkMatter::GetListOfCandidates()
{
	cout << " " << endl;
	cout << "    List of available candidates is:" << endl;
	cout << "    	- Annihilation" << endl;
	cout << "    	- Decay" << endl;
	cout << " " << endl;

}
void JDDarkMatter::GetListOfSources()
{
	if(GetAuthor()=="Bonnivard")
	{
		cout << " " << endl;
		cout << "    List of available sources for Bonnivard is:" << endl;
		cout << "    	- ..." << endl;
		cout << "    	- (To Be Filled) " << endl;
		cout << " " << endl;
	}
	else if(GetAuthor()=="Geringer")
	{
		cout << " " << endl;
		cout << "    List of available sources for Geringer-Sameth is:" << endl;
		cout << "    	- ..." << endl;
		cout << "    	- (To Be Filled) " << endl;
		cout << " " << endl;
	}
	else
	{
		cout << " " << endl;
		cout << "    Author not defined, no sources available." << endl;
		cout << "    You can define your own JFactors using the constructor:" << endl;
		cout << " 	 	- JDDarkMatter(TGraph* jfactor);" << endl;
		cout << " " << endl;
	}
}
void JDDarkMatter::GetListOfAuthors()
{

}

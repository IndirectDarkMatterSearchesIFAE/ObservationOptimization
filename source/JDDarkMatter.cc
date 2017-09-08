/*
 * JDDM.cc
 *
 *  Created on: 03/07/2017
 *  Authors: David Navarro Gironés 	<<david.navarro.girones@gmail.com>>
 *  		 Joaquim Palacio 		<<jpalacio@ifae.es>>
 *
 *
 *			 THIS CLASS IS THE ONE RELATED WITH THE DARK MATTER DATA.
 *			 WITH THIS CLASS YOU CAN EVALUATE THE JFACTOR VS THETA, THE LOS VS THETA AND PHI, THE JFACTOR OBTAINED BY INTEGRATING THE LOS
 *  		 VARIABLES:
 *  		 	THETA 	[DEG]
 *  		 	PHI	  	[RAD]
 *  		 	OFFSET	[DEG]
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
#include <Rtypes.h>
#include <TAttFill.h>
#include <TAttLine.h>
#include <TAttText.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TF2.h>
#include <TGraph.h>
#include <TH1.h>
#include <TLegend.h>
#include <TPave.h>
#include <TString.h>
#include <TVirtualPad.h>
#include <TStyle.h>

using namespace std;

static const Double_t SolarMass2GeV = 1.1154e57;  			// [GeV/SolarM]
static const Double_t kpc2cm        = 3.08568e21; 			// [cm/kpc]


//-----------------------------------------------
//
//	This is the constructor used to show the possibilities that offers the JDDarkMatter class
JDDarkMatter::JDDarkMatter():
		sAuthor(""), sSource(""), sCandidate(""), sMySourcePath (""),
		gJFactor(NULL), fEvaluateJFactorVsTheta(NULL), fEvaluateLOSVsTheta(NULL), fEvaluateNormLOSVsTheta(NULL),
		fIntegrateJFactorFromLOSVsTheta(NULL), fIntegrateJFactorOffFromLOSVsTheta(NULL),
		fEvaluateLOSThetaVsThetaPhi(NULL), fEvaluateLOSOffThetaVsThetaPhi(NULL),
		bIsBonnivard(0),bIsGeringer(0),bIsJFactor(0),
		dDeg2Rad(TMath::Pi()/180.)
{
	cout << endl;
	cout << endl;
	cout << "   Constructor JDDarkMatter..." << endl;
	cout << endl;
	cout << endl;

	CreateFunctionsDM();
}

//-----------------------------------------------
//
//	This is the constructor used when the data is given by a TGraph
JDDarkMatter::JDDarkMatter(TGraph* jfactor):
		sAuthor(""), sSource(""), sCandidate(""), sMySourcePath (""),
		gJFactor(NULL), fEvaluateJFactorVsTheta(NULL), fEvaluateLOSVsTheta(NULL), fEvaluateNormLOSVsTheta(NULL),
		fIntegrateJFactorFromLOSVsTheta(NULL), fIntegrateJFactorOffFromLOSVsTheta(NULL),
		fEvaluateLOSThetaVsThetaPhi(NULL), fEvaluateLOSOffThetaVsThetaPhi(NULL),
		bIsBonnivard(0),bIsGeringer(0),bIsJFactor(0),
		dDeg2Rad(TMath::Pi()/180.)
{
	cout << endl;
	cout << endl;
	cout << "   Constructor JDDarkMatter..." << endl;
	cout << endl;
	cout << endl;

	if(!SetJFactorFromTGraph(jfactor))
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
//	This is the constructor used when the data is given by a txtFile
JDDarkMatter::JDDarkMatter(TString txtFile):
		sAuthor(""), sSource(""), sCandidate(""), sMySourcePath (""),
		gJFactor(NULL), fEvaluateJFactorVsTheta(NULL), fEvaluateLOSVsTheta(NULL), fEvaluateNormLOSVsTheta(NULL),
		fIntegrateJFactorFromLOSVsTheta(NULL), fIntegrateJFactorOffFromLOSVsTheta(NULL),
		fEvaluateLOSThetaVsThetaPhi(NULL), fEvaluateLOSOffThetaVsThetaPhi(NULL),
		bIsBonnivard(0),bIsGeringer(0),bIsJFactor(0),
		dDeg2Rad(TMath::Pi()/180.)
{
	cout << endl;
	cout << endl;
	cout << "   Constructor JDDarkMatter..." << endl;
	cout << endl;
	cout << endl;

	if(!SetJFactorFromTxtFile(txtFile))
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
//	This is the constructor used when the references are used.
//
//	Possible variables are:
// 	sAuthor 	= (TString) name of author
//	sSource 	= (TString) name of dark matter halo
// 	sCandidate 	= (TString) type of signal
//  sMyPath     = (TString) name of the path			This is the path where the data is located
//
//	It redirects us to CreateFunctionDM()
//
JDDarkMatter::JDDarkMatter(TString author, TString source, TString candidate, TString mySourcePath):
		sAuthor(author), sSource(source), sCandidate(candidate), sMySourcePath (mySourcePath),
		gJFactor(NULL), fEvaluateJFactorVsTheta(NULL), fEvaluateLOSVsTheta(NULL), fEvaluateNormLOSVsTheta(NULL),
		fIntegrateJFactorFromLOSVsTheta(NULL), fIntegrateJFactorOffFromLOSVsTheta(NULL),
		fEvaluateLOSThetaVsThetaPhi(NULL), fEvaluateLOSOffThetaVsThetaPhi(NULL),
		bIsBonnivard(0),bIsGeringer(0),bIsJFactor(0), bIsMinusSig1(0),
		dDeg2Rad(TMath::Pi()/180.)
{
	cout << endl;
	cout << endl;
	cout << "   Constructor JDDarkMatter..." << endl;
	cout << endl;
	cout << endl;

	if(!SetJFactorFromReferences())
	{
		cout << "   ****************************************************" << endl;
		cout << "   ***                                              ***" << endl;
		cout << "   ***   JFactor could not be read from references  ***" << endl;
		cout << "   ***                                              ***" << endl;
		cout << "   ****************************************************" << endl;
		return;
	}
	CreateFunctionsDM();
}

//-----------------------------------------------
//
//	This is the destructor.
//  It deletes the functions in order not to be reused
JDDarkMatter::~JDDarkMatter()
{

	if (gJFactor)									delete gJFactor;
	if (fEvaluateJFactorVsTheta)					delete fEvaluateJFactorVsTheta;
	if (fEvaluateLOSVsTheta)						delete fEvaluateLOSVsTheta;
	if (fEvaluateNormLOSVsTheta)					delete fEvaluateNormLOSVsTheta;
	if (fIntegrateJFactorFromLOSVsTheta)			delete fIntegrateJFactorFromLOSVsTheta;
	if (fIntegrateJFactorOffFromLOSVsTheta)			delete fIntegrateJFactorOffFromLOSVsTheta;
	if (fEvaluateLOSThetaVsThetaPhi)				delete fEvaluateLOSThetaVsThetaPhi;
	if (fEvaluateLOSOffThetaVsThetaPhi)				delete fEvaluateLOSOffThetaVsThetaPhi;

		cout << endl;
		cout << endl;
		cout << "   Destructor JDDarkMatter..." << endl;
		cout << endl;
		cout << endl;

}

//-----------------------------------------------
//	This function creates the important functions of this class. The functions are:
//	TF1 fEvaluateJFactorVsTheta: 			evaluates the JFactor vs Theta; JFactor [~GeV,~cm]     theta [deg]
//	TF1 fEvaluateLOSVsTheta: 				evaluates the LOS vs Theta; LOS [~GeV, ~cm] 	theta [deg]
//	TF1 fEvaluateNormLOSVsTheta:			evaluates the LOS normalized at a certain parameter vs Theta; LOS [~GeV, ~cm] 	theta [deg]
//	TF1 fIntegrateJFactorFromLOSVsTheta:	integrates the LOS to obtain the JFactor vs Theta; JFactor [~GeV,~cm]     theta [deg]
//	TF1 fIntegrateJFactorOffFromLOSVsTheta:	integrates the LOS on the OFF region to obtain the JFactor vs Theta; JFactor [~GeV,~cm]     theta [deg]
//  TF2 fEvaluateLOSThetaVsThetaPhi:		evaluates the LOS multiplied by Theta vs Theta and Phi; LOS [~GeV, ~cm] 	theta [deg] 	phi[rad]
//  TF2 fEvaluateLOSOffThetaVsThetaPhi:		evaluates the LOS on the OFF multiplied by Theta vs Theta and Phi; LOS [~GeV, ~cm] 	theta [deg] 	phi[rad]
void JDDarkMatter::CreateFunctionsDM()
{

	fEvaluateJFactorVsTheta = new TF1("fEvaluateJFactorVsTheta",this,&JDDarkMatter::TGraphEvaluateJFactorVsTheta,0.,GetThetaMax(),0,"JDDarkMatter","TGraphEvaluateJFactorVsTheta");
	fEvaluateLOSVsTheta = new TF1("fEvaluateLOSVsTheta", this, &JDDarkMatter::EvaluateLOSVsTheta,0.,GetThetaMax(),0, "JDDarkMatter", "EvaluateLOSVsTheta");
	fEvaluateNormLOSVsTheta = new TF1("fEvaluateNormLOSVsTheta", this, &JDDarkMatter::EvaluateNormLOSVsTheta, 0., GetThetaMax() , 1, "JDDarkMatter", "EvaluateNormLOSVsTheta");
	fIntegrateJFactorFromLOSVsTheta = new TF1("fIntegrateJFactorFromLOSVsTheta",this,&JDDarkMatter::IntegrateJFactorFromLOSVsTheta,0.,GetThetaMax(),0,"JDDarkMatter","IntegrateJFactorFromLOSVsTheta");
	fIntegrateJFactorOffFromLOSVsTheta = new TF1("fIntegrateJFactorOffFromLOSVsTheta",this,&JDDarkMatter::IntegrateJFactorOffFromLOSVsTheta,0.,GetThetaMax(),1,"JDDarkMatter","IntegrateJFactorOffFromLOSVsTheta");

	fEvaluateLOSThetaVsThetaPhi = new TF2("fEvaluateLOSThetaVsThetaPhi", this, &JDDarkMatter::EvaluateLOSThetaVsThetaPhi,0.,GetThetaMax(),0.,2*TMath::Pi(),0, "JDDarkMatter", "EvaluateLOSThetaVsThetaPhi");
	fEvaluateLOSOffThetaVsThetaPhi = new TF2("fEvaluateLOSOffThetaVsThetaPhi", this, &JDDarkMatter::EvaluateLOSOffThetaVsThetaPhi,0.,GetThetaMax(),0.,2*TMath::Pi(),1, "JDDarkMatter", "EvaluateLOSOffThetaVsThetaPhi");

}

//-----------------------------------------------
//
//	This boolean is TRUE(1) if the JFactor can be read and FALSE(0) if the JFactor can not be read
//	It fills a TGraph with the data given by the user with another TGraph
//	It sets the maximum and minimum value of the JFactor and the maximum value of theta
//	If the process is correct, the boolean SetIsJFactor is TRUE
Bool_t JDDarkMatter::SetJFactorFromTGraph(TGraph* jfactor, Bool_t verbose)//, Bool_t verbose=0)
{

	gJFactor =jfactor;

	SetNumPointsJFactorGraph((Int_t)gJFactor->GetN());
	if(GetNumPointsJFactorGraph()<=0) return 0;

	Int_t numPoint = GetNumPointsJFactorGraph();
	SetJFactorMin(gJFactor->GetY()[0]);
	SetJFactorMax(gJFactor->GetY()[numPoint-1]);
	SetThetaMax(gJFactor->GetX()[numPoint-1]);

	if (verbose==1)	for(Int_t i=0; i<numPoint;i++){cout << gJFactor->GetX()[i] << " " << gJFactor->GetY()[i] << endl;}

	SetIsJFactor(1);
	return 1;
}

//-----------------------------------------------
//
//	This boolean is TRUE(1) if the JFactor can be read and FALSE(0) if the JFactor can not be read
//	It fills a TGraph with the data given by the user with a txtFile
//	It sets the maximum and minimum value of the JFactor and the maximum value of theta
//	If the process is correct, the boolean SetIsJFactor is TRUE
Bool_t JDDarkMatter::SetJFactorFromTxtFile(TString txtFile, Bool_t verbose)//, Bool_t verbose=0)
{
	Double_t theta, dJ;
	Int_t contador=0;

	gJFactor = new TGraph();

	ifstream file (txtFile);
	while(file >> theta >> dJ)
	{
		// only for Tests
		if (verbose==1)	cout << theta << " " << dJ << endl;

		gJFactor->SetPoint(contador,theta,dJ);


		if(contador==0) SetJFactorMin(dJ);

		contador ++;
	}

	SetNumPointsJFactorGraph(contador);
	SetJFactorMax(dJ);
	SetThetaMax(theta);
	file.close();

	if(GetNumPointsJFactorGraph()<=0) return 0;

	SetIsJFactor(1);
	return 1;
}

//-----------------------------------------------
//	This boolean is TRUE(1) if the JFactor can be read and FALSE(0) if the JFactor can not be read
//	It creates the two ReadJFactor functions, depending on the author.
//	If the process is correct, the boolean SetIsBonnivard(Geringer) is TRUE(FALSE)
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

		ReadJFactorBonnivard();
		return 1;
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

		ReadJFactorGeringer();
		return 1;
	}

	return 0;
}


//-----------------------------------------------
//	This function reads the JFactor data of Bonnivard
//	It fulfills a TGraph (number of points, theta [deg], JFactor[~GeV, ~cm]) with this data
//	It sets the maximum and minimum value of the JFactor and the maximum value of theta
//	It allows to distinguish between Decay or Annihilation
//
//	### (QUIM) The number of points of the TGraph is it automatically by the reference. Would it be usefull to safe it?
//	### (QUIM) Declare dTheta, dJ, dJ_m1, dJ_p1, dJ_m2, dJ_p2

void JDDarkMatter::ReadJFactorBonnivard(Bool_t verbose)
{
	Int_t contador = 0;

	gJFactor = new TGraph();
	gJFactor_m1 = new TGraph();

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
						gJFactor_m1->SetPoint(contador,theta,(dJ_m1*(TMath::Power(SolarMass2GeV,1.)/TMath::Power(kpc2cm,2.))));

						// only for Tests
						if (verbose==1) cout << theta << " " << dJ*(TMath::Power(SolarMass2GeV,1.)/TMath::Power(kpc2cm,2.)) << endl;
						if(contador==0) SetJFactorMin(dJ*(TMath::Power(SolarMass2GeV,1.)/TMath::Power(kpc2cm,2.)));

						contador ++;
					}

					SetNumPointsJFactorGraph(contador);
					SetJFactorMax(dJ*(TMath::Power(SolarMass2GeV,1.)/TMath::Power(kpc2cm,2.)));
					SetThetaMax(theta);
					file.close();
					SetIsJFactor(1);

		}

		else if(GetCandidate() == "Annihilation")
		{
			ifstream file (GetSourcePath()+"/references/JFactor/Bonnivard/"+GetSourceName()+"_Jalphaint_cls_READ.output");
				while(file >> theta >> dJ >> dJ_m1 >> dJ_p1 >> dJ_m2 >> dJ_p2)
				{
					gJFactor->SetPoint(contador,theta,(dJ*(TMath::Power(SolarMass2GeV,2.)/TMath::Power(kpc2cm,5.))));
					gJFactor_m1->SetPoint(contador,theta,(dJ*(TMath::Power(SolarMass2GeV,2.)/TMath::Power(kpc2cm,5.))));

					// only for Tests
					if (verbose==1) cout << theta << " " << dJ*(TMath::Power(SolarMass2GeV,2.)/TMath::Power(kpc2cm,5.)) << endl;
					if(contador==0) SetJFactorMin(dJ*(TMath::Power(SolarMass2GeV,2.)/TMath::Power(kpc2cm,5.)));

					contador ++;
				}

				SetNumPointsJFactorGraph(contador);
				SetJFactorMax(dJ*(TMath::Power(SolarMass2GeV,2.)/TMath::Power(kpc2cm,5.)));
				SetThetaMax(theta);
				file.close();
				SetIsJFactor(1);

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
//	It fulfills a TGraph (number of points, theta [deg], JFactor[~GeV, ~cm]) with this data
//	It sets the maximum and minimum value of the JFactor and the maximum value of theta
//	It allows to distinguish between Decay or Annihilation
//
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

	gJFactor = new TGraph();
	gJFactor_m1 = new TGraph();

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
			if(GetCandidate() == "Decay")
			{
				gJFactor->SetPoint(contador, theta, TMath::Power(10., LogJdec));
				gJFactor_m1->SetPoint(contador, theta, TMath::Power(10., LogJdec1m));
			}

			else if (GetCandidate() == "Annihilation")
			{
				gJFactor->SetPoint(contador, theta, TMath::Power(10., LogJann));
				gJFactor_m1->SetPoint(contador, theta, TMath::Power(10., LogJann1m));
			}
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
		SetIsJFactor(1);

}

//-----------------------------------------------
// It evaluates the TGraph JFactor [~GeV, ~cm] vs Theta [deg]
//
// x[0] 	= dTheta [deg]
Double_t JDDarkMatter::TGraphEvaluateJFactorVsTheta(Double_t* x, Double_t* par)
{
	if (GetIsMinusSig1()==0)
	{
		cout<<"Estic al JFactor"<<endl;
		return gJFactor->Eval(x[0]);
	}
	else
	{
		cout<<"Estic al JFactor_m1"<<endl;
	return gJFactor_m1->Eval(x[0]);
	}
}

//-----------------------------------------------
// It integrates the LOS [~GeV, ~cm] vs Theta [deg] and Phi [rad] multiplied by Theta [deg] in order to obtain the JFactor [~GeV, ~cm]
//
// x[0] 	= dTheta [deg]
Double_t JDDarkMatter::IntegrateJFactorFromLOSVsTheta(Double_t* x, Double_t* par)
{
	return fEvaluateLOSThetaVsThetaPhi->Integral(0.,x[0],0.,2*TMath::Pi(),1e-6);
}

//-----------------------------------------------
// It integrates the LOS [~GeV, ~cm] vs Theta [deg] and Phi [rad] on the OFF region multiplied by Theta [deg] in order to obtain the JFactor [~GeV, ~cm]
//
// x[0] 	= dTheta [deg]
// par[0] 	= offset [deg]
Double_t JDDarkMatter::IntegrateJFactorOffFromLOSVsTheta(Double_t* x, Double_t* par)
{
	fEvaluateLOSOffThetaVsThetaPhi->SetParameter(0,par[0]);
	return fEvaluateLOSOffThetaVsThetaPhi->Integral(0.,x[0],0.,2*TMath::Pi(),1e-6);
}

//----------------------------------------------------
// 	It evaluates the LOS [~GeV, ~cm] vs Theta [deg]
//	The LOS is calculated from the derivative of the JFactor divided by 2*PI*Sin(theta)
//
// 	x[0]		= theta	[deg]
Double_t JDDarkMatter::EvaluateLOSVsTheta(Double_t* x, Double_t* par)
{
	return fEvaluateJFactorVsTheta->Derivative(x[0])/(2*TMath::Pi()*TMath::Sin(x[0]*dDeg2Rad));
}

//----------------------------------------------------
// 	It evaluates the LOS [~GeV, ~cm] vs Theta [deg] normalized with the LOS at a certain Theta
//
// 	x[0]		= theta	[deg]
// par[0] 		= offset [deg]
Double_t JDDarkMatter::EvaluateNormLOSVsTheta(Double_t* x, Double_t* par)
{
	return fEvaluateJFactorVsTheta->Derivative(x[0])/fEvaluateJFactorVsTheta->Derivative(par[0]);
}

////----------------------------------------------------
//// 	It evaluates the LOS vs Theta.
//// 	The LOS is calculated from the derivative of the JFactor divided by 2*PI*Sin(theta)
////
//// 	x[0]		= theta	[deg]
//// 	x[1]		= phi	[rad]
//Double_t JDDarkMatter::EvaluateLOSVsThetaPhi(Double_t* x, Double_t* par)
//{
//	return fEvaluateLOSVsTheta->Eval(x[0]);
//}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  **********************************************************************************************************
//	WARNING: I would say the correct function is LOS multiplied by Sinus Theta and not LOS multiplied by Theta.
//		   	 In case is LOS multiplied by Theta I don't understand why it has to be converted to rad.
//  **********************************************************************************************************
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------
// It evaluates the LOS vs Theta. The LOS is calculated from the derivative of the JFactor divided by 2*PI*Sin(theta)
//
// x[0]		= theta	[deg]
// x[1]		= phi	[rad]
// par[0]	= offset	[deg]
Double_t JDDarkMatter::EvaluateLOSThetaVsThetaPhi(Int_t* a=0, Double_t* x, Double_t* par)
{
	Double_t X0rad =x[0]*dDeg2Rad;	// x[0] in radians

	if (a==0)
	{
	return fEvaluateLOSVsTheta->Eval(x[0])*TMath::Sin(X0rad);//Per què ha d'estar en radians?
	}

	else
	{
	return fEvaluateLOSVsTheta->Eval(x[0])*X0rad;
	}
}

//----------------------------------------------------
// It evaluates the LOS vs Theta. The LOS is calculated from the derivative of the JFactor divided by 2*PI*Sin(theta)
//
// x[0]		= theta	[deg]
// x[1]		= phi	[rad]
// par[0]	= offset	[deg]
Double_t JDDarkMatter::EvaluateLOSOffThetaVsThetaPhi(Int_t* a=0, Double_t* x, Double_t* par)
{
	Double_t X0rad =x[0]/180.*TMath::Pi();	// x[0] in radians

	Double_t distFromHalo=TMath::Sqrt(x[0]*x[0]+par[0]*par[0]-2*x[0]*par[0]*TMath::Cos(x[1])+(TMath::Pi()/2));

	if (a==0)
	{
	return fEvaluateLOSVsTheta->Eval(distFromHalo)*TMath::Sin(X0rad);
	}

	else
	{
	return fEvaluateLOSVsTheta->Eval(distFromHalo)*X0rad;
	}
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
		cout << "    	- boo1" << endl;
		cout << "    	- car" << endl;
		cout << "    	- coma" << endl;
		cout << "    	- cvn1" << endl;
		cout << "    	- cvn2" << endl;
		cout << "    	- for" << endl;
		cout << "    	- her" << endl;
		cout << "    	- leo1" << endl;
		cout << "    	- leo2" << endl;
		cout << "    	- leo4" << endl;
		cout << "    	- leo5" << endl;
		cout << "    	- leot" << endl;
		cout << "    	- scl" << endl;
		cout << "    	- seg1" << endl;
		cout << "    	- seg2" << endl;
		cout << "    	- sex" << endl;
		cout << "    	- uma1" << endl;
		cout << "    	- uma2" << endl;
		cout << "    	- umi" << endl;
		cout << "    	- wil1" << endl;
		cout << " " << endl;
	}
	else if(GetAuthor()=="Geringer")
	{
		cout << " " << endl;
		cout << "    List of available sources for Geringer-Sameth is:" << endl;
		cout << "    	- Bootes" << endl;
		cout << "    	- Carina" << endl;
		cout << "    	- Coma Berenice" << endl;
		cout << "    	- Canes Venatici I" << endl;
		cout << "    	- Canes Venatici II" << endl;
		cout << "    	- Draco" << endl;
		cout << "    	- Fornax" << endl;
		cout << "    	- Hercules" << endl;
		cout << "    	- Leo I" << endl;
		cout << "    	- Leo II" << endl;
		cout << "    	- Leo IV" << endl;
		cout << "    	- Leo V" << endl;
		cout << "    	- Leo T" << endl;
		cout << "    	- Sculptor" << endl;
		cout << "    	- Segue 1" << endl;
		cout << "    	- Sextans" << endl;
		cout << "    	- Ursa Major I" << endl;
		cout << "    	- Ursa Major II" << endl;
		cout << "    	- Ursa Minor" << endl;
		cout << " " << endl;
	}
	else
	{
		cout << " " << endl;
		cout << "    Author not defined, no sources available." << endl;
		GetListOfAuthors();
	}
}

void JDDarkMatter::GetListOfAuthors()
{
	cout << " " << endl;
	cout << "    List of available authors is:" << endl;
	cout << "    	- Bonnivard" << endl;
	cout << "    	- Geringer" << endl;
	cout << " " << endl;
}

void JDDarkMatter::GetUnits()
{
	cout << " " << endl;
	cout << "    All units are given in:" << endl;
	cout << "    	- ~GeV" << endl;
	cout << "    	- ~cm" << endl;
	cout << "    	- ~deg" << endl;
	cout << " " << endl;
}

void JDDarkMatter::GetListOfConstructors()
{
	cout << " " << endl;
	cout << "    List of available constructors is:" << endl;
	cout << "    	- 	JDDarkMatter()" << endl;
	cout << "    	- 	JDDarkMatter(TGraph* jfactor)" << endl;
	cout << "    	- 	JDDarkMatter(TString txtFile)" << endl;
	cout << "    	- 	JDDarkMatter(TString author, TString source, TString candidate, TString mySourcePath)" << endl;
	cout << " " << endl;
}

void JDDarkMatter::GetWarning()
{
	cout << "  *****************************" << endl;
	cout << "  ***" << endl;
	cout << "  ***  WARNING:" << endl;
	cout << "  ***" << endl;
	cout << "  ***  	- 	JFactor not defined..." << endl;
	cout << " " << endl;
	cout << "  *****************************" << endl;

}

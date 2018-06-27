/*
 * JDAstroProfile.cc
 *
 *  Created on: 09/04/2018
 *
 *  Author:  Joaquim Palacio 		<<jpalacio@ifae.es>>
 *
 *
 */

#include "JDAstroProfile.h"

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
//	This is the constructor used to show the possibilities that offers the JDAstroProfile class
//	It redirects us to CreateFunctionDM()
JDAstroProfile::JDAstroProfile():
		fdNdOmegaVsTheta(NULL), fdNdOmegaSigma1VsTheta(NULL),
		fNormdNdOmegaVsTheta(NULL), fNormdNdOmegaSigma1VsTheta(NULL), fIntegratedNdOmegaThetaVsTheta(NULL), fIntegratedNdOmegaSigma1ThetaVsTheta(NULL),
		fIntegratedNdOmegaOffThetaVsTheta(NULL), fIntegratedNdOmegaSigma1OffThetaVsTheta(NULL), fdNdOmegaThetaVsThetaPhi(NULL),
		fdNdOmegaSigma1ThetaVsThetaPhi(NULL), fdNdOmegaOffThetaVsThetaPhi(NULL), fdNdOmegaSigma1OffThetaVsThetaPhi(NULL),
		bIsdNdOmega(0),
		dDeg2Rad(TMath::Pi()/180.), dBinResolution(0.05)
{
	cout << endl;
	cout << endl;
	cout << "   Constructor JDAstroProfile..." << endl;
	cout << endl;
	cout << endl;
}

//-----------------------------------------------
//
//	This is the destructor.
//  It deletes the functions in order not to be reused
JDAstroProfile::~JDAstroProfile()
{

	if (gdNdOmega)											delete gdNdOmega;
	if (fdNdOmegaVsTheta)							delete fdNdOmegaVsTheta;
	if (fdNdOmegaSigma1VsTheta)						delete fdNdOmegaSigma1VsTheta;
	if (fNormdNdOmegaVsTheta)						delete fNormdNdOmegaVsTheta;
	if (fNormdNdOmegaSigma1VsTheta)					delete fNormdNdOmegaSigma1VsTheta;
	if (fIntegratedNdOmegaThetaVsTheta)							delete fIntegratedNdOmegaThetaVsTheta;
	if (fIntegratedNdOmegaSigma1ThetaVsTheta)						delete fIntegratedNdOmegaSigma1ThetaVsTheta;
	if (fIntegratedNdOmegaOffThetaVsTheta)						delete fIntegratedNdOmegaOffThetaVsTheta;
	if (fIntegratedNdOmegaSigma1OffThetaVsTheta)					delete fIntegratedNdOmegaSigma1OffThetaVsTheta;
	if (fdNdOmegaThetaVsThetaPhi)					delete fdNdOmegaThetaVsThetaPhi;
	if (fdNdOmegaSigma1ThetaVsThetaPhi)				delete fdNdOmegaSigma1ThetaVsThetaPhi;
	if (fdNdOmegaOffThetaVsThetaPhi)				delete fdNdOmegaOffThetaVsThetaPhi;
	if (fdNdOmegaSigma1OffThetaVsThetaPhi)				delete fdNdOmegaSigma1OffThetaVsThetaPhi;

		cout << endl;
		cout << endl;
		cout << "   Destructor JDAstroProfile..." << endl;
		cout << endl;
		cout << endl;
}

//-----------------------------------------------
//	This function creates the important functions of this class. The functions are:
//	TF1 fdNdOmegaVsTheta: 					evaluates the dNdOmega vs Theta; dNdOmega [~GeV, ~cm] 	theta [deg]
//	TF1 fdNdOmegaSigma1VsTheta: 				evaluates the dNdOmegaSigma1 vs Theta; dNdOmega [~GeV, ~cm] 	theta [deg]
//	TF1 fNormdNdOmegaVsTheta:				evaluates the dNdOmega normalized at a certain parameter vs Theta; dNdOmega [~GeV, ~cm] 	theta [deg]
//	TF1 fNormdNdOmegaSigma1VsTheta:				evaluates the dNdOmegaSigma1 normalized at a certain parameter vs Theta; dNdOmega [~GeV, ~cm] 	theta [deg]
//	TF1 fIntegratedNdOmegaThetaVsTheta:		integrates the dNdOmega to obtain the dNdOmega vs Theta; dNdOmega [~GeV,~cm]     theta [deg]
//	TF1 fIntegratedNdOmegaSigma1VsTheta:		integrates the dNdOmegaSigma1 to obtain the dNdOmegaSigma1 vs Theta; dNdOmega [~GeV,~cm]     theta [deg]
//	TF1 fIntegratedNdOmegaOffVsTheta:		integrates the dNdOmega on the OFF region to obtain the dNdOmega vs Theta; dNdOmega [~GeV,~cm]     theta [deg]
//	TF1 fIntegratedNdOmegaSigma1OffVsTheta:	integrates the dNdOmegaSigma1 on the OFF region to obtain the dNdOmegaSigma1 on the OFF region vs Theta; dNdOmega [~GeV,~cm]     theta [deg]
//  TF2 fdNdOmegaThetaVsThetaPhi:			evaluates the dNdOmega multiplied by Theta vs Theta and Phi; dNdOmega [~GeV, ~cm] 	theta [deg] 	phi[rad]
//  TF2 fdNdOmegaSigma1ThetaVsThetaPhi:			evaluates the dNdOmegaSigma1 multiplied by Theta vs Theta and Phi; dNdOmega [~GeV, ~cm] 	theta [deg] 	phi[rad]
//  TF2 fdNdOmegaOffThetaVsThetaPhi:			evaluates the dNdOmega on the OFF multiplied by Theta vs Theta and Phi; dNdOmega [~GeV, ~cm] 	theta [deg] 	phi[rad]
//  TF2 fdNdOmegaSigma1OffThetaVsThetaPhi:		evaluates the dNdOmegaSigma1 on the OFF multiplied by Theta vs Theta and Phi; dNdOmega [~GeV, ~cm] 	theta [deg] 	phi[rad]
void JDAstroProfile::CreateFunctionsAP()
{



	fdNdOmegaVsTheta = new TF1("fdNdOmegaVsTheta", this, &JDAstroProfile::dNdOmegaVsTheta,0.,GetThetaMax(),0, "JDAstroProfile", "dNdOmegaVsTheta");
	fdNdOmegaSigma1VsTheta = new TF1("fdNdOmegaSigma1VsTheta", this, &JDAstroProfile::dNdOmegaSigma1VsTheta,0.,GetThetaMax(),0, "JDAstroProfile", "dNdOmegaSigma1VsTheta");

	fdNdOmegaOffVsThetaPhi = new TF2("fdNdOmegaOffVsThetaPhi", this, &JDAstroProfile::dNdOmegaOffVsThetaPhi,0.,GetThetaMax(),0.,2*TMath::Pi(),1, "JDAstroProfile", "dNdOmegaOffVsThetaPhi");
	fdNdOmegaSigma1OffVsThetaPhi = new TF2("fdNdOmegaSigma1OffVsThetaPhi", this, &JDAstroProfile::dNdOmegaSigma1OffVsThetaPhi,0.,GetThetaMax(),0.,2*TMath::Pi(),1, "JDAstroProfile", "dNdOmegaSigma1OffVsThetaPhi");

	fdNdOmegaThetaVsThetaPhi = new TF2("fdNdOmegaThetaVsThetaPhi", this, &JDAstroProfile::dNdOmegaThetaVsThetaPhi,0.,GetThetaMax(),0.,2*TMath::Pi(),0, "JDAstroProfile", "dNdOmegaThetaVsThetaPhi");
	fdNdOmegaSigma1ThetaVsThetaPhi = new TF2("fdNdOmegaSigma1ThetaVsThetaPhi", this, &JDAstroProfile::dNdOmegaSigma1ThetaVsThetaPhi,0.,GetThetaMax(),0.,2*TMath::Pi(),0, "JDAstroProfile", "dNdOmegaSigma1ThetaVsThetaPhi");
	fdNdOmegaOffThetaVsThetaPhi = new TF2("fdNdOmegaOffThetaVsThetaPhi", this, &JDAstroProfile::dNdOmegaOffThetaVsThetaPhi,0.,GetThetaMax(),0.,2*TMath::Pi(),1, "JDAstroProfile", "dNdOmegaOffThetaVsThetaPhi");
	fdNdOmegaSigma1OffThetaVsThetaPhi = new TF2("fdNdOmegaSigma1OffThetaVsThetaPhi", this, &JDAstroProfile::dNdOmegaSigma1OffThetaVsThetaPhi,0.,GetThetaMax(),0.,2*TMath::Pi(),1, "JDAstroProfile", "dNdOmegaSigma1OffThetaVsThetaPhi");

	fIntegratedNdOmegaThetaVsTheta = new TF1("fIntegratedNdOmegaThetaVsTheta",this,&JDAstroProfile::IntegratedNdOmegaThetaVsTheta,0.,GetThetaMax(),0,"JDAstroProfile","IntegratedNdOmegaThetaVsTheta");
	fIntegratedNdOmegaSigma1ThetaVsTheta = new TF1("fIntegratedNdOmegaSigma1ThetaVsTheta",this,&JDAstroProfile::IntegratedNdOmegaSigma1ThetaVsTheta,0.,GetThetaMax(),0,"JDAstroProfile","IntegratedNdOmegaSigma1ThetaVsTheta");
	fIntegratedNdOmegaOffThetaVsTheta = new TF1("fIntegratedNdOmegaOffThetaVsTheta",this,&JDAstroProfile::IntegratedNdOmegaOffThetaVsTheta,0.,GetThetaMax(),1,"JDAstroProfile","IntegratedNdOmegaOffThetaVsTheta");
	fIntegratedNdOmegaSigma1OffThetaVsTheta = new TF1("fIntegratedNdOmegaSigma1OffThetaVsTheta",this,&JDAstroProfile::IntegratedNdOmegaSigma1OffThetaVsTheta,0.,GetThetaMax(),1,"JDAstroProfile","IntegratedNdOmegaSigma1OffThetaVsTheta");



	/////////////////////////// TO DELETE
	fNormdNdOmegaVsTheta = new TF1("fNormdNdOmegaVsTheta", this, &JDAstroProfile::NormdNdOmegaVsTheta, 0., GetThetaMax() , 1, "JDAstroProfile", "NormdNdOmegaVsTheta");
	fNormdNdOmegaSigma1VsTheta = new TF1("fNormdNdOmegaSigma1VsTheta", this, &JDAstroProfile::NormdNdOmegaSigma1VsTheta, 0., GetThetaMax() , 1, "JDAstroProfile", "NormdNdOmegaSigma1VsTheta");

}

//-----------------------------------------------
// It evaluates the TGraph dNdOmega [# · deg^{-2}] vs Theta [deg]
//
// x[0] 	= dTheta [deg]
Double_t JDAstroProfile::TGraphdNdOmegaVsTheta(Double_t* x, Double_t* par)
{
		return gdNdOmega->Eval(x[0]);
}

//-----------------------------------------------
// It evaluates the TGraph dNdOmegaSigma1 [# · deg^{-2}] vs Theta [deg]
//
// x[0] 	= dTheta [deg]
Double_t JDAstroProfile::TGraphdNdOmegaSigma1VsTheta(Double_t* x, Double_t* par)
{
		return gdNdOmegaSigma1->Eval(x[0]);
}

//-----------------------------------------------
// It integrates the dNdOmega [# · deg^{-2}] vs Theta [deg] and Phi [rad] multiplied by Theta [deg] in order to obtain the N(Delta Omega) [#]
//
// x[0] 	= dTheta [deg]
Double_t JDAstroProfile::IntegratedNdOmegaThetaVsTheta(Double_t* x, Double_t* par)
{
	return fdNdOmegaThetaVsThetaPhi->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2);
}

//-----------------------------------------------
// It integrates the dNdOmegaSigma1 [# · deg^{-2}] vs Theta [deg] and Phi [rad] multiplied by Theta [deg] in order to obtain the NSigma1(Delta Omega) [#]
//
// x[0] 	= dTheta [deg]
Double_t JDAstroProfile::IntegratedNdOmegaSigma1ThetaVsTheta(Double_t* x, Double_t* par)
{
	return fdNdOmegaSigma1ThetaVsThetaPhi->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2);
}

//-----------------------------------------------
// It integrates the dN_OFFdOmega [# · deg^{-2}] vs Theta [deg] and Phi [rad] on the OFF region multiplied by Theta [deg] in order to obtain the N_OFF (Delta Omega) [#]
//
// x[0] 	= dTheta [deg]
// par[0] 	= offset distance [deg]
Double_t JDAstroProfile::IntegratedNdOmegaOffThetaVsTheta(Double_t* x, Double_t* par)
{
	fdNdOmegaOffThetaVsThetaPhi->SetParameter(0,par[0]);
	return fdNdOmegaOffThetaVsThetaPhi->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2);
}

//-----------------------------------------------
// It integrates the dN_OFFSigma1dOmega [# · deg^{-2}] vs Theta [deg] and Phi [rad] on the OFF region multiplied by Theta [deg] in order to obtain the N_OFFSigma1 (Delta Omega) [#]
//
// x[0] 	= dTheta [deg]
// par[0] 	= offset distance [deg]
Double_t JDAstroProfile::IntegratedNdOmegaSigma1OffThetaVsTheta(Double_t* x, Double_t* par)
{
	fdNdOmegaSigma1OffThetaVsThetaPhi->SetParameter(0,par[0]);
	return fdNdOmegaSigma1OffThetaVsThetaPhi->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2);
}

//----------------------------------------------------
// 	It evaluates the dNdOmega [~GeV, ~cm] vs Theta [deg]
//
// 	x[0]		= theta	[deg]
Double_t JDAstroProfile::dNdOmegaVsTheta(Double_t* x, Double_t* par)
{
	return gdNdOmega->Eval(x[0]);
}

//----------------------------------------------------
// 	It evaluates the dNdOmegaSigma1 [~GeV, ~cm] vs Theta [deg]
//
// 	x[0]		= theta	[deg]
Double_t JDAstroProfile::dNdOmegaSigma1VsTheta(Double_t* x, Double_t* par)
{
	return gdNdOmegaSigma1->Eval(x[0]);
}

//----------------------------------------------------
// It evaluates the dNdOmega multiplied by Sin(Theta) vs Theta and Phi.
// The dNdOmega can be also multiplied by Theta if we are not considering Spherical Coordinates.
//
// x[0]		= theta	[deg]
// x[1]		= phi	[rad]
// x0rad    = theta [rad]
Double_t JDAstroProfile::dNdOmegaThetaVsThetaPhi(Double_t* x, Double_t* par)
{
	Double_t X0rad =x[0]*dDeg2Rad;

	if (GetIsSphericalCoordinates()==1)
	{
		return fdNdOmegaVsTheta->Eval(x[0])*TMath::Sin(X0rad);
	}
	else
	{
		return fdNdOmegaVsTheta->Eval(x[0])*x[0];
	}
}



//----------------------------------------------------
// It evaluates the dNdOmegaSigma1 multiplied by Sin(Theta) vs Theta and Phi.
// The dNdOmegaSigma1 can be also multiplied by Theta if we are not considering Spherical Coordinates.
//
// x[0]		= theta	[deg]
// x[1]		= phi	[rad]
// x0rad    = theta [rad]
Double_t JDAstroProfile::dNdOmegaSigma1ThetaVsThetaPhi(Double_t* x, Double_t* par)
{
	Double_t X0rad =x[0]*dDeg2Rad;

	if (GetIsSphericalCoordinates()==1)
	{
		return fdNdOmegaSigma1VsTheta->Eval(x[0])*TMath::Sin(X0rad);
	}
	else
	{
		return fdNdOmegaSigma1VsTheta->Eval(x[0])*x[0];
	}
}

//----------------------------------------------------
// It evaluates the dNdOmega on the OFF region vs Theta and Phi.
// The dNdOmega on the OFF region can be also multiplied by Theta if we are not considering Spherical Coordinates.
//
// x[0]		= theta	[deg]
// x[1]		= phi	[rad]
// par[0]	= offset	[deg]
// x0rad    = theta [rad]
// distFromHalo = distance from the center of the halo [deg] (Calculated from the law of cosines)
Double_t JDAstroProfile::dNdOmegaOffVsThetaPhi(Double_t* x, Double_t* par)
{
	Double_t distCenterSource = TMath::Sqrt(x[0]*x[0]+par[0]*par[0]-2*x[0]*par[0]*TMath::Cos(x[1]+(TMath::Pi()/2)));

	if (GetIsSphericalCoordinates()==1)
	{
		return fdNdOmegaVsTheta->Eval(distCenterSource);
	}
	else
	{
		return fdNdOmegaVsTheta->Eval(distCenterSource);
	}
}

//----------------------------------------------------
// It evaluates the dNdOmega_sig1 on the OFF region vs Theta and Phi.
// The dNdOmega on the OFF region can be also multiplied by Theta if we are not considering Spherical Coordinates.
//
// x[0]		= theta	[deg]
// x[1]		= phi	[rad]
// x0rad    = theta [rad]
// distFromHalo = distance from the center of the halo [deg] (Calculated from the law of cosines)
Double_t JDAstroProfile::dNdOmegaSigma1OffVsThetaPhi(Double_t* x, Double_t* par)
{
	Double_t distCenterSource = TMath::Sqrt(x[0]*x[0]+par[0]*par[0]-2*x[0]*par[0]*TMath::Cos(x[1]+(TMath::Pi()/2)));

	if (GetIsSphericalCoordinates()==1)
	{
		return fdNdOmegaSigma1VsTheta->Eval(distCenterSource);
	}
	else
	{
		return fdNdOmegaSigma1VsTheta->Eval(distCenterSource);
	}
}

//----------------------------------------------------
// It evaluates the dNdOmega on the OFF region multiplied by Sinus(Theta) vs Theta and Phi.
// The dNdOmega on the OFF region can be also multiplied by Theta if we are not considering Spherical Coordinates.
//
// x[0]		= theta	[deg]
// x[1]		= phi	[rad]
// par[0]	= offset	[deg]
// x0rad    = theta [rad]
// distFromHalo = distance from the center of the halo [deg] (Calculated from the law of cosines)
Double_t JDAstroProfile::dNdOmegaOffThetaVsThetaPhi(Double_t* x, Double_t* par)
{
	Double_t X0rad =x[0]*dDeg2Rad;

	Double_t distCenterSource = TMath::Sqrt(x[0]*x[0]+par[0]*par[0]-2*x[0]*par[0]*TMath::Cos(x[1]+(TMath::Pi()/2)));

	if (GetIsSphericalCoordinates()==1)
	{
		return fdNdOmegaVsTheta->Eval(distCenterSource)*TMath::Sin(X0rad);
	}
	else
	{
		return fdNdOmegaVsTheta->Eval(distCenterSource)*x[0];
	}
}

//----------------------------------------------------
// It evaluates the dNdOmegaSigma1 on the OFF region multiplied by Sinus(Theta) vs Theta and Phi.
// The dNdOmegaSigma1 on the OFF region can be also multiplied by Theta if we are not considering Spherical Coordinates.
//
// x[0]		= theta	[deg]
// x[1]		= phi	[rad]
// par[0]	= offset	[deg]
// x0rad    = theta [rad]
// distFromHalo = distance from the center of the halo [deg] (Calculated from the law of cosines)
Double_t JDAstroProfile::dNdOmegaSigma1OffThetaVsThetaPhi(Double_t* x, Double_t* par)
{
	Double_t X0rad =x[0]/180.*TMath::Pi();

	Double_t distCenterSource=TMath::Sqrt(x[0]*x[0]+par[0]*par[0]-2*x[0]*par[0]*TMath::Cos(x[1]+(TMath::Pi()/2)));

	if (GetIsSphericalCoordinates()==1)
	{
		return fdNdOmegaSigma1VsTheta->Eval(distCenterSource)*TMath::Sin(X0rad);
	}
	else
	{
		return fdNdOmegaSigma1VsTheta->Eval(distCenterSource)*X0rad;
	}
}

//It shows the available constructors
void JDAstroProfile::GetListOfConstructors()
{
	cout << " " << endl;
	cout << "    List of available constructors is:" << endl;
	cout << "    	- 	... " << endl;
	cout << " " << endl;
}

//It shows a warning message if anything is wrong
void JDAstroProfile::GetWarning()
{
	cout << "  *****************************" << endl;
	cout << "  ***" << endl;
	cout << "  ***  WARNING:" << endl;
	cout << "  ***" << endl;
	cout << "  ***  	- 	dN/dOmega not defined..." << endl;
	cout << " " << endl;
	cout << "  *****************************" << endl;
}

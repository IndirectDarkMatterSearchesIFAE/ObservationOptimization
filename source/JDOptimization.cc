/*
 * DarkMatter.cc
 *
 *  Created on: 03/07/2017
 *      Author: david
 */

#include "JDOptimization.h"
#include "JDDarkMatter.h"

#include <TGraph.h>
#include <TMath.h>

using namespace std;

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
//	The inputs are:
// 	author			= (TString) name of author
//	source 			= (TString) name of dark matter halo
// 	candidate 		= (TString) type of signal
//  mySourcePath    = (TString) name of the path of the source
//	instrumentName	= (TString) name of the instrument
//	wobble			= (Double_t) wobble distance
//	myInstrumentPath= (TString) name of the path of the instrument
//
//	It redirects us to CreateFunctionDM()
//
JDOptimization::JDOptimization(TString author, TString source, TString candidate, TString mySourcePath, TString instrumentName, Double_t wobble, TString myInstrumentPath):
		JDDarkMatter( author, source, candidate, mySourcePath), JDInstrument( instrumentName, wobble, myInstrumentPath),
		fEvaluateQFactorVsTheta(NULL), fEvaluateJFactorFromLOS_OnVsTheta(NULL), fEvaluateJFactorFromLOS_OffVsTheta(NULL),
		fEvaluateJFactorFromLOS_TotalVsTheta(NULL), fEvaluateLOSPerSinusThetaVsDcg(NULL), fEvaluateQFactorFromLOS_TotalVsTheta(NULL),
		fEvaluateJFactorEffectiveVsTheta(NULL)
{
	    cout << endl;
		cout << endl;
		cout << "Constructor JDOptimization..." << endl;
		cout << endl;
		cout << endl;

		CreateFunctions();
}

//-----------------------------------------------
//
//	This is the destructor.
//  It deletes the functions in order not to be reused
JDOptimization::~JDOptimization()
{
	if (fEvaluateQFactorVsTheta)						delete fEvaluateQFactorVsTheta;
	if (fEvaluateJFactorFromLOS_OnVsTheta)				delete fEvaluateJFactorFromLOS_OnVsTheta;
	if (fEvaluateJFactorFromLOS_OffVsTheta)				delete fEvaluateJFactorFromLOS_OffVsTheta;
	if (fEvaluateJFactorFromLOS_TotalVsTheta)			delete fEvaluateJFactorFromLOS_TotalVsTheta;
	if (fEvaluateLOSPerSinusThetaVsDcg)					delete fEvaluateLOSPerSinusThetaVsDcg;
	if (fEvaluateJFactorEffectiveVsTheta)				delete fEvaluateJFactorEffectiveVsTheta;

		cout << endl;
		cout << endl;
		cout << "Destructor JDOptimization..." << endl;
		cout << endl;
		cout << endl;

}

//-----------------------------------------------
//
//	This function creates the important functions of this class.
//
//	fJFactorEffectiveVsTheta-> TF1 that evaluates the JFactor multiplied by the efficiency of the camera
//	fEvaluateQFactorVsTheta -> TF1 that evaluates the QFactor vs Theta; QFactor [~GeV,~cm deg] theta [deg]
//	fJFactorFromLOS_OnVsTheta
//	fJFactorFromLOS_OffVsTheta
//	fJFactorFromLOS_TotalVsTheta
//	fEvaluateLOSPerSinusThetaVsDcg
void JDOptimization::CreateFunctions()
{
	//	First case
		fEvaluateQFactorVsTheta = new TF1("fEvaluateQFactorVsTheta", this, &JDOptimization::EvaluateQFactorVsTheta, 0., GetThetaMax(), 1, "JDOptimization", "EvaluateQFactorVsTheta");

	//	Second case
		fEvaluateLOSPerSinusThetaVsDcg = new TF2("fEvaluateLOSPerSinusThetaVsDcg", this, &JDOptimization::EvaluateLOSPerSinusThetaVsDcg, 0., GetThetaMax(), -TMath::Pi(), TMath::Pi(), 1, "JDPOptimization", "EvaluateLOSVsDcg");
		fEvaluateJFactorFromLOS_OnVsTheta = new TF1("fEvaluateJFactorFromLOS_OnVsTheta", this, &JDOptimization::EvaluateJFactorFromLOS_OnVsTheta, 0., GetThetaMax(), 1, "JDOptimization", "EvaluateJFactorFromLOS_OnVsTheta");
		fEvaluateJFactorFromLOS_OffVsTheta = new TF1("fEvaluateJFactorFromLOS_OffVsTheta", this, &JDOptimization::EvaluateJFactorFromLOS_OffVsTheta, 0., GetThetaMax(), 1, "JDOptimization", "EvaluateJFactorFromLOS_OffVsTheta");
		fEvaluateJFactorFromLOS_TotalVsTheta = new TF1("fEvaluateJFactorFromLOS_TotalVsTheta", this, &JDOptimization::EvaluateJFactorFromLOS_TotalVsTheta, 0., GetThetaMax(), 1, "JDOptimization", "EvaluateJFactorFromLOS_TotalVsTheta");
		fEvaluateQFactorFromLOS_TotalVsTheta = new TF1("fEvaluateQFactorFromLOS_TotalVsTheta", this, &JDOptimization::EvaluateQFactorFromLOS_TotalVsTheta, 0., GetThetaMax(), 1, "JDOptimization", "EvaluateJFactorFromLOS_TotalVsTheta");

	//	Third case
		fEvaluateJFactorEffectiveVsTheta = new TF1("fEvaluateJFactorEffectiveVsTheta", this, &JDOptimization::EvaluateJFactorEffectiveVsTheta, 0., GetThetaMax(), 0, "JDOptimization", "EvaluateJFactorEffectiveVsTheta");
		fEvaluateQFactorEffectiveVsTheta = new TF1("fEvaluateQFactorEffectiveVsTheta", this, &JDOptimization::EvaluateQFactorEffectiveVsTheta, 0., GetThetaMax(), 1, "JDOptimization", "EvaluateQFactorEffectiveVsTheta");

}

//----------------------------------------------------
//	It evaluates the QFactor (JFactor/Theta) vs Theta normalized at a chosen point of normalization
//
// x[0] 	= dTheta
// par[0] 	= point of normalization
Double_t JDOptimization::EvaluateQFactorVsTheta(Double_t* x, Double_t* par)
{
	 return (GetTF1JFactorVsTheta()->Eval(x[0])/x[0])/(GetTF1JFactorVsTheta()->Eval(par[0])/par[0]);
}

//----------------------------------------------------
//	It evaluates the LOS multiplied by the Sinu of Theta vs dcg (distance to the centre of the galaxy)
//	The function is a TF2 function
//
// x[0] 	= dTheta	[deg]
// x[1]		= phi		[rad]
// par[0] 	= wobble distance	[deg]
Double_t JDOptimization::EvaluateLOSPerSinusThetaVsDcg(Double_t* x, Double_t* par)
{
	Double_t Dcg = TMath::Power(4*TMath::Power(par[0],2)+TMath::Power(x[0],2)+2*2*par[0]*x[0]*TMath::Cos(x[1]),0.5);
	Double_t thetaRad = x[0]*Deg2Rad;

	return GetTF1LOSVsTheta()->Eval(Dcg)*TMath::Sin(thetaRad);
}

//----------------------------------------------------
//	It evaluates the JFactor vs theta obtained integrating the line of sight taking into account the distance to the centre of the galaxy is only theta, the "signal zone"
//
// x[0] 	= dTheta	[deg]
// x[1]		= phi		[rad]
Double_t JDOptimization::EvaluateJFactorFromLOS_OnVsTheta(Double_t* x, Double_t* par)
{
	fEvaluateLOSPerSinusThetaVsDcg->SetParameter(0, 0.);
	return (fEvaluateLOSPerSinusThetaVsDcg->Integral(0., x[0], -TMath::Pi(), TMath::Pi(),1e-4));
}

//----------------------------------------------------
//	It evaluates the JFactor vs theta obtained integrating the line of sight taking into account the distance to the centre of the galaxy is not only theta, but also a wobble distance added, the "background zone"
//
// x[0] 	= dTheta	[deg]
// x[1]		= phi		[rad]
Double_t JDOptimization::EvaluateJFactorFromLOS_OffVsTheta(Double_t* x, Double_t* par)
{
	fEvaluateLOSPerSinusThetaVsDcg->SetParameter(0, GetWobble());
	return (fEvaluateLOSPerSinusThetaVsDcg->Integral(0., x[0], -TMath::Pi(), TMath::Pi(),5e-3));
}

//----------------------------------------------------
//	It evaluates the JFactor vs theta obtained by substracting to the JFactor of the "signal zone" the JFactor of the "background zone". I call it the total JFactor.
//
// x[0] 	= dTheta	[deg]
Double_t JDOptimization::EvaluateJFactorFromLOS_TotalVsTheta(Double_t* x, Double_t* par)
{
	return fEvaluateJFactorFromLOS_OnVsTheta->Eval(x[0])-fEvaluateJFactorFromLOS_OffVsTheta->Eval(x[0]);
}

//----------------------------------------------------
//	It evaluates the QFactor from the total LOS (JFactorFromLOSTotal/Theta) vs Theta normalized with a certain value
//
// x[0] 	= dTheta
// par[0] 	= point of normalization
Double_t JDOptimization::EvaluateQFactorFromLOS_TotalVsTheta(Double_t* x, Double_t* par)
{
	return (fEvaluateJFactorFromLOS_TotalVsTheta->Eval(x[0])/x[0])/(GetTF1JFactorVsTheta()->Eval(par[0])/par[0]);
}

//----------------------------------------------------
// It evaluates the JFactor multiplied by the efficiency of the camera (It can be easily changed to the JFactorFromLOSTotalEffective)
// x[0] = dTheta	[deg]
Double_t JDOptimization::EvaluateJFactorEffectiveVsTheta(Double_t* x, Double_t* par)
{
	return (fEvaluateJFactorFromLOS_TotalVsTheta->Eval(x[0]))*(GetTF1EfficiencyVsTheta()->Eval(x[0]));
}

//----------------------------------------------------
//	It evaluates the QFactor Effective (JFactorEffective/Theta) vs Theta normalized with a certain value
//
// x[0] 	= dTheta
// par[0] 	= point of normalization
Double_t JDOptimization::EvaluateQFactorEffectiveVsTheta(Double_t* x, Double_t* par)
{
	return (fEvaluateJFactorEffectiveVsTheta->Eval(x[0])/x[0])/(GetTF1JFactorVsTheta()->Eval(par[0])/par[0]);
}

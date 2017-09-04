/*
 * JDOptimization.cc
 *
 *  Created on: 03/07/2017
 *  Authors: David Navarro Gironés 	<<david.navarrogir@e-campus.uab.cat>>
 *  		 Joaquim Palacio 		<<jpalacio@ifae.es>>
 *
 *  		 ADD A GENERAL DESCRIPTION ON THE CLASS, THE MAIN FUNCTIONS, THE VARIABLES
 *  		 AND MENTION THE "runExample#.C" THAT SHOWS HOW TO USE IT
 */

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
#include <iostream>
#include <TStyle.h>

#include "JDOptimization.h"
#include "JDDarkMatter.h"
#include "JDInstrument.h"

//const static double Deg2Rad = TMath::Pi()/180.;

using namespace std;

////-----------------------------------------------
//// new (QUIM)

JDOptimization::JDOptimization(TString txtFile, TString instrumentName, Double_t distCameraCenter, Double_t wobble):
fEvaluateQ0FactorVsTheta(NULL), fEvaluateJFactorFromLOS_OnVsTheta(NULL), fEvaluateJFactorFromLOS_OffVsTheta(NULL),
fEvaluateJFactorFromLOS_TotalVsTheta(NULL), fEvaluateLOSPerSinusThetaVsDcg(NULL), fEvaluateQFactorFromLOS_TotalVsTheta(NULL),
fEvaluateJFactorEffectiveVsTheta(NULL),
dDeg2Rad(TMath::Pi()/180.)
{
	    cout << endl;
		cout << endl;
		cout << "   Constructor JDOptimization..." << endl;
		cout << endl;
		cout << endl;

		jdDarkMatter= new JDDarkMatter(txtFile);
		jdInstrument= new JDInstrument(distCameraCenter, wobble);

		CreateFunctions();
}

////-----------------------------------------------
//// new (QUIM)
////
////	This is the constructor.
////
////	The inputs are:
//// 	author			= (TString) name of author
////	source 			= (TString) name of dark matter halo
//// 	candidate 		= (TString) type of signal
////  mySourcePath    = (TString) name of the path of the source
////	instrumentName	= (TString) name of the instrument
////	wobble			= (Double_t) wobble distance
////	myInstrumentPath= (TString) name of the path of the instrument
JDOptimization::JDOptimization(TString author, TString source, TString candidate, TString mySourcePath, TString instrumentName, Double_t distCameraCenter, Double_t wobble):
fEvaluateQ0FactorVsThetaWobble(NULL),fEvaluateQ1FactorVsThetaWobble(NULL),fEvaluateQ2FactorVsThetaWobble(NULL),
fEvaluateQ0FactorVsTheta(NULL), fEvaluateQ1FactorVsTheta(NULL),fEvaluateQ2FactorVsTheta(NULL),
fEvaluateJFactorFromLOS_OnVsTheta(NULL), fEvaluateJFactorFromLOS_OffVsTheta(NULL),
fEvaluateJFactorFromLOS_TotalVsTheta(NULL), fEvaluateLOSPerSinusThetaVsDcg(NULL), fEvaluateQFactorFromLOS_TotalVsTheta(NULL),
fEvaluateJFactorEffectiveVsTheta(NULL),
dDeg2Rad(TMath::Pi()/180.)
{
	    cout << endl;
		cout << endl;
		cout << "   Constructor JDOptimization..." << endl;
		cout << endl;
		cout << endl;

		jdDarkMatter= new JDDarkMatter(author, source, candidate, mySourcePath);
		jdInstrument= new JDInstrument(distCameraCenter, wobble);

		CreateFunctions();
}

//-----------------------------------------------
//
//	This is the destructor.
//  It deletes the functions in order not to be reused
JDOptimization::~JDOptimization()
{
	if (fEvaluateQ0FactorVsTheta)						delete fEvaluateQ0FactorVsTheta;
	if (fEvaluateQ1FactorVsTheta)						delete fEvaluateQ1FactorVsTheta;
	if (fEvaluateQ2FactorVsTheta)						delete fEvaluateQ2FactorVsTheta;

	if (fEvaluateQ0FactorVsThetaWobble)					delete fEvaluateQ0FactorVsThetaWobble;
	if (fEvaluateQ1FactorVsThetaWobble)					delete fEvaluateQ1FactorVsThetaWobble;
	if (fEvaluateQ2FactorVsThetaWobble)					delete fEvaluateQ2FactorVsThetaWobble;

	if (fEvaluateJFactorFromLOS_OnVsTheta)				delete fEvaluateJFactorFromLOS_OnVsTheta;
	if (fEvaluateJFactorFromLOS_OffVsTheta)				delete fEvaluateJFactorFromLOS_OffVsTheta;
	if (fEvaluateJFactorFromLOS_TotalVsTheta)			delete fEvaluateJFactorFromLOS_TotalVsTheta;
	if (fEvaluateLOSPerSinusThetaVsDcg)					delete fEvaluateLOSPerSinusThetaVsDcg;
	if (fEvaluateJFactorEffectiveVsTheta)				delete fEvaluateJFactorEffectiveVsTheta;

	cout << endl;
	cout << endl;
	cout << "   Destructor JDOptimization..." << endl;
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
	// (QUIM) Hauriem de definir uns criteris tipo: Q = N_gamma/Sqrt(N_bkg),
	//	[where acceptance_gamma=acceptance_bkg, tinc un plot que demostra aixó, recorda-m'ho]
	//	IDEAL: 					Q1 = J/theta
	//	LEAKAGE EFFECT: 		Q2 = int_LOS_On/Sqrt(theta*theta + int_LOS_Off)
	//  ACCEPTANCE EFFECT: 		Q3 = J_eff/theta_eff
	// 	LEAKAGE + ACCEPTANCE:	Q4 = int_LOS_On_eff/Sqrt(theta_eff*theta_eff + int_LOS_Off_eff)

	//	J_on/theta
		fEvaluateQ0FactorVsTheta = new TF1("fEvaluateQ0FactorVsTheta", this, &JDOptimization::EvaluateQ0FactorVsTheta, 0., GetThetaMax(), 1, "JDOptimization", "EvaluateQ0FactorVsTheta");
	//	J_on/Sqrt{theta^2+J_off}
		fEvaluateQ1FactorVsTheta = new TF1("fEvaluateQ1FactorVsTheta", this, &JDOptimization::EvaluateQ1FactorVsTheta, 0., GetThetaMax(), 1, "JDOptimization", "EvaluateQ1FactorVsTheta");
		//	J_eff/theta_eff
		fEvaluateQ2FactorVsTheta = new TF1("fEvaluateQ2FactorVsTheta", this, &JDOptimization::EvaluateQ2FactorVsTheta, 0., GetThetaMax(), 1, "JDOptimization", "EvaluateQ2FactorVsTheta");


	//	Second case
		fEvaluateLOSPerSinusThetaVsDcg = new TF2("fEvaluateLOSPerSinusThetaVsDcg", this, &JDOptimization::EvaluateLOSPerSinusThetaVsDcg, 0., GetThetaMax(), -TMath::Pi(), TMath::Pi(), 1, "JDOptimization", "EvaluateLOSPerSinusThetaVsDcg");
		fEvaluateJFactorFromLOS_OnVsTheta = new TF1("fEvaluateJFactorFromLOS_OnVsTheta", this, &JDOptimization::EvaluateJFactorFromLOS_OnVsTheta, 0., GetThetaMax(), 1, "JDOptimization", "EvaluateJFactorFromLOS_OnVsTheta");
		fEvaluateJFactorFromLOS_OffVsTheta = new TF1("fEvaluateJFactorFromLOS_OffVsTheta", this, &JDOptimization::EvaluateJFactorFromLOS_OffVsTheta, 0., GetThetaMax(), 1, "JDOptimization", "EvaluateJFactorFromLOS_OffVsTheta");
		fEvaluateJFactorFromLOS_TotalVsTheta = new TF1("fEvaluateJFactorFromLOS_TotalVsTheta", this, &JDOptimization::EvaluateJFactorFromLOS_TotalVsTheta, 0., GetThetaMax(), 1, "JDOptimization", "EvaluateJFactorFromLOS_TotalVsTheta");
		fEvaluateQFactorFromLOS_TotalVsTheta = new TF1("fEvaluateQFactorFromLOS_TotalVsTheta", this, &JDOptimization::EvaluateQFactorFromLOS_TotalVsTheta, 0., GetThetaMax(), 1, "JDOptimization", "EvaluateJFactorFromLOS_TotalVsTheta");

	//	Third case
		fEvaluateJFactorEffectiveVsTheta = new TF1("fEvaluateJFactorEffectiveVsTheta", this, &JDOptimization::EvaluateJFactorEffectiveVsTheta, 0., GetThetaMax(), 0, "JDOptimization", "EvaluateJFactorEffectiveVsTheta");
		fEvaluateQFactorEffectiveVsTheta = new TF1("fEvaluateQFactorEffectiveVsTheta", this, &JDOptimization::EvaluateQFactorEffectiveVsTheta, 0., GetThetaMax(), 1, "JDOptimization", "EvaluateQFactorEffectiveVsTheta");

		//	J_on/theta
		fEvaluateQ0FactorVsThetaWobble = new TF2("fEvaluateQ0FactorVsThetaWobble", this, &JDOptimization::EvaluateQ0FactorVsThetaWobble, 0., GetThetaMax(), 0., GetDistCameraCenterMax(),2, "JDOptimization", "EvaluateQ0FactorVsThetaWobble");
		//	J_on/Sqrt{theta^2+J_off}
		fEvaluateQ1FactorVsThetaWobble = new TF2("fEvaluateQ1FactorVsThetaWobble", this, &JDOptimization::EvaluateQ1FactorVsThetaWobble, 0., GetThetaMax(), 0., GetDistCameraCenterMax(),2, "JDOptimization", "EvaluateQ1FactorVsThetaWobble");
		//	J_eff/theta_eff
		fEvaluateQ2FactorVsThetaWobble = new TF2("fEvaluateQ2FactorVsThetaWobble", this, &JDOptimization::EvaluateQ2FactorVsThetaWobble, 0., GetThetaMax(), 0., GetDistCameraCenterMax(),2, "JDOptimization", "EvaluateQ2FactorVsThetaWobble");


}

//----------------------------------------------------
//	It evaluates the QFactor normalized at a chosen point of normalization
//	Q0 = (JFactor/Theta) vs Theta
// x[0] 	= theta	[deg]
// par[0] 	= theta of normalization	[deg]
Double_t JDOptimization::EvaluateQ0FactorVsTheta(Double_t* x, Double_t* par)
{
	if(par[0]<0.)
	{
		return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])/x[0]);
	}
	else
	{
		return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])/x[0])
			 /(jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0])/par[0]);
	}
}

//----------------------------------------------------
//	It evaluates the QFactor normalized at a chosen point of normalization
//	Q0 = (JFactor/Theta) vs Theta
// x[0] 	= theta							[deg]
// x[0] 	= wobbleDist					[deg]
// p[0] 	= theta of normalization		[deg]
// p[1] 	= wobbleDist of normalization	[deg]
Double_t JDOptimization::EvaluateQ0FactorVsThetaWobble(Double_t* x, Double_t* par)
{
	if(par[0]<0. || par[1]<0.)
	{
		return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])/x[0]);
	}
	else
	{
		return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])/x[0])
			 /(jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0])/par[0]);
	}
}


//----------------------------------------------------
//	It evaluates the QFactor normalized at a chosen point of normalization
//	Q0 = (JFactor_on/Sqrt{Theta^2+J_off}) vs Theta
// x[0] 	= theta	[deg]
// par[0] 	= theta of normalization	[deg]
Double_t JDOptimization::EvaluateQ1FactorVsTheta(Double_t* x, Double_t* par)
{
	if(par[0]<0.)
	{
		return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0]))/
				 TMath::Sqrt(
				 TMath::Power(x[0],2)+
				 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*jdInstrument->GetWobbleDistance())->Eval(x[0]));
	}
	else
	{
	 return ((jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0]))/
			 	 TMath::Sqrt(
				 TMath::Power(x[0],2)+
				 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*GetWobbleDistance())->Eval(x[0])))
	 	 	 /
			 ((jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0]))/
			 	 TMath::Sqrt(
				 TMath::Power(par[0],2)+
				 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*GetWobbleDistance())->Eval(par[0])));
	}
}

//----------------------------------------------------
//	It evaluates the QFactor normalized at a chosen point of normalization
//	Q0 = (JFactor_on/Sqrt{Theta^2+J_off}) vs Theta
// x[0] 	= theta							[deg]
// x[1] 	= wobble dist					[deg]
// par[0] 	= theta of normalization		[deg]
// par[1] 	= wobble dist of normalization	[deg]
Double_t JDOptimization::EvaluateQ1FactorVsThetaWobble(Double_t* x, Double_t* par)
{
	if(par[0]<0. || par[1]<0.)
	{
		return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0]))/
				 TMath::Sqrt(
				 TMath::Power(x[0],2)+
				 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*x[1])->Eval(x[0]));
	}
	else
	{
	 return ((jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0]))/
			 	 TMath::Sqrt(
				 TMath::Power(x[0],2)+
				 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*x[1])->Eval(x[0])))
	 	 	 /
			 ((jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0]))/
			 	 TMath::Sqrt(
				 TMath::Power(par[0],2)+
				 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*par[1])->Eval(par[0])));
	}
}

//----------------------------------------------------
//	new (QUIM)
Double_t JDOptimization::EvaluateQ2FactorVsTheta(Double_t* x, Double_t* par)
{
	if(par[0]<0.)
	{
		return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0]))/
				 TMath::Sqrt(
				 TMath::Power(x[0],2)+
				 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*GetWobbleDistance())->Eval(x[0]));
	}
	else
	{
	 return ((jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0]))/
			 	 TMath::Sqrt(
				 TMath::Power(x[0],2)+
				 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*GetWobbleDistance())->Eval(x[0])))
	 	 	 /
			 ((jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0]))/
			 	 TMath::Sqrt(
				 TMath::Power(par[0],2)+
				 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*GetWobbleDistance())->Eval(par[0])));
	}
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
	Double_t thetaRad = x[0]*dDeg2Rad;

	return jdDarkMatter->GetTF1LOSVsTheta()->Eval(Dcg)*TMath::Sin(thetaRad);
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
	fEvaluateLOSPerSinusThetaVsDcg->SetParameter(0, jdInstrument->GetWobbleDistance());
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
	return (fEvaluateJFactorFromLOS_TotalVsTheta->Eval(x[0])/x[0])/(jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0])/par[0]);
}

//----------------------------------------------------
// It evaluates the JFactor multiplied by the efficiency of the camera (It can be easily changed to the JFactorFromLOSTotalEffective)
// x[0] = dTheta	[deg]
Double_t JDOptimization::EvaluateJFactorEffectiveVsTheta(Double_t* x, Double_t* par)
{
	return (fEvaluateJFactorFromLOS_TotalVsTheta->Eval(x[0]))*(jdInstrument->GetTF1EfficiencyVsTheta()->Eval(x[0]));
}

//----------------------------------------------------
//	It evaluates the QFactor Effective (JFactorEffective/Theta) vs Theta normalized with a certain value
//
// x[0] 	= dTheta
// par[0] 	= point of normalization
Double_t JDOptimization::EvaluateQFactorEffectiveVsTheta(Double_t* x, Double_t* par)
{
	return (fEvaluateJFactorEffectiveVsTheta->Eval(x[0])/x[0])/(jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0])/par[0]);
}

void JDOptimization::GetListOfQFactors()
{
	cout << endl;
	cout << "   ***************************************************************" << endl;
	cout << "   ***                                                         ***" << endl;
	cout << "   ***   - List of possibilities is:                           ***" << endl;
	cout << "   ***      0 : Q0 = J/theta                                   ***" << endl;
	cout << "   ***      1 : Q1 = J_on/Sqrt{theta^2 + J_off}                ***" << endl;
	cout << "   ***      2 : Q2 = J_1sm/theta                               ***" << endl;
	cout << "   ***      # : Q3 = J_eff/theta_eff                           ***" << endl;
	cout << "   ***      # : Qj = ...                                       ***" << endl;
	cout << "   ***                                                         ***" << endl;
	cout << "   ***      12: Q12 = J_on_1sm/Sqrt{theta^2 + J_off_1sm}       ***" << endl;
	cout << "   ***                                                         ***" << endl;
	cout << "   ***************************************************************" << endl;
	cout << endl;
}

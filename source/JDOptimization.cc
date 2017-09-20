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
//// CASE OF ENTERING THE DARK MATTER DATA WITH A TXT FILE
//JDOptimization::JDOptimization(TString txtFile, TString instrumentName, Double_t distCameraCenter, Double_t wobble):
//fEvaluateQ0FactorVsTheta(NULL), fEvaluateJFactorFromLOS_OnVsTheta(NULL), fEvaluateJFactorFromLOS_OffVsTheta(NULL),
//fEvaluateJFactorFromLOS_TotalVsTheta(NULL), fEvaluateLOSPerSinusThetaVsDcg(NULL), fEvaluateQFactorFromLOS_TotalVsTheta(NULL),
//fEvaluateJFactorEffectiveVsTheta(NULL),
//dDeg2Rad(TMath::Pi()/180.)
//{
//	    cout << endl;
//		cout << endl;
//		cout << "   Constructor JDOptimization..." << endl;
//		cout << endl;
//		cout << endl;
//
//		jdDarkMatter= new JDDarkMatter(txtFile);
//		jdInstrument= new JDInstrument(distCameraCenter, wobble);
//
//		CreateFunctions();
//}

////-----------------------------------------------
//// new (QUIM)
////	THIS IS THE CASE FOR DARK MATTER FROM REFERENCES AND AN ACCEPTANCE INTRODUCED BY SOME OF THE REFERENCES
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
dDeg2Rad(TMath::Pi()/180.)
{
	    cout << endl;
		cout << endl;
		cout << "   Constructor JDOptimization..." << endl;
		cout << endl;
		cout << endl;

		if (instrumentName == "IDEAL")
		{
			jdDarkMatter= new JDDarkMatter(author, source, candidate, mySourcePath);
			jdInstrument= new JDInstrument(distCameraCenter, wobble);
		}

		else if (instrumentName == "MAGICPointLike")
		{
			jdDarkMatter= new JDDarkMatter(author, source, candidate, mySourcePath);
			jdInstrument= new JDInstrument(distCameraCenter, wobble);
		}

		else if (instrumentName == "CTA")
		{
			// FALTA L'OPCIÓ D'ENTRAR INFORMACIÓ A PARTIR DE CTA
		}

		else
		{

			cout << "   "<< endl;
			cout << "   ERROR: The instrument name chosen is not one of the possibilities "<< endl;
			cout << "   "<< endl;

			jdInstrument= new JDInstrument();
			jdInstrument->GetListOfInstruments();
		}
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
	//	J_m1/theta
		fEvaluateQ2FactorVsTheta = new TF1("fEvaluateQ2FactorVsTheta", this, &JDOptimization::EvaluateQ2FactorVsTheta, 0., GetThetaMax(), 1, "JDOptimization", "EvaluateQ2FactorVsTheta");
	//	J_eff/theta_eff
		fEvaluateQ3FactorVsTheta = new TF1("fEvaluateQ3FactorVsTheta", this, &JDOptimization::EvaluateQ3FactorVsTheta, 0., GetThetaMax(), 1, "JDOptimization", "EvaluateQ3FactorVsTheta");

	// J_on_1sm/Sqrt{theta^2 + J_off_1sm}
		fEvaluateQ12FactorVsTheta = new TF1("fEvaluateQ12FactorVsTheta", this, &JDOptimization::EvaluateQ12FactorVsTheta, 0., GetThetaMax(), 1, "JDOptimization", "EvaluateQ12FactorVsTheta");
	// J_on_eff/Sqrt{(theta_eff)^2 + J_off_eff}
		fEvaluateQ13FactorVsTheta = new TF1("fEvaluateQ13FactorVsTheta", this, &JDOptimization::EvaluateQ13FactorVsTheta, 0., GetThetaMax(), 1, "JDOptimization", "EvaluateQ13FactorVsTheta");
	// J_1sm_eff/theta_eff
		fEvaluateQ23FactorVsTheta = new TF1("fEvaluateQ23FactorVsTheta", this, &JDOptimization::EvaluateQ23FactorVsTheta, 0., GetThetaMax(), 1, "JDOptimization", "EvaluateQ23FactorVsTheta");
	// J_on_1sm_eff/Sqrt{(theta_eff)^2 + J_off_1sm_eff}
		fEvaluateQ123FactorVsTheta = new TF1("fEvaluateQ123FactorVsTheta", this, &JDOptimization::EvaluateQ123FactorVsTheta, 0., GetThetaMax(), 1, "JDOptimization", "EvaluateQ123FactorVsTheta");

		// ENS DÓNA UN TF2, A DIFERÈNCIA DEL D'ABANS TENIM LA DEPENDÈNCIA EN EL WOBBLE

		//	J_on/theta
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
//	Q1 = (JFactor_on/Sqrt{Theta^2+J_off}) vs Theta
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
//	Q2 = (JFactor_1sm/Theta) vs Theta
// x[0] 	= theta	[deg]
// par[0] 	= theta of normalization	[deg]
Double_t JDOptimization::EvaluateQ2FactorVsTheta(Double_t* x, Double_t* par)
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
//	Q3 = (JFactor_eff/Theta_eff) vs Theta
// x[0] 	= theta	[deg]
// par[0] 	= theta of normalization	[deg]
Double_t JDOptimization::EvaluateQ3FactorVsTheta(Double_t* x, Double_t* par)
{
	if(par[0]<0.)
		{
			return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])/x[0])*(TMath::Power(jdInstrument->GetTF1EfficiencyVsTheta()->Eval(x[0]),0.5));
		}
		else
		{
			return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])/x[0])*(TMath::Power(jdInstrument->GetTF1EfficiencyVsTheta()->Eval(x[0]),0.5))
				 /(jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0])/par[0])*(TMath::Power(jdInstrument->GetTF1EfficiencyVsTheta()->Eval(par[0]),0.5));
		}
}


//----------------------------------------------------
//	It evaluates the QFactor normalized at a chosen point of normalization
//	Q12 = (JFactor_on_1sm/Sqrt{Theta^2 + JFactor_off_1sm}) vs Theta
// x[0] 	= theta	[deg]
// par[0] 	= theta of normalization	[deg]
Double_t JDOptimization::EvaluateQ12FactorVsTheta(Double_t* x, Double_t* par)
{
	if(par[0]<0.)
	{
		return (jdDarkMatter->GetTF1JFactorFromLOSVsTheta()->Eval(x[0]))/
				 TMath::Sqrt(
				 TMath::Power(x[0],2)+
				 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*jdInstrument->GetWobbleDistance())->Eval(x[0]));
	}
	else
	{
	 return ((jdDarkMatter->GetTF1JFactorFromLOSVsTheta()->Eval(x[0]))/
			 	 TMath::Sqrt(
				 TMath::Power(x[0],2)+
				 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*GetWobbleDistance())->Eval(x[0])))
	 	 	 /
			 ((jdDarkMatter->GetTF1JFactorFromLOSVsTheta()->Eval(par[0]))/
			 	 TMath::Sqrt(
				 TMath::Power(par[0],2)+
				 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*GetWobbleDistance())->Eval(par[0])));
	}
}

//----------------------------------------------------
//	It evaluates the QFactor normalized at a chosen point of normalization
//	Q13 = (JFactor_on_eff/Sqrt{(Theta_eff)^2+JFactor_off}) vs Theta
// x[0] 	= theta	[deg]
// par[0] 	= theta of normalization	[deg]
Double_t JDOptimization::EvaluateQ13FactorVsTheta(Double_t* x, Double_t* par)
{
	if(par[0]<0. || par[1]<0.)
	{
		return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta()->Eval(x[0]))/
				 TMath::Sqrt(
				 TMath::Power(x[0]*jdInstrument->GetTF1EfficiencyVsTheta()->Eval(x[0]),2)+
				 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*x[1])->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta()->Eval(x[0]));
	}
	else
	{
	 return ((jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta()->Eval(x[0]))/
			 	 TMath::Sqrt(
				 TMath::Power(x[0]*jdInstrument->GetTF1EfficiencyVsTheta()->Eval(x[0]),2)+
				 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*x[1])->Eval(x[0]))*jdInstrument->GetTF1EfficiencyVsTheta()->Eval(x[0]))
	 	 	 /
			 ((jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0])*jdInstrument->GetTF1EfficiencyVsTheta()->Eval(par[0]))/
			 	 TMath::Sqrt(
				 TMath::Power(par[0]*jdInstrument->GetTF1EfficiencyVsTheta()->Eval(par[0]),2)+
				 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*par[1])->Eval(par[0]))*jdInstrument->GetTF1EfficiencyVsTheta()->Eval(par[0]));
	}
}

//----------------------------------------------------
//	It evaluates the QFactor normalized at a chosen point of normalization
//	Q23 = (JFactor_1m_eff/Theta_eff) vs Theta
// x[0] 	= theta	[deg]
// par[0] 	= theta of normalization	[deg]
Double_t JDOptimization::EvaluateQ23FactorVsTheta(Double_t* x, Double_t* par)
{
	if(par[0]<0.)
		{
			return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta()->Eval(x[0])/x[0]);
		}
		else
		{
			return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta()->Eval(x[0])/x[0])
				 /(jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0])*jdInstrument->GetTF1EfficiencyVsTheta()->Eval(par[0])/par[0]);
		}
}

//----------------------------------------------------
// It evaluates the QFactor normalized at a chosen point of normalization
// Q123 = (JFactor_on_1m_eff/Sqrt{(Theta_eff)^2+JFactor_off_1m_eff}) vs Theta
// x[0] 	= theta	[deg]
// par[0] 	= theta of normalization	[deg]
Double_t JDOptimization::EvaluateQ123FactorVsTheta(Double_t* x, Double_t* par)
{
	if(par[0]<0. || par[1]<0.)
	{
		return (jdDarkMatter->GetTF1JFactorFromLOSVsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta()->Eval(x[0]))/
				 TMath::Sqrt(
				 TMath::Power(x[0]*jdInstrument->GetTF1EfficiencyVsTheta()->Eval(x[0]),2)+
				 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*x[1])->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta()->Eval(x[0]));
	}
	else
	{
	 return ((jdDarkMatter->GetTF1JFactorFromLOSVsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta()->Eval(x[0]))/
			 	 TMath::Sqrt(
				 TMath::Power(x[0]*jdInstrument->GetTF1EfficiencyVsTheta()->Eval(x[0]),2)+
				 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*x[1])->Eval(x[0]))*jdInstrument->GetTF1EfficiencyVsTheta()->Eval(x[0]))
	 	 	 /
			 ((jdDarkMatter->GetTF1JFactorFromLOSVsTheta()->Eval(par[0])*jdInstrument->GetTF1EfficiencyVsTheta()->Eval(par[0]))/
			 	 TMath::Sqrt(
				 TMath::Power(par[0]*jdInstrument->GetTF1EfficiencyVsTheta()->Eval(par[0]),2)+
				 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*par[1])->Eval(par[0]))*jdInstrument->GetTF1EfficiencyVsTheta()->Eval(par[0]));
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
//	It evaluates the QFactor normalized at a chosen point of normalization
//	Q0 = (JFactor_on/Sqrt{Theta^2+J_off}) vs Theta
// x[0] 	= theta							[deg]
// x[1] 	= wobble dist					[deg]
// par[0] 	= theta of normalization		[deg]
// par[1] 	= wobble dist of normalization	[deg]
Double_t JDOptimization::EvaluateQ2FactorVsThetaWobble(Double_t* x, Double_t* par)
{
	//NO ES CORRESPON AMB LA FUNCIO, HE FET COPIAR PEGAR DE L'ANTERIOR PER RESSOLDRE UN PROBLEMA
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

void JDOptimization::GetListOfQFactors()
{
	cout << endl;
	cout << "   ***************************************************************" << endl;
	cout << "   ***                                                         		***" << endl;
	cout << "   ***   - List of possibilities is:                           		***" << endl;
	cout << "   ***      0 : Q0 = J/theta                                   		***" << endl;
	cout << "   ***      1 : Q1 = J_on/Sqrt{theta^2 + J_off}                		***" << endl;
	cout << "   ***      2 : Q2 = J_1sm/theta                               		***" << endl;
	cout << "   ***      3 : Q3 = J_eff/theta_eff                           		***" << endl;
	cout << "   ***                                                         		***" << endl;
	cout << "   ***      12: Q12 = J_on_1sm/Sqrt{theta^2 + J_off_1sm}       		***" << endl;
	cout << "   ***      13: Q13 = J_on_eff/Sqrt{(theta_eff)^2 + J_off_eff} 		***" << endl;
	cout << "   ***      23: Q23 = J_1sm_eff/theta_eff								***" << endl;
	cout << "   ***     123: Q123 = J_on_1sm_eff/Sqrt{(theta_eff)^2 + J_off_1sm_eff} ***" << endl;
	cout << "   ***                                                         		***" << endl;
	cout << "   ***********************************************************************" << endl;
	cout << endl;
}

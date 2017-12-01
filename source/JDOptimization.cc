/*
 * JDOptimization.cc
 *
 *  Created on: 03/07/2017
 *  Last revision: 01/12/2017
 *
 *  Authors: David Navarro Gironés 	<<david.navarrogir@e-campus.uab.cat>>
 *  		 Joaquim Palacio 		<<jpalacio@ifae.es>>
 *
 *  		 ADD A GENERAL DESCRIPTION ON THE CLASS, THE MAIN FUNCTIONS, THE VARIABLES
 *
 *
 *  		 THIS CLASS IS THE ONE RELATED WITH THE OPTIMIZATION OF THE INSTRUMENT.
 *  		 WITH THIS CLASS YOU CAN OBTAIN THE OPTIMAL THETA AND WOBBLE DISTANCE FOR EACH SOURCE AND FOR EACH INSTRUMENT.
 *			 YOU CAN EVALUATE QFACTOR VS THETA AND THE QFACTOR VS THETA AND PHI
 *  		 VARIABLES:
 *  		 	THETA 	[DEG]
 *  		 	PHI	  	[RAD]
 *  		 	OFFSET	[DEG]
 *  		 The macro "exampleJDoptimization.cxx" shows how to use this class.
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

using namespace std;

//-----------------------------------------------
//
//	This is the constructor used when the data is given by a txt file
//  It fills a constructor with the necessary data depending on the instrument used
//	It redirects us to CreateFunctionDM()
JDOptimization::	JDOptimization(TString txtFile, TString myInstrumentPath, TString instrumentName, Double_t distCameraCenter, Double_t wobble):
fEvaluateQ0FactorVsTheta(NULL), fEvaluateQ1FactorVsTheta(NULL), fEvaluateQ2FactorVsTheta(NULL), fEvaluateQ3FactorVsTheta(NULL),
fEvaluateQ12FactorVsTheta(NULL), fEvaluateQ13FactorVsTheta(NULL), fEvaluateQ23FactorVsTheta(NULL), fEvaluateQ123FactorVsTheta(NULL),
fEvaluateQ0FactorVsThetaWobble(NULL),fEvaluateQ1FactorVsThetaWobble(NULL),fEvaluateQ2FactorVsThetaWobble(NULL), fEvaluateQ3FactorVsThetaWobble(NULL),
fEvaluateQ12FactorVsThetaWobble(NULL), fEvaluateQ13FactorVsThetaWobble(NULL), fEvaluateQ23FactorVsThetaWobble(NULL),
fEvaluateQ123FactorVsThetaWobble(NULL),bIsJFactorOnLessOff(1),
dDeg2Rad(TMath::Pi()/180.)
{

	cout << endl;
	cout << endl;
	cout << "   Constructor JDOptimization..." << endl;
	cout << endl;
	cout << endl;

	if (instrumentName == "IDEAL")
	{
		jdDarkMatter= new JDDarkMatter(txtFile);
		jdInstrument= new JDInstrument(distCameraCenter, wobble, instrumentName);
	}

	else if (instrumentName == "MAGICPointLike" || instrumentName == "Sensitivity")
	{
		jdDarkMatter= new JDDarkMatter(txtFile);
		jdInstrument= new JDInstrument(instrumentName, wobble, myInstrumentPath);
	}

	else if (instrumentName == "CTANorth50To80GeV")
	{
		jdDarkMatter= new JDDarkMatter(txtFile);
		jdInstrument= new JDInstrument(instrumentName, wobble, myInstrumentPath);
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
//	This is the constructor used when the dark matter and the acceptance data is from the references.
//
//	The inputs are:
// 	author			= (TString) name of author
//	source 			= (TString) name of dark matter halo
// 	candidate 		= (TString) type of signal
//  mySourcePath    = (TString) name of the path of the source
//  myInstrumentPath    = (TString) name of the path of the instrument
//	instrumentName	= (TString) name of the instrument
//  distCameraCenter = (Double_t) distance to the center of the camera
//	wobble			= (Double_t) wobble distance
JDOptimization::JDOptimization(TString author, TString source, TString candidate, TString mySourcePath, TString myInstrumentPath, TString instrumentName, Double_t distCameraCenter, Double_t wobble):
fEvaluateQ0FactorVsTheta(NULL), fEvaluateQ1FactorVsTheta(NULL), fEvaluateQ2FactorVsTheta(NULL), fEvaluateQ3FactorVsTheta(NULL),
fEvaluateQ12FactorVsTheta(NULL), fEvaluateQ13FactorVsTheta(NULL), fEvaluateQ23FactorVsTheta(NULL), fEvaluateQ123FactorVsTheta(NULL),
fEvaluateQ0FactorVsThetaWobble(NULL),fEvaluateQ1FactorVsThetaWobble(NULL),fEvaluateQ2FactorVsThetaWobble(NULL), fEvaluateQ3FactorVsThetaWobble(NULL),
fEvaluateQ12FactorVsThetaWobble(NULL), fEvaluateQ13FactorVsThetaWobble(NULL), fEvaluateQ23FactorVsThetaWobble(NULL),
fEvaluateQ123FactorVsThetaWobble(NULL),bIsJFactorOnLessOff(1),
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
			jdInstrument= new JDInstrument(distCameraCenter, wobble, instrumentName);
		}

		else if (instrumentName == "MAGICPointLike")
		{
			jdDarkMatter= new JDDarkMatter(author, source, candidate, mySourcePath);
			jdInstrument= new JDInstrument(instrumentName, wobble, myInstrumentPath);
		}

		else if (instrumentName == "Sensitivity")
		{
			jdDarkMatter= new JDDarkMatter(author, source, candidate, mySourcePath);
			jdInstrument= new JDInstrument(instrumentName, wobble, myInstrumentPath);
		}

		else if (instrumentName == "CTANorth50To80GeV")
		{
			jdDarkMatter= new JDDarkMatter(author, source, candidate, mySourcePath);
			jdInstrument= new JDInstrument(instrumentName, wobble, myInstrumentPath);
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
	if (fEvaluateQ3FactorVsTheta)						delete fEvaluateQ3FactorVsTheta;
	if (fEvaluateQ12FactorVsTheta)						delete fEvaluateQ12FactorVsTheta;
	if (fEvaluateQ13FactorVsTheta)						delete fEvaluateQ13FactorVsTheta;
	if (fEvaluateQ23FactorVsTheta)						delete fEvaluateQ23FactorVsTheta;
	if (fEvaluateQ123FactorVsTheta)						delete fEvaluateQ123FactorVsTheta;

	if (fEvaluateQ0FactorVsThetaWobble)					delete fEvaluateQ0FactorVsThetaWobble;
	if (fEvaluateQ1FactorVsThetaWobble)					delete fEvaluateQ1FactorVsThetaWobble;
	if (fEvaluateQ2FactorVsThetaWobble)					delete fEvaluateQ2FactorVsThetaWobble;
	if (fEvaluateQ3FactorVsThetaWobble)					delete fEvaluateQ3FactorVsThetaWobble;
	if (fEvaluateQ12FactorVsThetaWobble)				delete fEvaluateQ12FactorVsThetaWobble;
	if (fEvaluateQ13FactorVsThetaWobble)				delete fEvaluateQ13FactorVsThetaWobble;
	if (fEvaluateQ23FactorVsThetaWobble)				delete fEvaluateQ23FactorVsThetaWobble;
	if (fEvaluateQ123FactorVsThetaWobble)				delete fEvaluateQ123FactorVsThetaWobble;

	cout << endl;
	cout << endl;
	cout << "   Destructor JDOptimization..." << endl;
	cout << endl;
	cout << endl;

}

//-----------------------------------------------
//
//	This function creates the important functions of this class.
//  Creates the QFactor Vs Theta and the Qfactor Vs Theta and Phi
void JDOptimization::CreateFunctions()
{
	//	IDEAL: 									Q0 = J_on/theta
	//	LEAKAGE EFFECT: 						Q1 = J_on-J_off/theta
	//  UNCERTAINTY EFFECT: 					Q2 = J_1sm/theta
	// 	ACCEPTANCE EFFECT:						Q3 = J_eff/theta_eff
	//  LEAKAGE + UNCERTAINTY:  				Q12 = J_on_1sm-J_off_1sm/theta
	//  LEAKAGE + ACCEPTANCE:   				Q13 = J_on_eff-J_off_eff/theta_eff
	//  UNCERTAINTY + ACCEPTANCE:   			Q23 = J_1sm_eff/theta_eff
	//  LEAKAGE + UNCERTAINTY + ACCEPTANCE: 	Q123 = J_on_1sm_eff-J_off_1sm_eff/theta_eff



		fEvaluateQ0FactorVsTheta = new TF1("fEvaluateQ0FactorVsTheta", this, &JDOptimization::EvaluateQ0FactorVsTheta, 0.001, GetThetaMax(), 1, "JDOptimization", "EvaluateQ0FactorVsTheta");
		fEvaluateQ1FactorVsTheta = new TF1("fEvaluateQ1FactorVsTheta", this, &JDOptimization::EvaluateQ1FactorVsTheta, 0.001, GetThetaMax(), 1, "JDOptimization", "EvaluateQ1FactorVsTheta");
		fEvaluateQ2FactorVsTheta = new TF1("fEvaluateQ2FactorVsTheta", this, &JDOptimization::EvaluateQ2FactorVsTheta, 0.001, GetThetaMax(), 1, "JDOptimization", "EvaluateQ2FactorVsTheta");
		fEvaluateQ3FactorVsTheta = new TF1("fEvaluateQ3FactorVsTheta", this, &JDOptimization::EvaluateQ3FactorVsTheta, 0.001, GetThetaMax(), 1, "JDOptimization", "EvaluateQ3FactorVsTheta");
		fEvaluateQ12FactorVsTheta = new TF1("fEvaluateQ12FactorVsTheta", this, &JDOptimization::EvaluateQ12FactorVsTheta, 0.001, GetThetaMax(), 1, "JDOptimization", "EvaluateQ12FactorVsTheta");
		fEvaluateQ13FactorVsTheta = new TF1("fEvaluateQ13FactorVsTheta", this, &JDOptimization::EvaluateQ13FactorVsTheta, 0.001, GetThetaMax(), 1, "JDOptimization", "EvaluateQ13FactorVsTheta");
		fEvaluateQ23FactorVsTheta = new TF1("fEvaluateQ23FactorVsTheta", this, &JDOptimization::EvaluateQ23FactorVsTheta, 0.001, GetThetaMax(), 1, "JDOptimization", "EvaluateQ23FactorVsTheta");
		fEvaluateQ123FactorVsTheta = new TF1("fEvaluateQ123FactorVsTheta", this, &JDOptimization::EvaluateQ123FactorVsTheta, 0.001, GetThetaMax(), 1, "JDOptimization", "EvaluateQ123FactorVsTheta");

		fEvaluateQ0FactorVsThetaWobble = new TF2("fEvaluateQ0FactorVsThetaWobble", this, &JDOptimization::EvaluateQ0FactorVsThetaWobble, 0.001, GetThetaMax(), 0., GetDistCameraCenterMax(),2, "JDOptimization", "EvaluateQ0FactorVsThetaWobble");
		fEvaluateQ1FactorVsThetaWobble = new TF2("fEvaluateQ1FactorVsThetaWobble", this, &JDOptimization::EvaluateQ1FactorVsThetaWobble, 0.001, GetThetaMax(), 0., GetDistCameraCenterMax(),2, "JDOptimization", "EvaluateQ1FactorVsThetaWobble");
		fEvaluateQ2FactorVsThetaWobble = new TF2("fEvaluateQ2FactorVsThetaWobble", this, &JDOptimization::EvaluateQ2FactorVsThetaWobble, 0.001, GetThetaMax(), 0., GetDistCameraCenterMax(),2, "JDOptimization", "EvaluateQ2FactorVsThetaWobble");
		fEvaluateQ3FactorVsThetaWobble = new TF2("fEvaluateQ3FactorVsThetaWobble", this, &JDOptimization::EvaluateQ3FactorVsThetaWobble, 0.001, GetThetaMax(), 0., GetDistCameraCenterMax(),2, "JDOptimization", "EvaluateQ3FactorVsThetaWobble");
		fEvaluateQ12FactorVsThetaWobble = new TF2("fEvaluateQ12FactorVsThetaWobble", this, &JDOptimization::EvaluateQ12FactorVsThetaWobble, 0.001, GetThetaMax(), 0., GetDistCameraCenterMax(),2, "JDOptimization", "EvaluateQ12FactorVsThetaWobble");
		fEvaluateQ13FactorVsThetaWobble = new TF2("fEvaluateQ13FactorVsThetaWobble", this, &JDOptimization::EvaluateQ13FactorVsThetaWobble, 0.001, GetThetaMax(), 0., GetDistCameraCenterMax(),2, "JDOptimization", "EvaluateQ13FactorVsThetaWobble");
		fEvaluateQ23FactorVsThetaWobble = new TF2("fEvaluateQ23FactorVsThetaWobble", this, &JDOptimization::EvaluateQ23FactorVsThetaWobble, 0.001, GetThetaMax(), 0., GetDistCameraCenterMax(),2, "JDOptimization", "EvaluateQ23FactorVsThetaWobble");
		fEvaluateQ123FactorVsThetaWobble = new TF2("fEvaluateQ123FactorVsThetaWobble", this, &JDOptimization::EvaluateQ123FactorVsThetaWobble, 0.001, GetThetaMax(), 0., GetDistCameraCenterMax(),2, "JDOptimization", "EvaluateQ123FactorVsThetaWobble");

}

//----------------------------------------------------
//	It evaluates the Q0Factor vs Theta normalized at a chosen point of normalization
//
//  Q0 = (JFactor/Theta)
//  x[0] 	= theta	[deg]
//  par[0] 	= theta of normalization [deg]
Double_t JDOptimization::EvaluateQ0FactorVsTheta(Double_t* x, Double_t* par)
{
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
}

//----------------------------------------------------
//	It evaluates the Q1Factor vs Theta normalized at a chosen point of normalization
//
//	Q1 = Q1 = J_on-J_off/theta
//  x[0] 	= theta	[deg]
//  par[0] 	= theta of normalization [deg]
Double_t JDOptimization::EvaluateQ1FactorVsTheta(Double_t* x, Double_t* par)
{
	if (GetIsJFactorOnLessOff())
	{

		if (par[0]<0.)
			{
				return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])
						-jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*GetWobbleDistance())->Eval(x[0]))/x[0];
			}
			else
			{
			 return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])
					 -jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*GetWobbleDistance())->Eval(x[0]))/x[0]
					 /
					 (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0])/par[0]);
			}
	}

	// NOT USED

	else
	{
		if (par[0]<0.)
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
}

//----------------------------------------------------
//	It evaluates the Q2Factor vs Theta normalized at a chosen point of normalization
//
//	Q2 = JFactor_1sm/Theta
//  x[0] 	= theta	[deg]
//  par[0] 	= theta of normalization [deg]
Double_t JDOptimization::EvaluateQ2FactorVsTheta(Double_t* x, Double_t* par)
{
	if(par[0]<0.)
		{
			return (jdDarkMatter->GetTF1JFactor_m1VsTheta()->Eval(x[0])/x[0]);
		}
		else
		{
			return (jdDarkMatter->GetTF1JFactor_m1VsTheta()->Eval(x[0])/x[0])
				 /(jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0])/par[0]);
		}
}

//----------------------------------------------------
//	It evaluates the Q3Factor vs Theta normalized at a chosen point of normalization
//
//	Q3 = JFactor_eff/Theta_eff
//  x[0] 	= theta	[deg]
//  par[0] 	= theta of normalization [deg]
Double_t JDOptimization::EvaluateQ3FactorVsTheta(Double_t* x, Double_t* par)
{
	if(par[0]<0.)
		{
			return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])/x[0])*(TMath::Power(jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0]),0.5));
		}
		else
		{
			return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])/x[0])*(TMath::Power(jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0]),0.5))
					 /(jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0])/par[0]);
		}
}

//----------------------------------------------------
//	It evaluates the Q12Factor vs Theta normalized at a chosen point of normalization
//
//	Q12 = J_on_1sm-J_off_1sm/theta
//  x[0] 	= theta	[deg]
//  par[0] 	= theta of normalization [deg]
Double_t JDOptimization::EvaluateQ12FactorVsTheta(Double_t* x, Double_t* par)
{
	if (GetIsJFactorOnLessOff())
	{
		if(par[0]<0.)
		{
			return (jdDarkMatter->GetTF1JFactor_m1VsTheta()->Eval(x[0])
					-jdDarkMatter->GetTF1JFactor_m1OffFromLOSVsTheta(2*jdInstrument->GetWobbleDistance())->Eval(x[0]))/x[0];
		}
		else
		{
			return (jdDarkMatter->GetTF1JFactor_m1VsTheta()->Eval(x[0])
					-jdDarkMatter->GetTF1JFactor_m1OffFromLOSVsTheta(2*jdInstrument->GetWobbleDistance())->Eval(x[0]))/x[0]
					/
					(jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0])/par[0]);
		}
	}

	// NOT USED

	else
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
}

//----------------------------------------------------
//	It evaluates the Q13Factor vs Theta normalized at a chosen point of normalization
//
//	Q13 = J_on_eff-J_off_eff/theta_eff
//  x[0] 	= theta	[deg]
//  par[0] 	= theta of normalization [deg]
Double_t JDOptimization::EvaluateQ13FactorVsTheta(Double_t* x, Double_t* par)
{
	if (GetIsJFactorOnLessOff())
	{
		if(par[0]<0. || par[1]<0.)
		{

			return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0])
					-jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*GetWobbleDistance())->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0]))
					/x[0]*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0]);
		}

		else
		{
			return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0])
					-jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*GetWobbleDistance())->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0]))
					/x[0]*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0])
					/
					(jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0])/par[0]);
		}
	}

	// NOT USED

	else
	{

		if(par[0]<0. || par[1]<0.)
		{
			return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0]))/
					 TMath::Sqrt(
					 TMath::Power(x[0]*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0]),2)+
					 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*GetWobbleDistance())->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0]));
		}
		else
		{
		 return ((jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0]))/
					 TMath::Sqrt(
					 TMath::Power(x[0]*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0]),2)+
					 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*GetWobbleDistance())->Eval(x[0]))*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0]))
				 /
				 ((jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0])*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(par[0]))/
					 TMath::Sqrt(
					 TMath::Power(par[0]*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(par[0]),2)+
					 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*GetWobbleDistance())->Eval(par[0]))*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(par[0]));
		}
	}
}

//----------------------------------------------------
//	It evaluates the Q23Factor vs Theta normalized at a chosen point of normalization
//
//	Q23 = J_1sm_eff/theta_eff
//  x[0] 	= theta	[deg]
//  par[0] 	= theta of normalization [deg]
Double_t JDOptimization::EvaluateQ23FactorVsTheta(Double_t* x, Double_t* par)
{
	if(par[0]<0.)
		{
			return (jdDarkMatter->GetTF1JFactor_m1VsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0])/x[0]);
		}
		else
		{
			return (jdDarkMatter->GetTF1JFactor_m1VsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0])/x[0])
					/
					(jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0])/par[0]);
		}
}

//----------------------------------------------------
//	It evaluates the Q123Factor vs Theta normalized at a chosen point of normalization
//
//	Q123 = J_on_1sm_eff-J_off_1sm_eff/theta_eff
//  x[0] 	= theta	[deg]
//  par[0] 	= theta of normalization [deg]
Double_t JDOptimization::EvaluateQ123FactorVsTheta(Double_t* x, Double_t* par)
{
	if (GetIsJFactorOnLessOff())
	{
		if(par[0]<0. || par[1]<0.)
			{

				return (jdDarkMatter->GetTF1JFactor_m1VsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0])
						-jdDarkMatter->GetTF1JFactor_m1OffFromLOSVsTheta(2*GetWobbleDistance())->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0]))
						/x[0]*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0]);
			}

			else
			{
				return (jdDarkMatter->GetTF1JFactor_m1VsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0])
						-jdDarkMatter->GetTF1JFactor_m1OffFromLOSVsTheta(2*GetWobbleDistance())->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0]))
						/x[0]*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0])
						/
						(jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0])/par[0]);
			}
	}

	// NOT USED

	else
	{

		if(par[0]<0. || par[1]<0.)
		{
			return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0]))/
					 TMath::Sqrt(
					 TMath::Power(x[0]*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0]),2)+
					 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*GetWobbleDistance())->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0]));
		}
		else
		{
		 return ((jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0]))/
					 TMath::Sqrt(
					 TMath::Power(x[0]*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0]),2)+
					 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*GetWobbleDistance())->Eval(x[0]))*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0]))
				 /
				 ((jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0])*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(par[0]))/
					 TMath::Sqrt(
					 TMath::Power(par[0]*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(par[0]),2)+
					 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*GetWobbleDistance())->Eval(par[0]))*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(par[0]));
		}
	}
}

//----------------------------------------------------
//	It evaluates the Q0Factor vs Theta and Phi normalized at a chosen point of normalization
//
//	Q0 = J_on/theta
//  x[0] 	= theta							[deg]
//  x[1] 	= wobble dist					[deg]
//  par[0] 	= theta of normalization		[deg]
Double_t JDOptimization::EvaluateQ0FactorVsThetaWobble(Double_t* x, Double_t* par)
{
	if(par[0]<0. || par[1]<0.)
	{
		return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])/x[0]);
	}
	else
	{
		return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])/x[0])
			   /
			   (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0])/par[0]);
	}
}

//----------------------------------------------------
//	It evaluates the Q1Factor vs Theta and Phi normalized at a chosen point of normalization
//
//	Q1 = J_on-J_off/theta
//  x[0] 	= theta							[deg]
//  x[1] 	= wobble dist					[deg]
//  par[0] 	= theta of normalization		[deg]
//  par[1] 	= wobble dist of normalization	[deg]
Double_t JDOptimization::EvaluateQ1FactorVsThetaWobble(Double_t* x, Double_t* par)
{

	if (GetIsJFactorOnLessOff())
	{
		if (par[0]<0.)
			{
				return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])
						-jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*x[1])->Eval(x[0]))/x[0];
			}
			else
			{
			 return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])
					 -jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*x[1])->Eval(x[0]))/x[0]
					 /
					   (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0])/par[0]);
			}
	}

	// NOT USED
	else
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

}

//----------------------------------------------------
//	It evaluates the Q2Factor vs Theta and Phi normalized at a chosen point of normalization
//
//	Q2 = J_1sm/theta
//  x[0] 	= theta							[deg]
//  x[1] 	= wobble dist					[deg]
//  par[0] 	= theta of normalization		[deg]
Double_t JDOptimization::EvaluateQ2FactorVsThetaWobble(Double_t* x, Double_t* par)
{
	if(par[0]<0. || par[1]<0.)
	{
		return (jdDarkMatter->GetTF1JFactor_m1VsTheta()->Eval(x[0])/x[0]);
	}
	else
	{
		return (jdDarkMatter->GetTF1JFactor_m1VsTheta()->Eval(x[0])/x[0])
			 /(jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0])/par[0]);
	}
}

//----------------------------------------------------
//	It evaluates the Q3Factor vs Theta and Phi normalized at a chosen point of normalization
//
//	Q3 = J_eff/theta_eff
//  x[0] 	= theta							[deg]
//  x[1] 	= wobble dist					[deg]
//  par[0] 	= theta of normalization		[deg]
Double_t JDOptimization::EvaluateQ3FactorVsThetaWobble(Double_t* x, Double_t* par)
{
	if(par[0]<0. || par[1]<0.)
	{
		return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])/x[0])*(TMath::Power(jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(x[0]),0.5));
	}
	else
	{
		return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])/x[0])*(TMath::Power(jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(x[0]),0.5))
				 /
			   (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0])/par[0]);
	}
}

//----------------------------------------------------
//	It evaluates the Q12Factor vs Theta and Phi normalized at a chosen point of normalization
//
//	Q12 = J_on_1sm-J_off_1sm/theta
//  x[0] 	= theta							[deg]
//  x[1] 	= wobble dist					[deg]
//  par[0] 	= theta of normalization		[deg]
//  par[1]  = wobble of normalization       [deg]
Double_t JDOptimization::EvaluateQ12FactorVsThetaWobble(Double_t* x, Double_t* par)
{
	if (GetIsJFactorOnLessOff())
	{
		if(par[0]<0.)
		{
			return (jdDarkMatter->GetTF1JFactor_m1VsTheta()->Eval(x[0])
					-jdDarkMatter->GetTF1JFactor_m1OffFromLOSVsTheta(2*x[1])->Eval(x[0]))/x[0];
		}
		else
		{
			return (jdDarkMatter->GetTF1JFactor_m1VsTheta()->Eval(x[0])
					-jdDarkMatter->GetTF1JFactor_m1OffFromLOSVsTheta(2*x[1])->Eval(x[0]))/x[0]
					/
					(jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0])/par[0]);
		}
	}

	// NOT USED
	else
	{
		if(par[0]<0.)
		{
			return (jdDarkMatter->GetTF1JFactorFromLOSVsTheta()->Eval(x[0]))/
					 TMath::Sqrt(
					 TMath::Power(x[0],2)+
					 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*x[1])->Eval(x[0]));
		}
		else
		{
		 return ((jdDarkMatter->GetTF1JFactorFromLOSVsTheta()->Eval(x[0]))/
				 	 TMath::Sqrt(
					 TMath::Power(x[0],2)+
					 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*x[1])->Eval(x[0])))
		 	 	 /
				 ((jdDarkMatter->GetTF1JFactorFromLOSVsTheta()->Eval(par[0]))/
				 	 TMath::Sqrt(
					 TMath::Power(par[0],2)+
					 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*par[1])->Eval(par[0])));
		}
	}

}

//----------------------------------------------------
//	It evaluates the Q13Factor vs Theta and Phi normalized at a chosen point of normalization
//
//	Q13 = J_on_eff-J_off_eff/theta_eff
//  x[0] 	= theta							[deg]
//  x[1] 	= wobble dist					[deg]
//  par[0] 	= theta of normalization		[deg]
//  par[1]  = wobble of normalization       [deg]
Double_t JDOptimization::EvaluateQ13FactorVsThetaWobble(Double_t* x, Double_t* par)
{
	if (GetIsJFactorOnLessOff())
	{
		if(par[0]<0. || par[1]<0.)
		{
			return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(x[0])
					-jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*x[1])->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(x[0]))
					/x[0]*jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(x[0]);
		}

		else
		{
			return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(x[0])
					-jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(x[1])->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(x[0]))
					/x[0]*jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(x[0])
					 /
					(jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0])/par[0]);
		}
	}

	// NOT USED
	else
	{
		if(par[0]<0. || par[1]<0.)
		{
			return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(x[0]))/
					 TMath::Sqrt(
					 TMath::Power(x[0]*jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(x[0]),2)+
					 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*x[1])->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(x[0]));
		}
		else
		{
		 return ((jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(x[0]))/
				 	 TMath::Sqrt(
					 TMath::Power(x[0]*jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(x[0]),2)+
					 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*x[1])->Eval(x[0]))*jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(x[0]))
		 	 	 /
				 ((jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0])*jdInstrument->GetTF1EfficiencyVsTheta(par[1])->Eval(par[0]))/
				 	 TMath::Sqrt(
					 TMath::Power(par[0]*jdInstrument->GetTF1EfficiencyVsTheta(par[1])->Eval(par[0]),2)+
					 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*par[1])->Eval(par[0]))*jdInstrument->GetTF1EfficiencyVsTheta(par[1])->Eval(par[0]));
		}
	}
}

//----------------------------------------------------
//	It evaluates the Q23Factor vs Theta and Phi normalized at a chosen point of normalization
//
//	Q23 = J_1sm_eff/theta_eff
//  x[0] 	= theta							[deg]
//  x[1] 	= wobble dist					[deg]
//  par[0] 	= theta of normalization		[deg]
Double_t JDOptimization::EvaluateQ23FactorVsThetaWobble(Double_t* x, Double_t* par)
{
	if(par[0]<0.)
		{
			return (jdDarkMatter->GetTF1JFactor_m1VsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(x[0])/x[0]);
		}
		else
		{
			return (jdDarkMatter->GetTF1JFactor_m1VsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(x[0])/x[0])
					 /(jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0])/par[0]);
		}
}

//----------------------------------------------------
//	It evaluates the Q123Factor vs Theta and Phi normalized at a chosen point of normalization
//
//	Q123 = J_on_1sm_eff-J_off_1sm_eff/theta_eff
//  x[0] 	= theta							[deg]
//  x[1] 	= wobble dist					[deg]
//  par[0] 	= theta of normalization		[deg]
//  par[1]  = wobble of normalization       [deg]
Double_t JDOptimization::EvaluateQ123FactorVsThetaWobble(Double_t* x, Double_t* par)
{
	if (GetIsJFactorOnLessOff())
	{
		if(par[0]<0. || par[1]<0.)
		{

			return (jdDarkMatter->GetTF1JFactor_m1VsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(x[0])
					-jdDarkMatter->GetTF1JFactor_m1OffFromLOSVsTheta(2*x[1])->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(x[0]))
					/x[0]*jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(x[0]);
		}

		else
		{
			return (jdDarkMatter->GetTF1JFactor_m1VsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(x[0])
					-jdDarkMatter->GetTF1JFactor_m1OffFromLOSVsTheta(2*x[1])->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(x[0]))
					/x[0]*jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(x[0])
					 /(jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0])/par[0]);
		}
	}
	// NOT USED
	else
	{
		if(par[0]<0. || par[1]<0.)
		{
			return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(x[0]))/
					 TMath::Sqrt(
					 TMath::Power(x[0]*jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(x[0]),2)+
					 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*x[1])->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(x[0]));
		}
		else
		{
		 return ((jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(x[0]))/
				 	 TMath::Sqrt(
					 TMath::Power(x[0]*jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(x[0]),2)+
					 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*x[1])->Eval(x[0]))*jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(x[0]))
		 	 	 /
				 ((jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0])*jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(par[0]))/
				 	 TMath::Sqrt(
					 TMath::Power(par[0]*jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(par[0]),2)+
					 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*par[1])->Eval(par[0]))*jdInstrument->GetTF1EfficiencyVsTheta(x[1])->Eval(par[0]));
		}
	}
}


void JDOptimization::GetListOfQFactors()
{
	cout << endl;
	cout << "   ***************************************************************" << endl;
	cout << "   ***                                                         		***" << endl;
	cout << "   ***   - List of possibilities is:                           		***" << endl;
	cout << "   ***      0 : Q0 = J_on/theta                                   		***" << endl;
	cout << "   ***      1 : Q1 = J_on-J_off/theta				              		***" << endl;
	cout << "   ***      2 : Q2 = J_1sm/theta                               		***" << endl;
	cout << "   ***      3 : Q3 = J_eff/theta_eff                           		***" << endl;
	cout << "   ***                                                         		***" << endl;
	cout << "   ***      12: Q12 = J_on_1sm-J_off_1sm/theta				       		***" << endl;
	cout << "   ***      13: Q13 = J_on_eff-J_off_eff/theta_eff						***" << endl;
	cout << "   ***      23: Q23 = J_1sm_eff/theta_eff								***" << endl;
	cout << "   ***     123: Q123 = J_on_1sm_eff-J_off_1sm_eff/theta_eff			***" << endl;
	cout << "   ***                                                         		***" << endl;
	cout << "   ***********************************************************************" << endl;
	cout << endl;
}

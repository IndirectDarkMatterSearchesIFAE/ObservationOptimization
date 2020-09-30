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

const static Double_t binResolution = 0.1; // [degrees]]

//-----------------------------------------------
//
//	Empty constructor, not needed right now...
//
//
JDOptimization::	JDOptimization()
{
    cout << endl;
	cout << endl;
	cout << "   Constructor JDOptimization..." << endl;
	cout << endl;
	cout << endl;
}


//-----------------------------------------------
////
////	This is the constructor used when the data is given by a txt file
////  It fills a constructor with the necessary data depending on the instrument used
////	It redirects us to CreateFunctionDM()
//JDOptimization::	JDOptimization(TString txtFile, TString myInstrumentPath, TString instrumentName, Double_t distCameraCenter, Double_t wobble)//:
////fQ0FactorVsTheta(NULL), fQ1FactorVsTheta(NULL), fQ2FactorVsTheta(NULL), fQ3FactorVsTheta(NULL),
////fQ12FactorVsTheta(NULL), fQ13FactorVsTheta(NULL), fQ23FactorVsTheta(NULL), fQ123FactorVsTheta(NULL),
////fQ0FactorVsThetaWobble(NULL),fQ1FactorVsThetaWobble(NULL),fQ2FactorVsThetaWobble(NULL), fQ3FactorVsThetaWobble(NULL),
////fQ12FactorVsThetaWobble(NULL), fQ13FactorVsThetaWobble(NULL), fQ23FactorVsThetaWobble(NULL),
////fQ123FactorVsThetaWobble(NULL),bIsJFactorOnLessOff(1),
////dDeg2Rad(TMath::Pi()/180.), dBinResolution(binResolution),
////bIsdNdOmegaSmeared(0), bIsdNdOmegaSigma1Smeared(0)
//{
//
//	cout << endl;
//	cout << endl;
//	cout << "   Constructor JDOptimization..." << endl;
//	cout << endl;
//	cout << endl;
//
//	if (instrumentName == "IDEAL")
//	{
//		jdDarkMatter= new JDDarkMatter(txtFile);
//		jdInstrument= new JDInstrument(distCameraCenter, wobble, instrumentName);
//	}
//
//	else if (instrumentName == "MAGICPointLike" || instrumentName == "Sensitivity")
//	{
//		jdDarkMatter= new JDDarkMatter(txtFile);
//		jdInstrument= new JDInstrument(instrumentName, wobble, myInstrumentPath);
//	}
//
//	else if (instrumentName == "CTANorth50To80GeV")
//	{
//		jdDarkMatter= new JDDarkMatter(txtFile);
//		jdInstrument= new JDInstrument(instrumentName, wobble, myInstrumentPath);
//	}
//
//	else
//	{
//
//		cout << "   "<< endl;
//		cout << "   ERROR: The instrument name chosen is not one of the possibilities "<< endl;
//		cout << "   "<< endl;
//
//		jdInstrument= new JDInstrument();
//		jdInstrument->GetListOfInstruments();
//	}
//
//		//CreateFunctions();
//}

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
fQ0FactorVsTheta(NULL), fQ1FactorVsTheta(NULL), fQ2FactorVsTheta(NULL), fQ3FactorVsTheta(NULL),
//fQ12FactorVsTheta(NULL), fQ13FactorVsTheta(NULL), fQ23FactorVsTheta(NULL), fQ123FactorVsTheta(NULL),
fQ0FactorVsThetaWobble(NULL),fQ1FactorVsThetaWobble(NULL),fQ2FactorVsThetaWobble(NULL), fQ3FactorVsThetaWobble(NULL),
fQ12FactorVsThetaWobble(NULL), fQ13FactorVsThetaWobble(NULL), fQ23FactorVsThetaWobble(NULL),
fQ123FactorVsThetaWobble(NULL),bIsJFactorOnLessOff(1),
dDeg2Rad(TMath::Pi()/180.), dBinResolution(binResolution),
bIsdNdOmegaSmeared(0), bIsdNdOmegaSigma1Smeared(0)
{
	    cout << endl;
		cout << endl;
		cout << "   Constructor JDOptimization..." << endl;
		cout << endl;
		cout << endl;


		if ((instrumentName == "IDEAL")
				and
				(author == "Bonnivard" or author == "Geringer" ))
		{
			jdDarkMatter= new JDDarkMatter(author, source, candidate, mySourcePath);
			jdInstrument= new JDInstrument(distCameraCenter, wobble, instrumentName);
		}

		else if ((instrumentName == "MAGICPointLike" or instrumentName == "Sensitivity" or instrumentName == "CTANorth50To80GeV")
				and
				(author == "Bonnivard" or author == "Geringer" ))
		{
			jdDarkMatter= new JDDarkMatter(author, source, candidate, mySourcePath);
			jdInstrument= new JDInstrument(instrumentName, wobble, myInstrumentPath);
		}
		// This is a test for CTA Paper
		else if ((instrumentName == "MyInstrument")
				and
				(author == "MyJFactor"))
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
	if (fQ0FactorVsTheta)						delete fQ0FactorVsTheta;
	if (fQ1FactorVsTheta)						delete fQ1FactorVsTheta;
	if (fQ2FactorVsTheta)						delete fQ2FactorVsTheta;
	if (fQ3FactorVsTheta)						delete fQ3FactorVsTheta;
//	if (fQ12FactorVsTheta)						delete fQ12FactorVsTheta;
//	if (fQ13FactorVsTheta)						delete fQ13FactorVsTheta;
//	if (fQ23FactorVsTheta)						delete fQ23FactorVsTheta;
//	if (fQ24FactorVsTheta)						delete fQ24FactorVsTheta;
//	if (fQ123FactorVsTheta)						delete fQ123FactorVsTheta;
//	if (fQ124FactorVsTheta)						delete fQ124FactorVsTheta;
//	if (fQ134FactorVsTheta)						delete fQ134FactorVsTheta;
	if (fQ1234FactorVsTheta)					delete fQ1234FactorVsTheta;


	if (fQ0FactorVsThetaWobble)					delete fQ0FactorVsThetaWobble;
	if (fQ1FactorVsThetaWobble)					delete fQ1FactorVsThetaWobble;
	if (fQ2FactorVsThetaWobble)					delete fQ2FactorVsThetaWobble;
	if (fQ3FactorVsThetaWobble)					delete fQ3FactorVsThetaWobble;
	if (fQ12FactorVsThetaWobble)				delete fQ12FactorVsThetaWobble;
	if (fQ13FactorVsThetaWobble)				delete fQ13FactorVsThetaWobble;
	if (fQ23FactorVsThetaWobble)				delete fQ23FactorVsThetaWobble;
	if (fQ24FactorVsThetaWobble)				delete fQ24FactorVsThetaWobble;
	if (fQ123FactorVsThetaWobble)				delete fQ123FactorVsThetaWobble;
	if (fQ124FactorVsThetaWobble)				delete fQ124FactorVsThetaWobble;
	if (fQ134FactorVsThetaWobble)				delete fQ134FactorVsThetaWobble;
	if (fQ1234FactorVsThetaWobble)				delete fQ1234FactorVsThetaWobble;

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

	//	Angular Resoloution:					Q4 = J_on*PSF/theta


		fQ0FactorVsTheta = new TF1("fQ0FactorVsTheta", this, &JDOptimization::Q0FactorVsTheta, 1e-3, GetThetaMax(), 1, "JDOptimization", "Q0FactorVsTheta");
		fQ1FactorVsTheta = new TF1("fQ1FactorVsTheta", this, &JDOptimization::Q1FactorVsTheta, 1e-3, GetThetaMax(), 1, "JDOptimization", "Q1FactorVsTheta");
		fQ2FactorVsTheta = new TF1("fQ2FactorVsTheta", this, &JDOptimization::Q2FactorVsTheta, 1e-3, GetThetaMax(), 1, "JDOptimization", "Q2FactorVsTheta");
		fQ3FactorVsTheta = new TF1("fQ3FactorVsTheta", this, &JDOptimization::Q3FactorVsTheta, 1e-3, GetThetaMax(), 1, "JDOptimization", "Q3FactorVsTheta");
//		fQ12FactorVsTheta = new TF1("fQ12FactorVsTheta", this, &JDOptimization::Q12FactorVsTheta, 1e-3, GetThetaMax(), 1, "JDOptimization", "Q12FactorVsTheta");
//		fQ13FactorVsTheta = new TF1("fQ13FactorVsTheta", this, &JDOptimization::Q13FactorVsTheta, 1e-3, GetThetaMax(), 1, "JDOptimization", "Q13FactorVsTheta");
//		fQ23FactorVsTheta = new TF1("fQ23FactorVsTheta", this, &JDOptimization::Q23FactorVsTheta, 1e-3, GetThetaMax(), 1, "JDOptimization", "Q23FactorVsTheta");
//		fQ123FactorVsTheta = new TF1("fQ123FactorVsTheta", this, &JDOptimization::Q123FactorVsTheta, 1e-3, GetThetaMax(), 1, "JDOptimization", "Q123FactorVsTheta");
//
		fQ0FactorVsThetaWobble = new TF2("fQ0FactorVsThetaWobble", this, &JDOptimization::Q0FactorVsThetaWobble, 1e-3, GetThetaMax(), 0., GetDistCameraCenterMax(),2, "JDOptimization", "Q0FactorVsThetaWobble");
		fQ1FactorVsThetaWobble = new TF2("fQ1FactorVsThetaWobble", this, &JDOptimization::Q1FactorVsThetaWobble, 1e-3, GetThetaMax(), 0., GetDistCameraCenterMax(),2, "JDOptimization", "Q1FactorVsThetaWobble");
		fQ2FactorVsThetaWobble = new TF2("fQ2FactorVsThetaWobble", this, &JDOptimization::Q2FactorVsThetaWobble, 1e-3, GetThetaMax(), 0., GetDistCameraCenterMax(),2, "JDOptimization", "Q2FactorVsThetaWobble");
		fQ3FactorVsThetaWobble = new TF2("fQ3FactorVsThetaWobble", this, &JDOptimization::Q3FactorVsThetaWobble, 1e-3, GetThetaMax(), 0., GetDistCameraCenterMax(),2, "JDOptimization", "Q3FactorVsThetaWobble");
		fQ4FactorVsThetaWobble = new TF2("fQ4FactorVsThetaWobble", this, &JDOptimization::Q4FactorVsThetaWobble, 1e-3, GetThetaMax(), 0., GetDistCameraCenterMax(),2, "JDOptimization", "Q4FactorVsThetaWobble");
		fQ12FactorVsThetaWobble = new TF2("fQ12FactorVsThetaWobble", this, &JDOptimization::Q12FactorVsThetaWobble, 1e-3, GetThetaMax(), 0., GetDistCameraCenterMax(),2, "JDOptimization", "Q12FactorVsThetaWobble");
		fQ13FactorVsThetaWobble = new TF2("fQ13FactorVsThetaWobble", this, &JDOptimization::Q13FactorVsThetaWobble, 1e-3, GetThetaMax(), 0., GetDistCameraCenterMax(),2, "JDOptimization", "Q13FactorVsThetaWobble");
		fQ14FactorVsThetaWobble = new TF2("fQ14FactorVsThetaWobble", this, &JDOptimization::Q14FactorVsThetaWobble, 1e-3, GetThetaMax(), 0., GetDistCameraCenterMax(),2, "JDOptimization", "Q14FactorVsThetaWobble");
		fQ23FactorVsThetaWobble = new TF2("fQ23FactorVsThetaWobble", this, &JDOptimization::Q23FactorVsThetaWobble, 1e-3, GetThetaMax(), 0., GetDistCameraCenterMax(),2, "JDOptimization", "Q23FactorVsThetaWobble");
		fQ24FactorVsThetaWobble = new TF2("fQ24FactorVsThetaWobble", this, &JDOptimization::Q24FactorVsThetaWobble, 1e-3, GetThetaMax(), 0., GetDistCameraCenterMax(),2, "JDOptimization", "Q24FactorVsThetaWobble");
		fQ34FactorVsThetaWobble = new TF2("fQ34FactorVsThetaWobble", this, &JDOptimization::Q34FactorVsThetaWobble, 1e-3, GetThetaMax(), 0., GetDistCameraCenterMax(),2, "JDOptimization", "Q34FactorVsThetaWobble");
		fQ123FactorVsThetaWobble = new TF2("fQ123FactorVsThetaWobble", this, &JDOptimization::Q123FactorVsThetaWobble, 1e-3, GetThetaMax(), 0., GetDistCameraCenterMax(),2, "JDOptimization", "Q123FactorVsThetaWobble");
		fQ124FactorVsThetaWobble = new TF2("fQ124FactorVsThetaWobble", this, &JDOptimization::Q124FactorVsThetaWobble, 1e-3, GetThetaMax(), 0., GetDistCameraCenterMax(),2, "JDOptimization", "Q124FactorVsThetaWobble");
		fQ134FactorVsThetaWobble = new TF2("fQ134FactorVsThetaWobble", this, &JDOptimization::Q134FactorVsThetaWobble, 1e-3, GetThetaMax(), 0., GetDistCameraCenterMax(),2, "JDOptimization", "Q134FactorVsThetaWobble");
		fQ234FactorVsThetaWobble = new TF2("fQ234FactorVsThetaWobble", this, &JDOptimization::Q234FactorVsThetaWobble, 1e-3, GetThetaMax(), 0., GetDistCameraCenterMax(),2, "JDOptimization", "Q234FactorVsThetaWobble");
		fQ1234FactorVsThetaWobble = new TF2("fQ1234FactorVsThetaWobble", this, &JDOptimization::Q1234FactorVsThetaWobble, 1e-3, GetThetaMax(), 0., GetDistCameraCenterMax(),2, "JDOptimization", "Q1234FactorVsThetaWobble");
//
//
//		// Integrated...ThetaVsThetaPhi
		fIntegratedNdOmegaEpsilonThetaVsTheta = new TF1("fIntegratedNdOmegaEpsilonThetaVsTheta",this,&JDOptimization::IntegratedNdOmegaEpsilonThetaVsTheta,0.,GetThetaMax(),1,"JDOptimization","IntegratedNdOmegaEpsilonThetaVsTheta");
		fIntegratedNdOmegaSmearedThetaVsTheta = new TF1("fIntegratedNdOmegaSmearedThetaVsTheta",this,&JDOptimization::IntegratedNdOmegaSmearedThetaVsTheta,0.,GetThetaMax(),0,"JDOptimization","IntegratedNdOmegaSmearedThetaVsTheta");
		fIntegratedNdOmegaSmearedEpsilonThetaVsTheta = new TF1("fIntegratedNdOmegaSmearedEpsilonThetaVsTheta",this,&JDOptimization::IntegratedNdOmegaSmearedEpsilonThetaVsTheta,0.,GetThetaMax(),1,"JDOptimization","IntegratedNdOmegaSmearedEpsilonThetaVsTheta");
		fIntegratedNdOmegaSigma1SmearedEpsilonThetaVsTheta = new TF1("fIntegratedNdOmegaSigma1SmearedEpsilonThetaVsTheta",this,&JDOptimization::IntegratedNdOmegaSigma1SmearedEpsilonThetaVsTheta,0.,GetThetaMax(),1,"JDOptimization","IntegratedNdOmegaSigma1SmearedEpsilonThetaVsTheta");
		fIntegratedNdOmegaSmearedEpsilonOffThetaVsTheta = new TF1("fIntegratedNdOmegaSmearedEpsilonOffThetaVsTheta",this,&JDOptimization::IntegratedNdOmegaSmearedEpsilonOffThetaVsTheta,0.,GetThetaMax(),1,"JDOptimization","IntegratedNdOmegaSmearedEpsilonOffThetaVsTheta");
		fIntegratedNdOmegaSigma1SmearedEpsilonOffThetaVsTheta = new TF1("fIntegratedNdOmegaSigma1SmearedEpsilonOffThetaVsTheta",this,&JDOptimization::IntegratedNdOmegaSigma1SmearedEpsilonOffThetaVsTheta,0.,GetThetaMax(),1,"JDOptimization","IntegratedNdOmegaSigma1SmearedEpsilonOffThetaVsTheta");
		fIntegratedNdOmegaSmearedOffThetaVsTheta = new TF1("fIntegratedNdOmegaSmearedOffThetaVsTheta",this,&JDOptimization::IntegratedNdOmegaSmearedOffThetaVsTheta,0.,GetThetaMax(),1,"JDOptimization","IntegratedNdOmegaSmearedOffThetaVsTheta");
		fIntegratedNdOmegaSigma1EpsilonThetaVsTheta = new TF1("fIntegratedNdOmegaSigma1EpsilonThetaVsTheta",this,&JDOptimization::IntegratedNdOmegaSigma1EpsilonThetaVsTheta,0.,GetThetaMax(),1,"JDOptimization","IntegratedNdOmegaSigma1EpsilonThetaVsTheta");
		fIntegratedNdOmegaOffEpsilonThetaVsTheta = new TF1("fIntegratedNdOmegaOffEpsilonThetaVsTheta",this,&JDOptimization::IntegratedNdOmegaOffEpsilonThetaVsTheta,0.,GetThetaMax(),1,"JDOptimization","IntegratedNdOmegaOffEpsilonThetaVsTheta");
		fIntegratedNdOmegaSigma1OffEpsilonThetaVsTheta = new TF1("fIntegratedNdOmegaSigmaOff1EpsilonThetaVsTheta",this,&JDOptimization::IntegratedNdOmegaSigma1OffEpsilonThetaVsTheta,0.,GetThetaMax(),1,"JDOptimization","IntegratedNdOmegaSigma1OffEpsilonThetaVsTheta");
		fIntegratedNdOmegaSigma1SmearedThetaVsTheta = new TF1("fIntegratedNdOmegaSigma1SmearedThetaVsTheta",this,&JDOptimization::IntegratedNdOmegaSigma1SmearedThetaVsTheta,0.,GetThetaMax(),1,"JDOptimization","IntegratedNdOmegaSigma1SmearedThetaVsTheta");
		fIntegratedNdOmegaSigma1SmearedOffThetaVsTheta = new TF1("fIntegratedNdOmegaSigma1SmearedOffThetaVsTheta",this,&JDOptimization::IntegratedNdOmegaSigma1SmearedOffThetaVsTheta,0.,GetThetaMax(),1,"JDOptimization","IntegratedNdOmegaSigma1SmearedOffThetaVsTheta");
//
//		// ...ThetaVsThetaPhi
		fdNdOmegaSmearedThetaVsThetaPhi = new TF2("fdNdOmegaSmearedThetaVsThetaPhi", this, &JDOptimization::dNdOmegaSmearedThetaVsThetaPhi,1e-3,GetThetaMax(),0.,2*TMath::Pi(),0, "JDOptimization", "dNdOmegaSmearedThetaVsThetaPhi");
		fdNdOmegaSmearedOffThetaVsThetaPhi = new TF2("fdNdOmegaSmearedOffThetaVsThetaPhi", this, &JDOptimization::dNdOmegaSmearedOffThetaVsThetaPhi,1e-3,GetThetaMax(),0.,2*TMath::Pi(),1, "JDOptimization", "dNdOmegaSmearedOffThetaVsThetaPhi");
		fdNdOmegaEpsilonThetaVsThetaPhi = new TF2("fdNdOmegaEpsilonThetaVsThetaPhi", this, &JDOptimization::dNdOmegaEpsilonThetaVsThetaPhi,1e-3,GetThetaMax(),0.,2*TMath::Pi(),1, "JDOptimization", "dNdOmegaEpsilonThetaVsThetaPhi");
		fdNdOmegaSmearedEpsilonThetaVsThetaPhi = new TF2("fdNdOmegaSmearedEpsilonThetaVsThetaPhi", this, &JDOptimization::dNdOmegaSmearedEpsilonThetaVsThetaPhi,1e-3,GetThetaMax(),0.,2*TMath::Pi(),1, "JDOptimization", "dNdOmegaSmearedEpsilonThetaVsThetaPhi");
		fdNdOmegaSigma1SmearedEpsilonThetaVsThetaPhi = new TF2("fdNdOmegaSigma1SmearedEpsilonThetaVsThetaPhi", this, &JDOptimization::dNdOmegaSigma1SmearedEpsilonThetaVsThetaPhi,1e-3,GetThetaMax(),0.,2*TMath::Pi(),1, "JDOptimization", "dNdOmegaSigma1SmearedEpsilonThetaVsThetaPhi");
		fdNdOmegaSmearedEpsilonOffThetaVsThetaPhi = new TF2("fdNdOmegaSmearedEpsilonThetaVsThetaPhi", this, &JDOptimization::dNdOmegaSmearedEpsilonOffThetaVsThetaPhi,1e-3,GetThetaMax(),0.,2*TMath::Pi(),1, "JDOptimization", "dNdOmegaSmearedEpsilonOffThetaVsThetaPhi");
		fdNdOmegaSigma1SmearedEpsilonOffThetaVsThetaPhi = new TF2("fdNdOmegaSigma1SmearedEpsilonOffThetaVsThetaPhi", this, &JDOptimization::dNdOmegaSigma1SmearedEpsilonOffThetaVsThetaPhi,1e-3,GetThetaMax(),0.,2*TMath::Pi(),1, "JDOptimization", "dNdOmegaSigma1SmearedEpsilonOffThetaVsThetaPhi");
		fdNdOmegaOffEpsilonThetaVsThetaPhi = new TF2("fdNdOmegaOffEpsilonThetaVsThetaPhi", this, &JDOptimization::dNdOmegaOffEpsilonThetaVsThetaPhi, 1e-3, GetThetaMax(), 0., 2*TMath::Pi(),1, "JDOptimization", "dNdOmegaOffEpsilonThetaVsThetaPhi");
		fdNdOmegaSigma1EpsilonThetaVsThetaPhi = new TF2("fdNdOmegaSigma1EpsilonThetaVsThetaPhi", this, &JDOptimization::dNdOmegaSigma1EpsilonThetaVsThetaPhi, 1e-3, GetThetaMax(), 0., 2*TMath::Pi(),1, "JDOptimization", "dNdOmegaSigma1EpsilonThetaVsThetaPhi");
		fdNdOmegaSigma1OffEpsilonThetaVsThetaPhi = new TF2("fdNdOmegaSigma1OffEpsilonThetaVsThetaPhi", this, &JDOptimization::dNdOmegaSigma1OffEpsilonThetaVsThetaPhi, 1e-3, GetThetaMax(), 0., 2*TMath::Pi(),1, "JDOptimization", "dNdOmegaSigma1OffEpsilonThetaVsThetaPhi");
		fdNdOmegaSigma1SmearedThetaVsThetaPhi = new TF2("fdNdOmegaSigma1SmearedThetaVsThetaPhi", this, &JDOptimization::dNdOmegaSigma1SmearedThetaVsThetaPhi, 1e-3, GetThetaMax(), 0., 2*TMath::Pi(),1, "JDOptimization", "dNdOmegaSigma1SmearedThetaVsThetaPhi");
		fdNdOmegaSigma1SmearedOffThetaVsThetaPhi = new TF2("fdNdOmegaSigma1SmearedOffThetaVsThetaPhi", this, &JDOptimization::dNdOmegaSigma1SmearedOffThetaVsThetaPhi, 1e-3, GetThetaMax(), 0., 2*TMath::Pi(),1, "JDOptimization", "dNdOmegaSigma1SmearedOffThetaVsThetaPhi");
//
//		// ...VsThetaPhi
		fdNdOmegaSmearedVsTheta = new TF1("fdNdOmegaSmearedVsTheta", this, &JDOptimization::dNdOmegaSmearedVsTheta, 1e-3, GetThetaMax(),0, "JDOptimization", "dNdOmegaSmearedVsTheta");
		fdNdOmegaSigma1SmearedVsTheta = new TF1("fdNdOmegaSigma1SmearedVsTheta", this, &JDOptimization::dNdOmegaSigma1SmearedVsTheta, 1e-3, GetThetaMax(), 0, "JDOptimization", "dNdOmegaSigma1SmearedVsTheta");
//
		fdNdOmegaSmearedOffVsThetaPhi = new TF2("fdNdOmegaSmearedOffVsThetaPhi", this, &JDOptimization::dNdOmegaSmearedOffVsThetaPhi, 1e-3, GetThetaMax(), 0.,2*TMath::Pi(),1, "JDOptimization", "dNdOmegaSmearedOffVsThetaPhi");
		fdNdOmegaEpsilonVsThetaPhi = new TF2("fdNdOmegaEpsilonVsThetaPhi", this, &JDOptimization::dNdOmegaEpsilonVsThetaPhi, 1e-3, GetThetaMax(), 0.,2*TMath::Pi(),1, "JDOptimization", "dNdOmegaEpsilonVsThetaPhi");
		fdNdOmegaSmearedEpsilonVsThetaPhi = new TF2("fdNdOmegaSmearedEpsilonVsThetaPhi", this, &JDOptimization::dNdOmegaSmearedEpsilonVsThetaPhi, 1e-3, GetThetaMax(), 0.,2*TMath::Pi(),1, "JDOptimization", "dNdOmegaSmearedEpsilonVsThetaPhi");
		fdNdOmegaSigma1SmearedEpsilonVsThetaPhi = new TF2("fdNdOmegaSigma1SmearedEpsilonVsThetaPhi", this, &JDOptimization::dNdOmegaSigma1SmearedEpsilonVsThetaPhi, 1e-3, GetThetaMax(), 0.,2*TMath::Pi(),1, "JDOptimization", "dNdOmegaSigma1SmearedEpsilonVsThetaPhi");
		fdNdOmegaSmearedEpsilonOffVsThetaPhi = new TF2("fdNdOmegaSmearedEpsilonOffVsThetaPhi", this, &JDOptimization::dNdOmegaSmearedEpsilonOffVsThetaPhi, 1e-3, GetThetaMax(), 0.,2*TMath::Pi(),1, "JDOptimization", "dNdOmegaSmearedEpsilonOffVsThetaPhi");
		fdNdOmegaSigma1SmearedEpsilonOffVsThetaPhi = new TF2("fdNdOmegaSigma1SmearedEpsilonOffVsThetaPhi", this, &JDOptimization::dNdOmegaSigma1SmearedEpsilonOffVsThetaPhi, 1e-3, GetThetaMax(), 0.,2*TMath::Pi(),1, "JDOptimization", "dNdOmegaSigma1SmearedEpsilonOffVsThetaPhi");
		fdNdOmegaSigma1EpsilonVsThetaPhi = new TF2("fdNdOmegaSigma1EpsilonVsThetaPhi", this, &JDOptimization::dNdOmegaSigma1EpsilonVsThetaPhi, 1e-3, GetThetaMax(), 0., 2*TMath::Pi(),1, "JDOptimization", "dNdOmegaSigma1EpsilonVsThetaPhi");
		fdNdOmegaOffEpsilonVsThetaPhi = new TF2("fdNdOmegaOffEpsilonVsThetaPhi", this, &JDOptimization::dNdOmegaOffEpsilonVsThetaPhi, 1e-3, GetThetaMax(), 0., 2*TMath::Pi(),1, "JDOptimization", "dNdOmegaOffEpsilonVsThetaPhi");
		fdNdOmegaSigma1OffEpsilonVsThetaPhi = new TF2("fdNdOmegaSigma1OffEpsilonVsThetaPhi", this, &JDOptimization::dNdOmegaSigma1OffEpsilonVsThetaPhi, 1e-3, GetThetaMax(), 0., 2*TMath::Pi(),1, "JDOptimization", "dNdOmegaSigma1OffEpsilonVsThetaPhi");
		fdNdOmegaSigma1SmearedOffVsThetaPhi = new TF2("fdNdOmegaSigma1SmearedOffVsThetaPhi", this, &JDOptimization::dNdOmegaSigma1SmearedOffVsThetaPhi, 1e-3, GetThetaMax(), 0., 2*TMath::Pi(),1, "JDOptimization", "dNdOmegaSigma1SmearedOffVsThetaPhi");


}

//----------------------------------------------------
//	It evaluates the Q0Factor vs Theta normalized at a chosen point of normalization
//
//  Q0 = (JFactor/Theta)
//  x[0] 	= theta	[deg]
//  par[0] 	= theta of normalization [deg]
Double_t JDOptimization::Q0FactorVsTheta(Double_t* x, Double_t* par)
{
	if(par[0]<0.)
	{
		return (jdDarkMatter->GetTF1IntegratedNdOmegaThetaVsTheta()->Eval(x[0])/x[0]);
	}
	else
	{
		return (jdDarkMatter->GetTF1IntegratedNdOmegaThetaVsTheta()->Eval(x[0])/x[0])
			 /(jdDarkMatter->GetTF1IntegratedNdOmegaThetaVsTheta()->Eval(par[0])/par[0]);
	}
}

//----------------------------------------------------
//	It evaluates the Q1Factor vs Theta normalized at a chosen point of normalization
//
//	Q1 = Q1 = J_on-J_off/theta
//  x[0] 	= theta	[deg]
//  par[0] 	= theta of normalization [deg]
Double_t JDOptimization::Q1FactorVsTheta(Double_t* x, Double_t* par)
{
	if (GetIsIntegraldNdOmegaOnMinusOFF())
	{
		if (par[0]<0.)
			{
				return (jdDarkMatter->GetTF1IntegratedNdOmegaThetaVsTheta()->Eval(x[0]) -
						jdDarkMatter->GetTF1IntegratedNdOmegaOffThetaVsTheta(2*GetWobbleDistance())->Eval(x[0]))/x[0];
			}
			else
			{
				return ((jdDarkMatter->GetTF1IntegratedNdOmegaThetaVsTheta()->Eval(x[0]) -
						jdDarkMatter->GetTF1IntegratedNdOmegaOffThetaVsTheta(2*GetWobbleDistance())->Eval(x[0]))/x[0])
						/
						((jdDarkMatter->GetTF1IntegratedNdOmegaThetaVsTheta()->Eval(par[0]) -
						jdDarkMatter->GetTF1IntegratedNdOmegaOffThetaVsTheta(2*GetWobbleDistance())->Eval(par[0]))/par[0]);
			}
	}
	// NOT CORRECT IF YOU DONT DEFINE A & B
	// CHECK: What is A & B?
	else
	{
		if (par[0]<0.)
		{
			return (jdDarkMatter->GetTF1IntegratedNdOmegaThetaVsTheta()->Eval(x[0]))/
					 TMath::Sqrt(
					 TMath::Power(x[0],2)+
					 jdDarkMatter->GetTF1IntegratedNdOmegaOffThetaVsTheta(2*GetWobbleDistance())->Eval(x[0]));
		}
		else
		{
		 return ((jdDarkMatter->GetTF1IntegratedNdOmegaThetaVsTheta()->Eval(x[0]))/
					 TMath::Sqrt(
					 TMath::Power(x[0],2)+
					 jdDarkMatter->GetTF1IntegratedNdOmegaOffThetaVsTheta(2*GetWobbleDistance())->Eval(x[0])))
				 /
				 ((jdDarkMatter->GetTF1IntegratedNdOmegaThetaVsTheta()->Eval(par[0]))/
					 TMath::Sqrt(
					 TMath::Power(par[0],2)+
					 jdDarkMatter->GetTF1IntegratedNdOmegaOffThetaVsTheta(2*GetWobbleDistance())->Eval(par[0])));
		}

	}
}

//----------------------------------------------------
//	It evaluates the Q2Factor vs Theta normalized at a chosen point of normalization
//
//	Q2 = JFactor_1sm/Theta
//  x[0] 	= theta	[deg]
//  par[0] 	= theta of normalization [deg]
Double_t JDOptimization::Q2FactorVsTheta(Double_t* x, Double_t* par)
{
	if(par[0]<0.)
	{
		return (jdDarkMatter->GetTF1IntegratedNdOmegaSigma1ThetaVsTheta()->Eval(x[0])/x[0]);
	}
	else
	{
		return (jdDarkMatter->GetTF1IntegratedNdOmegaSigma1ThetaVsTheta()->Eval(x[0])/x[0])
			 /(jdDarkMatter->GetTF1IntegratedNdOmegaSigma1ThetaVsTheta()->Eval(par[0])/par[0]);
	}
}

//----------------------------------------------------
//	It evaluates the Q3Factor vs Theta normalized at a chosen point of normalization
//
//	Q3 = JFactor_eff/Theta_eff
//  x[0] 	= theta	[deg]
//  par[0] 	= theta of normalization [deg]
Double_t JDOptimization::Q3FactorVsTheta(Double_t* x, Double_t* par)
{
	fdNdOmegaEpsilonThetaVsThetaPhi->SetParameter(0,par[0]);

	if(par[0]<0.)
	{
		return fdNdOmegaEpsilonThetaVsThetaPhi->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2)
				/
				TMath::Sqrt(jdInstrument->GetTF2EpsilonThetaVsThetaPhi()->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2));
	}
	else
	{
		return (fdNdOmegaEpsilonThetaVsThetaPhi->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2)/
				TMath::Sqrt(jdInstrument->GetTF2EpsilonThetaVsThetaPhi()->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2)))
				/
				(fdNdOmegaEpsilonThetaVsThetaPhi->Integral(0.,par[0],0.,2*TMath::Pi(),1e-2)/
				TMath::Sqrt(jdInstrument->GetTF2EpsilonThetaVsThetaPhi()->Integral(0.,par[0],0.,2*TMath::Pi(),1e-2)));
	}
}

//----------------------------------------------------
//	It evaluates the Q12Factor vs Theta normalized at a chosen point of normalization
//
//	Q12 = J_on_1sm-J_off_1sm/theta
//  x[0] 	= theta	[deg]
//  par[0] 	= theta of normalization [deg]
//Double_t JDOptimization::Q12FactorVsTheta(Double_t* x, Double_t* par)
//{
//	if (GetIsIntegraldNdOmegaOnMinusOFF())
//	{
//		if(par[0]<0.)
//		{
//			return (jdDarkMatter->GetTF1JFactorSigma1VsTheta()->Eval(x[0])
//					-jdDarkMatter->GetTF1JFactorSigma1OffFromLOSVsTheta(2*jdInstrument->GetWobbleDistance())->Eval(x[0]))/x[0];
//		}
//		else
//		{
//			return (jdDarkMatter->GetTF1JFactorSigma1VsTheta()->Eval(x[0])
//					-jdDarkMatter->GetTF1JFactorSigma1OffFromLOSVsTheta(2*jdInstrument->GetWobbleDistance())->Eval(x[0]))/x[0]
//					/
//					((jdDarkMatter->GetTF1JFactorSigma1VsTheta()->Eval(par[0])
//					-jdDarkMatter->GetTF1JFactorSigma1OffFromLOSVsTheta(2*jdInstrument->GetWobbleDistance())->Eval(par[0]))/par[0]);
//		}
//	}
//
//	// NOT USED
//
//	else
//	{
//		if(par[0]<0.)
//		{
//			return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0]))/
//					 TMath::Sqrt(
//					 TMath::Power(x[0],2)+
//					 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*jdInstrument->GetWobbleDistance())->Eval(x[0]));
//		}
//		else
//		{
//		 return ((jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0]))/
//					 TMath::Sqrt(
//					 TMath::Power(x[0],2)+
//					 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*GetWobbleDistance())->Eval(x[0])))
//				 /
//				 ((jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0]))/
//					 TMath::Sqrt(
//					 TMath::Power(par[0],2)+
//					 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*GetWobbleDistance())->Eval(par[0])));
//		}
//	}
//}

//----------------------------------------------------
//	It evaluates the Q13Factor vs Theta normalized at a chosen point of normalization
//
//	Q13 = J_on_eff-J_off_eff/theta_eff   [or A·J_on_eff / Sqrt(theta_eff^2 + B·J_off_eff)]
//  x[0] 	= theta	[deg]
//  par[0] 	= theta of normalization [deg]
//Double_t JDOptimization::Q13FactorVsTheta(Double_t* x, Double_t* par)
//{
//	if (GetIsIntegraldNdOmegaOnMinusOFF())
//	{
//		if(par[0]<0. || par[1]<0.)
//		{
//
//			return (fdNdOmegaEpsilonThetaVsThetaPhi->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2)
//					-fdNdOmegaOffEpsilonThetaVsThetaPhi->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2))
//					/
//					jdInstrument->GetTF2EpsilonThetaVsThetaPhi()->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2);
//		}
//
//		else
//		{
//			return ((fdNdOmegaEpsilonThetaVsThetaPhi->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2)
//					-fdNdOmegaOffEpsilonThetaVsThetaPhi->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2))
//					/
//					jdInstrument->GetTF2EpsilonThetaVsThetaPhi()->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2))
//					/
//					((fdNdOmegaEpsilonThetaVsThetaPhi->Integral(0.,par[0],0.,2*TMath::Pi(),1e-2)
//					-fdNdOmegaOffEpsilonThetaVsThetaPhi->Integral(0.,par[0],0.,2*TMath::Pi(),1e-2))
//					/
//					jdInstrument->GetTF2EpsilonThetaVsThetaPhi()->Integral(0.,par[0],0.,2*TMath::Pi(),1e-2));
//		}
//	}
//	else // NOT USED: you need to define A & B
//	{
//
//		if(par[0]<0. || par[1]<0.)
//		{
//			return fdNdOmegaEpsilonThetaVsThetaPhi->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2)
//				   /
//					TMath::Sqrt(TMath::Power(jdInstrument->GetTF2EpsilonThetaVsThetaPhi()->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2),2)
//					+ TMath::Power(fdNdOmegaOffEpsilonThetaVsThetaPhi->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2),2) );
//		}
//		else // par[1] not used
//		{
//			return (fdNdOmegaEpsilonThetaVsThetaPhi->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2)
//				   /
//					TMath::Sqrt(TMath::Power(jdInstrument->GetTF2EpsilonThetaVsThetaPhi()->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2),2)
//					+ TMath::Power(fdNdOmegaOffEpsilonThetaVsThetaPhi->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2),2) ))
//					/
//					(fdNdOmegaEpsilonThetaVsThetaPhi->Integral(0.,par[0],0.,2*TMath::Pi(),1e-2)
//					 /
//					TMath::Sqrt(TMath::Power(jdInstrument->GetTF2EpsilonThetaVsThetaPhi()->Integral(0.,par[0],0.,2*TMath::Pi(),1e-2),2)
//					+ TMath::Power(fdNdOmegaOffEpsilonThetaVsThetaPhi->Integral(0.,par[0],0.,2*TMath::Pi(),1e-2),2) ));
//		}
//	}
//}

//----------------------------------------------------
//	It evaluates the Q23Factor vs Theta normalized at a chosen point of normalization
//
//	Q23 = J_1sm_eff/theta_eff
//  x[0] 	= theta	[deg]
//  par[0] 	= theta of normalization [deg]
//Double_t JDOptimization::Q23FactorVsTheta(Double_t* x, Double_t* par)
//{
//	if(par[0]<0.)
//		{
//			return (jdDarkMatter->GetTF1JFactorSigma1VsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0])/x[0]);
//		}
//		else
//		{
//			return (jdDarkMatter->GetTF1JFactorSigma1VsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0])/x[0])
//					/
//					(jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0])/par[0]);
//		}
//}

//----------------------------------------------------
//	It evaluates the Q123Factor vs Theta normalized at a chosen point of normalization
//
//	Q123 = J_on_1sm_eff-J_off_1sm_eff/theta_eff
//  x[0] 	= theta	[deg]
//  par[0] 	= theta of normalization [deg]
//Double_t JDOptimization::Q123FactorVsTheta(Double_t* x, Double_t* par)
//{
//	if (GetIsIntegraldNdOmegaOnMinusOFF())
//	{
//		if(par[0]<0. || par[1]<0.)
//			{
//
//				return (jdDarkMatter->GetTF1JFactorSigma1VsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0])
//						-jdDarkMatter->GetTF1JFactorSigma1OffFromLOSVsTheta(2*GetWobbleDistance())->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0]))
//						/x[0]*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0]);
//			}
//
//			else
//			{
//				return (jdDarkMatter->GetTF1JFactorSigma1VsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0])
//						-jdDarkMatter->GetTF1JFactorSigma1OffFromLOSVsTheta(2*GetWobbleDistance())->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0]))
//						/x[0]*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0])
//						/
//						(jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0])/par[0]);
//			}
//	}
//
//	// NOT USED
//
//	else
//	{
//
//		if(par[0]<0. || par[1]<0.)
//		{
//			return (jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0]))/
//					 TMath::Sqrt(
//					 TMath::Power(x[0]*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0]),2)+
//					 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*GetWobbleDistance())->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0]));
//		}
//		else
//		{
//		 return ((jdDarkMatter->GetTF1JFactorVsTheta()->Eval(x[0])*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0]))/
//					 TMath::Sqrt(
//					 TMath::Power(x[0]*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0]),2)+
//					 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*GetWobbleDistance())->Eval(x[0]))*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(x[0]))
//				 /
//				 ((jdDarkMatter->GetTF1JFactorVsTheta()->Eval(par[0])*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(par[0]))/
//					 TMath::Sqrt(
//					 TMath::Power(par[0]*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(par[0]),2)+
//					 jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*GetWobbleDistance())->Eval(par[0]))*jdInstrument->GetTF1EfficiencyVsTheta(GetWobbleDistance())->Eval(par[0]));
//		}
//	}
//}

//----------------------------------------------------
//	Q0Factor vs Theta and Phi. If defined, is normalized at theta_norm and phi_norm
//	Q0 = J_on/(Sqrt(4pi)·theta)
//
//  x[0] 	= theta							[deg]
//  x[1] 	= wobble dist					[deg]
//  par[0] 	= theta_norm					[deg]
//  par[1] 	= phi_norm						[deg]
Double_t JDOptimization::Q0FactorVsThetaWobble(Double_t* x, Double_t* par)
{
	Double_t sqrtEpsilonIdeal = x[0]*TMath::Sqrt(4*TMath::Pi());
	TF1* ffIntegratedNdOmegaThetaVsTheta = jdDarkMatter->GetTF1IntegratedNdOmegaThetaVsTheta();

	return (ffIntegratedNdOmegaThetaVsTheta->Eval(x[0])/sqrtEpsilonIdeal);
}

//----------------------------------------------------
//	It evaluates the Q1Factor vs Theta and Phi
//
//	Q1 = J_on-J_off/(Sqrt(4pi)·theta)    [ or A·J_on /Sqrt(theta^2 - B·J_off)
//  x[0] 	= theta							[deg]
//  x[1] 	= wobble dist					[deg]
Double_t JDOptimization::Q1FactorVsThetaWobble(Double_t* x, Double_t* par)
{
	Double_t sqrtEpsilonIdeal = x[0]*TMath::Sqrt(4*TMath::Pi());
	//jdDarkMatter->fIntegratedNdOmegaOffThetaVsTheta->SetParameter(0,2*x[1]);
	TF1 *ffIntegratedNdOmegaThetaVsTheta = jdDarkMatter->GetTF1IntegratedNdOmegaThetaVsTheta();
	TF1 *ffIntegratedNdOmegaOffThetaVsTheta = jdDarkMatter->GetTF1IntegratedNdOmegaOffThetaVsTheta(2*x[1]);

	if (GetIsIntegraldNdOmegaOnMinusOFF())
	{
		//return (jdDarkMatter->fIntegratedNdOmegaThetaVsTheta->Eval(x[0])-
		//			jdDarkMatter->fIntegratedNdOmegaOffThetaVsTheta->Eval(x[0]))/sqrtEpsilonIdeal;
		return (ffIntegratedNdOmegaThetaVsTheta->Eval(x[0])-
				ffIntegratedNdOmegaOffThetaVsTheta->Eval(x[0]))/sqrtEpsilonIdeal;
	}
	else	// NOT CORRECT IF YOU DONT DEFINE A & B
	{
//		return 	jdDarkMatter->fIntegratedNdOmegaThetaVsTheta->Eval(x[0])
//				/
//				(TMath::Sqrt( TMath::Power(sqrtEpsilonIdeal,2)+
//				jdDarkMatter->fIntegratedNdOmegaOffThetaVsTheta->Eval(x[0])));
		return 	ffIntegratedNdOmegaThetaVsTheta->Eval(x[0])
				/
				(TMath::Sqrt( TMath::Power(sqrtEpsilonIdeal,2)+
				ffIntegratedNdOmegaOffThetaVsTheta->Eval(x[0])));
	}
}

//----------------------------------------------------
//
Double_t JDOptimization::Q14FactorVsThetaWobble(Double_t* x, Double_t* par)
{
	if(!GetIsdNdOmegaSmeared()) SetdNdOmegaSmeared();

	Double_t sqrtEpsilonIdeal = x[0]*TMath::Sqrt(4*TMath::Pi());
	fIntegratedNdOmegaSmearedOffThetaVsTheta->SetParameter(0,2*x[1]);

	if (GetIsIntegraldNdOmegaOnMinusOFF())
	{
		return (fIntegratedNdOmegaSmearedThetaVsTheta->Eval(x[0])-
					fIntegratedNdOmegaSmearedOffThetaVsTheta->Eval(x[0]))/sqrtEpsilonIdeal;
	}
	else	// NOT CORRECT IF YOU DONT DEFINE A & B
	{
		return 	fIntegratedNdOmegaSmearedThetaVsTheta->Eval(x[0])
				/
				(TMath::Sqrt( TMath::Power(sqrtEpsilonIdeal,2)+
				fIntegratedNdOmegaSmearedOffThetaVsTheta->Eval(x[0])));
	}
}

//----------------------------------------------------
//	It evaluates the Q2Factor vs Theta and Phi
//	Q2 = J_Sigma1/theta
//
//  x[0] 	= theta							[deg]
//  x[1] 	= wobble dist					[deg]
Double_t JDOptimization::Q2FactorVsThetaWobble(Double_t* x, Double_t* par)
{
	Double_t sqrtEpsilonIdeal = x[0]*TMath::Sqrt(4*TMath::Pi());
	TF1* ffIntegratedNdOmegaSigma1ThetaVsTheta = jdDarkMatter->GetTF1IntegratedNdOmegaSigma1ThetaVsTheta();
	//return (jdDarkMatter->fIntegratedNdOmegaSigma1ThetaVsTheta->Eval(x[0])/sqrtEpsilonIdeal);
	return (ffIntegratedNdOmegaSigma1ThetaVsTheta->Eval(x[0])/sqrtEpsilonIdeal);

}

//----------------------------------------------------
//	It evaluates the Q3Factor vs Theta and Phi
//	Q3 = int{dN/dOmega · epsilon}/int{epsilon}
//
//  x[0] 	= theta							[deg]
//  x[1] 	= wobble dist					[deg]
Double_t JDOptimization::Q3FactorVsThetaWobble(Double_t* x, Double_t* par)
{
	fIntegratedNdOmegaEpsilonThetaVsTheta->SetParameter(0,x[1]);

	//jdInstrument->fIntegratedEpsilonThetaVsTheta->SetParameter(0,x[1]);
	TF1* ffIntegratedEpsilonThetaVsTheta = jdInstrument->GetTF1IntegratedEpsilonThetaVsTheta();
	ffIntegratedEpsilonThetaVsTheta->SetParameter(0,x[1]);
	return fIntegratedNdOmegaEpsilonThetaVsTheta->Eval(x[0])
			/
				TMath::Sqrt(ffIntegratedEpsilonThetaVsTheta->Eval(x[0]));
			//TMath::Sqrt(jdInstrument->fIntegratedEpsilonThetaVsTheta->Eval(x[0]));
}

//----------------------------------------------------
//	It evaluates the Q4Factor vs Theta and Phi
//	Q4
Double_t JDOptimization::Q4FactorVsThetaWobble(Double_t* x, Double_t* par)
{
	if(!GetIsdNdOmegaSmeared()) SetdNdOmegaSmeared();

	Double_t sqrtEpsilonIdeal = x[0]*TMath::Sqrt(4*TMath::Pi());

	return fIntegratedNdOmegaSmearedThetaVsTheta->Eval(x[0])
			/
			sqrtEpsilonIdeal;
}

//----------------------------------------------------
//	It evaluates the Q12Factor vs Theta and Phi
//	Q12 = J_on_1sm-J_off_1sm/theta
//
//  x[0] 	= theta							[deg]
//  x[1] 	= wobble dist					[deg]
Double_t JDOptimization::Q12FactorVsThetaWobble(Double_t* x, Double_t* par)
{
	Double_t sqrtEpsilonIdeal = x[0]*TMath::Sqrt(4*TMath::Pi());

	TF1* ffIntegratedNdOmegaSigma1ThetaVsTheta = jdDarkMatter->GetTF1IntegratedNdOmegaSigma1ThetaVsTheta();

	TF1* ffIntegratedNdOmegaSigma1OffThetaVsTheta = jdDarkMatter->GetTF1IntegratedNdOmegaSigma1OffThetaVsTheta();
	ffIntegratedNdOmegaSigma1OffThetaVsTheta->SetParameter(0,2*x[1]);

	if (GetIsIntegraldNdOmegaOnMinusOFF())
	{
		return (ffIntegratedNdOmegaSigma1ThetaVsTheta->Eval(x[0])-
					ffIntegratedNdOmegaSigma1OffThetaVsTheta->Eval(x[0]))/sqrtEpsilonIdeal;
	}
	else	// NOT CORRECT IF YOU DONT DEFINE A & B
	{
		return 	ffIntegratedNdOmegaSigma1ThetaVsTheta->Eval(x[0])
				/
				TMath::Sqrt( TMath::Power(sqrtEpsilonIdeal,2)+
						ffIntegratedNdOmegaSigma1OffThetaVsTheta->Eval(x[0]));
	}
}

//----------------------------------------------------
//	It evaluates the Q23Factor vs Theta and Phi normalized at a chosen point of normalization
//
//	Q23 = J_1sm_eff/theta_eff
//  x[0] 	= theta							[deg]
//  x[1] 	= wobble dist					[deg]
//  par[0] 	= theta of normalization		[deg]
Double_t JDOptimization::Q23FactorVsThetaWobble(Double_t* x, Double_t* par)
{
	fIntegratedNdOmegaSigma1EpsilonThetaVsTheta->SetParameter(0,x[1]);

	TF1* ffIntegratedEpsilonThetaVsTheta = jdInstrument->GetTF1IntegratedEpsilonThetaVsTheta();
	ffIntegratedEpsilonThetaVsTheta->SetParameter(0,x[1]);

	return fIntegratedNdOmegaSigma1EpsilonThetaVsTheta->Eval(x[0])
			/
			TMath::Sqrt(ffIntegratedEpsilonThetaVsTheta->Eval(x[0]));
}

//----------------------------------------------------
//	It evaluates the Q23Factor vs Theta and Phi normalized at a chosen point of normalization
//
//	Q24 =
Double_t JDOptimization::Q24FactorVsThetaWobble(Double_t* x, Double_t* par)
{
	if(!GetIsdNdOmegaSigma1Smeared()) SetdNdOmegaSigma1Smeared();

	Double_t sqrtEpsilonIdeal = x[0]*TMath::Sqrt(4*TMath::Pi());
	return (fIntegratedNdOmegaSigma1SmearedThetaVsTheta->Eval(x[0])/sqrtEpsilonIdeal);
}

//----------------------------------------------------
//	It evaluates the Q34Factor vs Theta and Phi normalized at a chosen point of normalization
//
//	Q34 =
Double_t JDOptimization::Q34FactorVsThetaWobble(Double_t* x, Double_t* par)
{
	if(!GetIsdNdOmegaSmeared()) SetdNdOmegaSmeared();

	fIntegratedNdOmegaSmearedEpsilonThetaVsTheta->SetParameter(0,x[1]);
	//jdInstrument->fIntegratedEpsilonThetaVsTheta->SetParameter(0,x[1]);
	TF1 *ffIntegratedEpsilonThetaVsTheta = jdInstrument->GetTF1IntegratedEpsilonThetaVsTheta();
	ffIntegratedEpsilonThetaVsTheta->SetParameter(0,x[1]);

	return fIntegratedNdOmegaSmearedEpsilonThetaVsTheta->Eval(x[0])
			/
			TMath::Sqrt(ffIntegratedEpsilonThetaVsTheta->Eval(x[0]));
			//TMath::Sqrt(jdInstrument->fIntegratedEpsilonThetaVsTheta->Eval(x[0]));
}

//----------------------------------------------------
//	It evaluates the Q13Factor vs Theta and Phi normalized at a chosen point of normalization
//
//	Q13 = J_on_eff-J_off_eff/theta_eff
//  x[0] 	= theta							[deg]
//  x[1] 	= wobble dist					[deg]
//  par[0] 	= theta of normalization		[deg]
//  par[1]  = wobble of normalization       [deg]
Double_t JDOptimization::Q13FactorVsThetaWobble(Double_t* x, Double_t* par)
{
	fIntegratedNdOmegaEpsilonThetaVsTheta->SetParameter(0,x[1]);
	fIntegratedNdOmegaOffEpsilonThetaVsTheta->SetParameter(0,x[1]);

	TF1* ffIntegratedEpsilonThetaVsTheta = jdInstrument->GetTF1IntegratedEpsilonThetaVsTheta();
	ffIntegratedEpsilonThetaVsTheta->SetParameter(0,x[1]);

	if (GetIsIntegraldNdOmegaOnMinusOFF())
	{
		return (fIntegratedNdOmegaEpsilonThetaVsTheta->Eval(x[0])-
				fIntegratedNdOmegaOffEpsilonThetaVsTheta->Eval(x[0]))
				/TMath::Sqrt(ffIntegratedEpsilonThetaVsTheta->Eval(x[0]));
	}
	else	// NOT CORRECT IF YOU DONT DEFINE A & B
	{
		return 	fIntegratedNdOmegaEpsilonThetaVsTheta->Eval(x[0])
				/
				(TMath::Sqrt( ffIntegratedEpsilonThetaVsTheta->Eval(x[0])+
				fIntegratedNdOmegaOffEpsilonThetaVsTheta->Eval(x[0])));
	}
}


//----------------------------------------------------
//	It evaluates the Q123Factor vs Theta and Phi normalized at a chosen point of normalization
//	Q123 = J_on_1sm_eff-J_off_1sm_eff/theta_eff
//
//  x[0] 	= theta							[deg]
//  x[1] 	= wobble dist					[deg]
Double_t JDOptimization::Q123FactorVsThetaWobble(Double_t* x, Double_t* par)
{
	fIntegratedNdOmegaSigma1EpsilonThetaVsTheta->SetParameter(0,x[1]);
	fIntegratedNdOmegaSigma1OffEpsilonThetaVsTheta->SetParameter(0,x[1]);

	TF1* ffIntegratedEpsilonThetaVsTheta = jdInstrument->GetTF1IntegratedEpsilonThetaVsTheta();
	ffIntegratedEpsilonThetaVsTheta->SetParameter(0,x[1]);




	if (GetIsIntegraldNdOmegaOnMinusOFF())
	{
		return (fIntegratedNdOmegaSigma1EpsilonThetaVsTheta->Eval(x[0])-
				fIntegratedNdOmegaSigma1OffEpsilonThetaVsTheta->Eval(x[0]))
				/TMath::Sqrt(ffIntegratedEpsilonThetaVsTheta->Eval(x[0]));
	}
	else	// NOT CORRECT IF YOU DONT DEFINE A & B
	{
		return 	fIntegratedNdOmegaSigma1EpsilonThetaVsTheta->Eval(x[0])
				/
				(TMath::Sqrt( ffIntegratedEpsilonThetaVsTheta->Eval(x[0])+
				fIntegratedNdOmegaSigma1OffEpsilonThetaVsTheta->Eval(x[0])));
	}
}

//----------------------------------------------------
//
Double_t JDOptimization::Q124FactorVsThetaWobble(Double_t* x, Double_t* par)
{
	if(!GetIsdNdOmegaSigma1Smeared()) SetdNdOmegaSigma1Smeared();

	fIntegratedNdOmegaSigma1SmearedThetaVsTheta->SetParameter(0,x[1]);
	fIntegratedNdOmegaSigma1SmearedOffThetaVsTheta->SetParameter(0,x[1]);

	TF1* ffIntegratedEpsilonThetaVsTheta = jdInstrument->GetTF1IntegratedEpsilonThetaVsTheta();
	ffIntegratedEpsilonThetaVsTheta->SetParameter(0,x[1]);

	if (GetIsIntegraldNdOmegaOnMinusOFF())
	{
		return (fIntegratedNdOmegaSigma1SmearedThetaVsTheta->Eval(x[0])-
				fIntegratedNdOmegaSigma1SmearedOffThetaVsTheta->Eval(x[0]))
				/TMath::Sqrt(ffIntegratedEpsilonThetaVsTheta->Eval(x[0]));
	}
	else	// NOT CORRECT IF YOU DONT DEFINE A & B
	{
		return 	fIntegratedNdOmegaSigma1SmearedThetaVsTheta->Eval(x[0])
				/
				(TMath::Sqrt( ffIntegratedEpsilonThetaVsTheta->Eval(x[0])+
				fIntegratedNdOmegaSigma1SmearedOffThetaVsTheta->Eval(x[0])));
	}
}

//----------------------------------------------------
// bla bla blab bla
//
// var[1] = aldkda [unit]
// par[0] =
//
Double_t JDOptimization::Q134FactorVsThetaWobble(Double_t* x, Double_t* par)
{
	if(!GetIsdNdOmegaSmeared()) SetdNdOmegaSmeared();

	fIntegratedNdOmegaSmearedEpsilonThetaVsTheta->SetParameter(0,x[1]);
	fIntegratedNdOmegaSmearedEpsilonOffThetaVsTheta->SetParameter(0,x[1]);
	//jdInstrument->fIntegratedEpsilonThetaVsTheta->SetParameter(0,x[1]);
	TF1 *ffIntegratedEpsilonThetaVsTheta = jdInstrument->GetTF1IntegratedEpsilonThetaVsTheta();
	ffIntegratedEpsilonThetaVsTheta->SetParameter(0,x[1]);

	if (GetIsIntegraldNdOmegaOnMinusOFF())
	{
		return (fIntegratedNdOmegaSmearedEpsilonThetaVsTheta->Eval(x[0])-
				fIntegratedNdOmegaSmearedEpsilonOffThetaVsTheta->Eval(x[0]))
				/TMath::Sqrt(ffIntegratedEpsilonThetaVsTheta->Eval(x[0]));
				///TMath::Sqrt(jdInstrument->fIntegratedEpsilonThetaVsTheta->Eval(x[0]));
	}
	else	// NOT CORRECT IF YOU DONT DEFINE A & B
	{
		return 	fIntegratedNdOmegaSmearedEpsilonThetaVsTheta->Eval(x[0])
				/
				(//TMath::Sqrt( jdInstrument->fIntegratedEpsilonThetaVsTheta->Eval(x[0])
				 TMath::Sqrt( ffIntegratedEpsilonThetaVsTheta->Eval(x[0])+
				fIntegratedNdOmegaSmearedEpsilonOffThetaVsTheta->Eval(x[0])));
	}
}

//----------------------------------------------------
//
Double_t JDOptimization::Q234FactorVsThetaWobble(Double_t* x, Double_t* par)
{
	if(!GetIsdNdOmegaSigma1Smeared()) SetdNdOmegaSigma1Smeared();

	fIntegratedNdOmegaSigma1SmearedEpsilonThetaVsTheta->SetParameter(0,x[1]);

	TF1* ffIntegratedEpsilonThetaVsTheta = jdInstrument->GetTF1IntegratedEpsilonThetaVsTheta();
	ffIntegratedEpsilonThetaVsTheta->SetParameter(0,x[1]);

	if (GetIsIntegraldNdOmegaOnMinusOFF())
	{
		return (fIntegratedNdOmegaSigma1SmearedEpsilonThetaVsTheta->Eval(x[0]))
				/TMath::Sqrt(ffIntegratedEpsilonThetaVsTheta->Eval(x[0]));
	}
	else	// NOT CORRECT IF YOU DONT DEFINE A & B
	{
		return 	fIntegratedNdOmegaSigma1SmearedEpsilonThetaVsTheta->Eval(x[0])
				/
				TMath::Sqrt( ffIntegratedEpsilonThetaVsTheta->Eval(x[0]));
	}
}

//----------------------------------------------------
//
Double_t JDOptimization::Q1234FactorVsThetaWobble(Double_t* x, Double_t* par)
{
	if(!GetIsdNdOmegaSigma1Smeared()) SetdNdOmegaSigma1Smeared();

	fIntegratedNdOmegaSigma1SmearedEpsilonThetaVsTheta->SetParameter(0,x[1]);
	fIntegratedNdOmegaSigma1SmearedEpsilonOffThetaVsTheta->SetParameter(0,x[1]);

	TF1* ffIntegratedEpsilonThetaVsTheta=jdInstrument->GetTF1IntegratedEpsilonThetaVsTheta();
	ffIntegratedEpsilonThetaVsTheta->SetParameter(0,x[1]);



	if (GetIsIntegraldNdOmegaOnMinusOFF())
	{
		return (fIntegratedNdOmegaSigma1SmearedEpsilonThetaVsTheta->Eval(x[0])-
				fIntegratedNdOmegaSigma1SmearedEpsilonOffThetaVsTheta->Eval(x[0]))
				/TMath::Sqrt(ffIntegratedEpsilonThetaVsTheta->Eval(x[0]));
	}
	else	// NOT CORRECT IF YOU DONT DEFINE A & B
	{
		return 	fIntegratedNdOmegaSigma1SmearedEpsilonThetaVsTheta->Eval(x[0])
				/
				(TMath::Sqrt( ffIntegratedEpsilonThetaVsTheta->Eval(x[0])+
				fIntegratedNdOmegaSigma1SmearedEpsilonOffThetaVsTheta->Eval(x[0])));
	}
}

//-----------------------------------------------
// It Integrateds the dNdOmega · Epsilon [# · deg^{-2}] vs Theta [deg] and Phi [rad] multiplied by Theta [deg] in order to obtain the N(Delta Omega) [#]
//
// x[0] 	= dTheta [deg]
// par[0] 	= offset distance [deg]
Double_t JDOptimization::IntegratedNdOmegaEpsilonThetaVsTheta(Double_t* x, Double_t* par)
{
	fdNdOmegaEpsilonThetaVsThetaPhi->SetParameter(0,par[0]);
	return fdNdOmegaEpsilonThetaVsThetaPhi->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2);
}

//-----------------------------------------------
//
Double_t JDOptimization::IntegratedNdOmegaSmearedEpsilonThetaVsTheta(Double_t* x, Double_t* par)
{
	fdNdOmegaSmearedEpsilonThetaVsThetaPhi->SetParameter(0,par[0]);
	return fdNdOmegaSmearedEpsilonThetaVsThetaPhi->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2);
}

//-----------------------------------------------
//
Double_t JDOptimization::IntegratedNdOmegaSigma1SmearedEpsilonThetaVsTheta(Double_t* x, Double_t* par)
{
	fdNdOmegaSigma1SmearedEpsilonThetaVsThetaPhi->SetParameter(0,par[0]);
	return fdNdOmegaSigma1SmearedEpsilonThetaVsThetaPhi->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2);
}

//-----------------------------------------------
//
Double_t JDOptimization::IntegratedNdOmegaSmearedEpsilonOffThetaVsTheta(Double_t* x, Double_t* par)
{
	fdNdOmegaSmearedEpsilonOffThetaVsThetaPhi->SetParameter(0,par[0]);
	return fdNdOmegaSmearedEpsilonOffThetaVsThetaPhi->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2);
}

//-----------------------------------------------
//
Double_t JDOptimization::IntegratedNdOmegaSigma1SmearedEpsilonOffThetaVsTheta(Double_t* x, Double_t* par)
{
	fdNdOmegaSigma1SmearedEpsilonOffThetaVsThetaPhi->SetParameter(0,par[0]);
	return fdNdOmegaSigma1SmearedEpsilonOffThetaVsThetaPhi->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2);
}

//-----------------------------------------------
//
Double_t JDOptimization::IntegratedNdOmegaSmearedThetaVsTheta(Double_t* x, Double_t* par)
{
	return fdNdOmegaSmearedThetaVsThetaPhi->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2);
}

//-----------------------------------------------
//
Double_t JDOptimization::IntegratedNdOmegaSmearedOffThetaVsTheta(Double_t* x, Double_t* par)
{
	fdNdOmegaSmearedOffThetaVsThetaPhi->SetParameter(0,par[0]);
	return fdNdOmegaSmearedOffThetaVsThetaPhi->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2);
}

//-----------------------------------------------
// It Integrateds the dNdOmega_off · Epsilon [# · deg^{-2}] vs Theta [deg] and Phi [rad] multiplied by Theta [deg] in order to obtain the N(Delta Omega) [#]
//
// x[0] 	= dTheta [deg]
// par[0] 	= offset distance [deg]
Double_t JDOptimization::IntegratedNdOmegaOffEpsilonThetaVsTheta(Double_t* x, Double_t* par)
{
	fdNdOmegaOffEpsilonThetaVsThetaPhi->SetParameter(0,par[0]);
	return fdNdOmegaOffEpsilonThetaVsThetaPhi->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2);
}

//-----------------------------------------------
// It Integrateds the dNdOmega_off_sig1 · Epsilon [# · deg^{-2}] vs Theta [deg] and Phi [rad] multiplied by Theta [deg] in order to obtain the N(Delta Omega) [#]
//
// x[0] 	= dTheta [deg]
// par[0] 	= offset distance [deg]
Double_t JDOptimization::IntegratedNdOmegaSigma1OffEpsilonThetaVsTheta(Double_t* x, Double_t* par)
{
	fdNdOmegaSigma1OffEpsilonThetaVsThetaPhi->SetParameter(0,par[0]);
	return fdNdOmegaSigma1OffEpsilonThetaVsThetaPhi->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2);
}

//-----------------------------------------------
// It Integrateds the dNdOmega_off_sig1 · Epsilon [# · deg^{-2}] vs Theta [deg] and Phi [rad] multiplied by Theta [deg] in order to obtain the N(Delta Omega) [#]
//
Double_t JDOptimization::IntegratedNdOmegaSigma1SmearedThetaVsTheta(Double_t* x, Double_t* par)
{
	fdNdOmegaSigma1SmearedThetaVsThetaPhi->SetParameter(0,par[0]);
	return fdNdOmegaSigma1SmearedThetaVsThetaPhi->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2);
}

//-----------------------------------------------
//
//
Double_t JDOptimization::IntegratedNdOmegaSigma1SmearedOffThetaVsTheta(Double_t* x, Double_t* par)
{
	fdNdOmegaSigma1SmearedOffThetaVsThetaPhi->SetParameter(0,par[0]);
	return fdNdOmegaSigma1SmearedOffThetaVsThetaPhi->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2);
}

//-----------------------------------------------
// It Integrateds the dNdOmega_sig1 · Epsilon [# · deg^{-2}] vs Theta [deg] and Phi [rad] multiplied by Theta [deg] in order to obtain the N(Delta Omega) [#]
//
// x[0] 	= dTheta [deg]
// par[0] 	= offset distance [deg]
Double_t JDOptimization::IntegratedNdOmegaSigma1EpsilonThetaVsTheta(Double_t* x, Double_t* par)
{
	fdNdOmegaSigma1EpsilonThetaVsThetaPhi->SetParameter(0,par[0]);
	return fdNdOmegaSigma1EpsilonThetaVsThetaPhi->Integral(0.,x[0],0.,2*TMath::Pi(),1e-2);
}

//----------------------------------------------------
// It evaluates the dNdOmega·Epsilon multiplied by Sin(Theta) vs Theta and Phi.
// The dNdOmega can be also multiplied by Theta if we are not considering Spherical Coordinates.
//
// x[0]		= theta	[deg]
// x[1]		= phi	[rad]
// x0rad    = theta [rad]
// par[0] 	= offset distance [deg]
Double_t JDOptimization::dNdOmegaEpsilonThetaVsThetaPhi(Double_t* x, Double_t* par)
{
	Double_t X0rad =x[0]*dDeg2Rad;
	fdNdOmegaEpsilonVsThetaPhi->SetParameter(0,par[0]);

	if (GetIsSphericalCoordinates()==1)
	{
		//return fdNdOmegaEpsilonThetaVsThetaPhi->Eval(x[0],x[1])*TMath::Sin(X0rad);
		return fdNdOmegaEpsilonVsThetaPhi->Eval(x[0],x[1])*TMath::Sin(X0rad);

	}
	else
	{
		//return fdNdOmegaEpsilonThetaVsThetaPhi->Eval(x[0],x[1])*x[0];
		return fdNdOmegaEpsilonVsThetaPhi->Eval(x[0],x[1])*x[0];
	}
}

//----------------------------------------------------
//
Double_t JDOptimization::dNdOmegaSmearedOffVsThetaPhi(Double_t* x, Double_t* par)
{
	Double_t distCenterSource = TMath::Sqrt(x[0]*x[0]+par[0]*par[0]-2*x[0]*par[0]*TMath::Cos(x[1]+(TMath::Pi()/2)));

	return fdNdOmegaSmearedVsTheta->Eval(distCenterSource);
}

//----------------------------------------------------
//	dN/dOmega * Epsilon
// x[0] = theta [deg]
// x[1] = phi 	[rad]
//  par[0] = wobble [deg]
//
// dccR [deg] 			distance to fov center
Double_t JDOptimization::dNdOmegaEpsilonVsThetaPhi(Double_t* x, Double_t* par)
{
	Double_t dccR = TMath::Sqrt(TMath::Power(par[0],2)+TMath::Power(x[0],2)-2*par[0]*x[0]*TMath::Cos(x[1]+(TMath::Pi()/2)));

	TF1* ffdNdOmegaVsTheta = GetTF1dNdOmegaVsTheta();
	TF1* ffEpsilonVsDcc = GetTF1EpsilonVsDcc();

	return ffdNdOmegaVsTheta->Eval(x[0])*ffEpsilonVsDcc->Eval(dccR);
}

//----------------------------------------------------
//
Double_t JDOptimization::dNdOmegaSmearedEpsilonVsThetaPhi(Double_t* x, Double_t* par)
{
	Double_t dccR = TMath::Sqrt(TMath::Power(par[0],2)+TMath::Power(x[0],2)-2*par[0]*x[0]*TMath::Cos(x[1]+(TMath::Pi()/2)));
	TF1 *ffEpsilonVsDcc = jdInstrument->GetTF1EpsilonVsDcc();

	//return fdNdOmegaSmearedVsTheta->Eval(x[0])*jdInstrument->fEpsilonVsDcc->Eval(dccR);
	return fdNdOmegaSmearedVsTheta->Eval(x[0])*ffEpsilonVsDcc->Eval(dccR);

}

//----------------------------------------------------
//
Double_t JDOptimization::dNdOmegaSigma1SmearedEpsilonVsThetaPhi(Double_t* x, Double_t* par)
{
	Double_t dccR = TMath::Sqrt(TMath::Power(par[0],2)+TMath::Power(x[0],2)-2*par[0]*x[0]*TMath::Cos(x[1]+(TMath::Pi()/2)));

	TF1* ffEpsilonVsDcc = jdInstrument->GetTF1EpsilonVsDcc();

	return fdNdOmegaSigma1SmearedVsTheta->Eval(x[0])*ffEpsilonVsDcc->Eval(dccR);
}

//----------------------------------------------------
//
Double_t JDOptimization::dNdOmegaSmearedEpsilonOffVsThetaPhi(Double_t* x, Double_t* par)
{
	Double_t dccR = TMath::Sqrt(TMath::Power(par[0],2)+TMath::Power(x[0],2)-2*par[0]*x[0]*TMath::Cos(x[1]+(TMath::Pi()/2)));
	fdNdOmegaSmearedOffVsThetaPhi->SetParameter(0,2*par[0]);
	TF1 *ffEpsilonVsDcc = jdInstrument->GetTF1EpsilonVsDcc();

	//return fdNdOmegaSmearedOffVsThetaPhi->Eval(x[0],x[1])*jdInstrument->fEpsilonVsDcc->Eval(dccR);
	return fdNdOmegaSmearedOffVsThetaPhi->Eval(x[0],x[1])*ffEpsilonVsDcc->Eval(dccR);

}

//----------------------------------------------------
//
Double_t JDOptimization::dNdOmegaSigma1SmearedEpsilonOffVsThetaPhi(Double_t* x, Double_t* par)
{
	Double_t dccR = TMath::Sqrt(TMath::Power(par[0],2)+TMath::Power(x[0],2)-2*par[0]*x[0]*TMath::Cos(x[1]+(TMath::Pi()/2)));
	fdNdOmegaSigma1SmearedOffVsThetaPhi->SetParameter(0,2*par[0]);
	TF1 *ffEpsilonVsDcc = jdInstrument->GetTF1EpsilonVsDcc();

//	return fdNdOmegaSigma1SmearedOffVsThetaPhi->Eval(x[0],x[1])*jdInstrument->fEpsilonVsDcc->Eval(dccR);
	return fdNdOmegaSigma1SmearedOffVsThetaPhi->Eval(x[0],x[1])*ffEpsilonVsDcc->Eval(dccR);

}

//----------------------------------------------------
//
Double_t JDOptimization::dNdOmegaSmearedVsTheta(Double_t* x, Double_t* par)
{
	return gdNdOmegaSmeared->Eval(x[0]);
}

/////////////////////////////////
/////////////////////////////////
/////////////////////////////////
/////////////////////////////////
//
//   TO REMOVE !!!
//
//
////----------------------------------------------------
////	dN/dOmega * Epsilon * theta
//// x[0] = theta [deg]
//// x[1] = phi 	[rad]
//// par[0] = wobble distance
//// dccR [deg] 			distance to fov center
//Double_t JDOptimization::dNdOmegaEpsilonThetaVsThetaPhi(Double_t* x, Double_t* par)
//{
//	fdNdOmegaEpsilonVsThetaPhi->SetParameter(0,par[0]);
//	return fdNdOmegaEpsilonVsThetaPhi->Eval(x[0],x[1])*x[0];
//}
/////////////////////////////////
/////////////////////////////////
/////////////////////////////////
/////////////////////////////////


//----------------------------------------------------
//
Double_t JDOptimization::dNdOmegaSmearedEpsilonThetaVsThetaPhi(Double_t* x, Double_t* par)
{
	fdNdOmegaSmearedEpsilonVsThetaPhi->SetParameter(0,par[0]);
	return fdNdOmegaSmearedEpsilonVsThetaPhi->Eval(x[0],x[1])*x[0];
}

//----------------------------------------------------
//
Double_t JDOptimization::dNdOmegaSigma1SmearedEpsilonThetaVsThetaPhi(Double_t* x, Double_t* par)
{
	fdNdOmegaSigma1SmearedEpsilonVsThetaPhi->SetParameter(0,par[0]);
	return fdNdOmegaSigma1SmearedEpsilonVsThetaPhi->Eval(x[0],x[1])*x[0];
}

//----------------------------------------------------
//
Double_t JDOptimization::dNdOmegaSmearedEpsilonOffThetaVsThetaPhi(Double_t* x, Double_t* par)
{
	fdNdOmegaSmearedEpsilonOffVsThetaPhi->SetParameter(0,par[0]);
	return fdNdOmegaSmearedEpsilonOffVsThetaPhi->Eval(x[0],x[1])*x[0];
}

//----------------------------------------------------
//
Double_t JDOptimization::dNdOmegaSigma1SmearedEpsilonOffThetaVsThetaPhi(Double_t* x, Double_t* par)
{
	fdNdOmegaSigma1SmearedEpsilonOffVsThetaPhi->SetParameter(0,par[0]);
	return fdNdOmegaSigma1SmearedEpsilonOffVsThetaPhi->Eval(x[0],x[1])*x[0];
}

//----------------------------------------------------
//
Double_t JDOptimization::dNdOmegaSmearedThetaVsThetaPhi(Double_t* x, Double_t* par)
{
	return fdNdOmegaSmearedVsTheta->Eval(x[0])*x[0];
}

//----------------------------------------------------
//
Double_t JDOptimization::dNdOmegaSmearedOffThetaVsThetaPhi(Double_t* x, Double_t* par)
{
	fdNdOmegaSmearedOffVsThetaPhi->SetParameter(0,2*par[0]);
	return fdNdOmegaSmearedOffVsThetaPhi->Eval(x[0],x[1])*x[0];
}

//----------------------------------------------------
//	dN/dOmega_Sigma1 * Epsilon
//
// x[0] = theta [deg]
// x[1] = phi 	[rad]
//  par[0] = wobble [deg]
// dccR [deg] distance to fov center
Double_t JDOptimization::dNdOmegaSigma1EpsilonVsThetaPhi(Double_t* x, Double_t* par)
{
	Double_t dccR = TMath::Sqrt(TMath::Power(par[0],2)+TMath::Power(x[0],2)-2*par[0]*x[0]*TMath::Cos(x[1]+(TMath::Pi()/2)));

	TF1* ffdNdOmegaSigma1VsTheta = jdDarkMatter->GetTF1dNdOmegaSigma1VsTheta();

	TF1* ffEpsilonVsDcc = jdInstrument->GetTF1EpsilonVsDcc();

	return ffdNdOmegaSigma1VsTheta->Eval(x[0])*ffEpsilonVsDcc->Eval(dccR);
}

//----------------------------------------------------
//	dN/dOmega_off_Sigma1 * Epsilon
//
// x[0] = theta [deg]
// x[1] = phi 	[rad]
//  par[0] = wobble [deg]
// dccR [deg] distance to fov center
Double_t JDOptimization::dNdOmegaSigma1OffEpsilonVsThetaPhi(Double_t* x, Double_t* par)
{
	Double_t dccR = TMath::Sqrt(TMath::Power(par[0],2)+TMath::Power(x[0],2)-2*par[0]*x[0]*TMath::Cos(x[1]+(TMath::Pi()/2)));

	TF2* ffdNdOmegaSigma1OffThetaVsThetaPhi = jdDarkMatter->GetTF2dNdOmegaSigma1OffThetaVSThetaPhi();
	ffdNdOmegaSigma1OffThetaVsThetaPhi->SetParameter(0,2*par[0]);

	TF1* ffEpsilonVsDcc = jdInstrument->GetTF1EpsilonVsDcc();

	return ffdNdOmegaSigma1OffThetaVsThetaPhi->Eval(x[0],x[1])*ffEpsilonVsDcc->Eval(dccR);
}

//----------------------------------------------------
//
Double_t JDOptimization::dNdOmegaSigma1SmearedOffVsThetaPhi(Double_t* x, Double_t* par)
{
	Double_t distCenterSourceOff = TMath::Sqrt(TMath::Power(par[0],2)+TMath::Power(x[0],2)-2*par[0]*x[0]*TMath::Cos(x[1]+(TMath::Pi()/2)));

	return gdNdOmegaSigma1Smeared->Eval(distCenterSourceOff);
}

//----------------------------------------------------
//
Double_t JDOptimization::dNdOmegaSigma1SmearedVsTheta(Double_t* x, Double_t* par)
{
	return gdNdOmegaSigma1Smeared->Eval(x[0]);
}

//----------------------------------------------------
//	dN/dOmega * Epsilon * theta
// x[0] = theta [deg]
// x[1] = phi 	[rad]
// par[0] = wobble distance
// dccR [deg] 			distance to fov center
Double_t JDOptimization::dNdOmegaSigma1EpsilonThetaVsThetaPhi(Double_t* x, Double_t* par)
{
	fdNdOmegaSigma1EpsilonVsThetaPhi->SetParameter(0,par[0]);
	return fdNdOmegaSigma1EpsilonVsThetaPhi->Eval(x[0],x[1])*x[0];
}

//----------------------------------------------------
//	dN/dOmega_off * Epsilon * theta
// x[0] = theta [deg]
// x[1] = phi 	[rad]
// par[0] = wobble distance
// dccR [deg] 			distance to fov center
Double_t JDOptimization::dNdOmegaSigma1OffEpsilonThetaVsThetaPhi(Double_t* x, Double_t* par)
{
	fdNdOmegaSigma1OffEpsilonVsThetaPhi->SetParameter(0,par[0]);
	return fdNdOmegaSigma1OffEpsilonVsThetaPhi->Eval(x[0],x[1])*x[0];
}

//----------------------------------------------------
//
Double_t JDOptimization::dNdOmegaSigma1SmearedThetaVsThetaPhi(Double_t* x, Double_t* par)
{
	return fdNdOmegaSigma1SmearedVsTheta->Eval(x[0])*x[0];
}

//----------------------------------------------------
//
Double_t JDOptimization::dNdOmegaSigma1SmearedOffThetaVsThetaPhi(Double_t* x, Double_t* par)
{
	fdNdOmegaSigma1SmearedOffVsThetaPhi->SetParameter(0,par[0]);
	return fdNdOmegaSigma1SmearedOffVsThetaPhi->Eval(x[0],x[1])*x[0];
}

//----------------------------------------------------
//	dN/dOmegaOff * Epsilon
// x[0] = theta [deg]
// x[1] = phi 	[rad]
//  par[0] = wobble [deg]
//
// dccR [deg] 			distance to fov center
Double_t JDOptimization::dNdOmegaOffEpsilonVsThetaPhi(Double_t* x, Double_t* par)
{
	Double_t dccR = TMath::Sqrt(TMath::Power(par[0],2)+TMath::Power(x[0],2)-2*par[0]*x[0]*TMath::Cos(x[1]+(TMath::Pi()/2)));

	TF2* ffdNdOmegaOffVsThetaPhi = jdDarkMatter->GetTF2dNdOmegaOffVsThetaPhi();
	ffdNdOmegaOffVsThetaPhi->SetParameter(0,2*par[0]);

	TF1* ffEpsilonVsDcc = jdInstrument->GetTF1EpsilonVsDcc();

	return ffdNdOmegaOffVsThetaPhi->Eval(x[0],x[1])*ffEpsilonVsDcc->Eval(dccR);
}

//----------------------------------------------------
//	dN/dOmegaOffTheta * Epsilon
// x[0] = theta [deg]
// x[1] = phi 	[rad]
//  par[0] = wobble [deg]
//
// dccR [deg] 			distance to fov center
Double_t JDOptimization::dNdOmegaOffEpsilonThetaVsThetaPhi(Double_t* x, Double_t* par)
{
	fdNdOmegaOffEpsilonVsThetaPhi->SetParameter(0,par[0]);
	return fdNdOmegaOffEpsilonVsThetaPhi->Eval(x[0],x[1])*x[0];
}

void JDOptimization::SetdNdOmegaSmeared()
{
	Double_t thetaMax=GetThetaMax();
	Double_t resolution = GetBinResolution();			//[deg/bin]
	Int_t iNumBins=thetaMax/resolution;
	Int_t numBins=iNumBins+1;

//	TF1* pointSpreadFunction = new TF1("pointSpreadFunction","1/([1]*TMath::Sqrt(2*TMath::Pi()))*exp(-0.5*((x-[0])/[1])^2)",0,thetaMax);
	TF2* pointSpreadFunction = new TF2("pointSpreadFunction","1/([1]*[1]*2*TMath::Pi())*exp(-0.5*(((x-[0])^2+(y-[0])^2)/[1]^2))",-thetaMax,thetaMax,-thetaMax,thetaMax);
	TString instrument = jdInstrument->GetInstrumentName();

	if(instrument=="MAGICPointLike")
	{
		pointSpreadFunction->SetParameter(0,0);
		pointSpreadFunction->SetParameter(1,0.155); // 68% containment at 0.155º for 100 GeV - see Arxiv1409.5594
	}
	else if(instrument=="CTANorth50To80GeV")
	{
		pointSpreadFunction->SetParameter(0,0);
		pointSpreadFunction->SetParameter(1,0.11);  // 68% containment at 0.11º for 100 GeV - see Arxiv1705.01790
	}
	else if(instrument=="MyInstrument")
	{
		pointSpreadFunction->SetParameter(0,0);
		pointSpreadFunction->SetParameter(1,GetMyInstrument68Containment());  // 68% containment at 0.11º for 100 GeV - see Arxiv1705.01790
	}
	else
	{
		GetWarning();
	}

	TH2D* smearingdNdOmegaBefore = new TH2D("smearingdNdOmegaBefore","",numBins,-thetaMax,thetaMax,numBins,-thetaMax,thetaMax);
	TH2D* smearingdNdOmegaAfter = new TH2D("smearingdNdOmegaAfter","",numBins,-thetaMax,thetaMax,numBins,-thetaMax,thetaMax);
	TH2D* smearingGauss = new TH2D("smearingGauss","",numBins,-thetaMax,thetaMax,numBins,-thetaMax,thetaMax);

	TF1* ffdNdOmegaVsTheta = jdDarkMatter->GetTF1dNdOmegaVsTheta();

	Double_t gaussVolume=0;
	for(Int_t i=1; i<numBins+1; i++)
	{
		Double_t distSourceCenterX = smearingGauss->ProjectionX()->GetBinCenter(i);
		for(Int_t j=1; j<numBins+1; j++)
		{
			Double_t distSourceCenterY = smearingGauss->ProjectionX()->GetBinCenter(j);
			Double_t distanceSourceCenter = TMath::Sqrt(distSourceCenterX*distSourceCenterX + distSourceCenterY*distSourceCenterY);
			smearingGauss->SetBinContent(i,j,pointSpreadFunction->Eval(distSourceCenterX,distSourceCenterY));
			//smearingdNdOmegaBefore->SetBinContent(i,j,jdDarkMatter->fdNdOmegaVsTheta->Eval(distanceSourceCenter));
			smearingdNdOmegaBefore->SetBinContent(i,j,ffdNdOmegaVsTheta->Eval(distanceSourceCenter));
			smearingdNdOmegaAfter->SetBinContent(i,j,0.);
			gaussVolume=gaussVolume+smearingGauss->GetBinContent(i,j);
		}
	}
	smearingGauss->Scale(1/gaussVolume);

	for(Int_t binCenterX=1; binCenterX<numBins+1; binCenterX++)
	{
		for(Int_t binCenterY=1; binCenterY<numBins+1; binCenterY++)
		{
			for(Int_t binSmearX=1; binSmearX<numBins+1; binSmearX++)
			{
				Int_t binDeltaX = binCenterX - binSmearX;
				for(Int_t binSmearY=1; binSmearY<numBins+1; binSmearY++)
				{
					Int_t binDeltaY = binCenterY - binSmearY;

					smearingdNdOmegaAfter->SetBinContent(binSmearX,binSmearY,
							smearingdNdOmegaAfter->GetBinContent(binSmearX,binSmearY)+
							smearingdNdOmegaBefore->GetBinContent(binCenterX,binCenterY)*
							smearingGauss->GetBinContent(iNumBins/2+1+binDeltaX,iNumBins/2+1+binDeltaY));
				}
			}
		}
	}

	gdNdOmegaSmeared = new TGraph();
	for(Int_t binCenterX=0; binCenterX<iNumBins/2+1; binCenterX++)
	{
		Double_t theta = smearingdNdOmegaAfter->ProjectionX()->GetBinCenter(iNumBins/2+1+binCenterX)-
				smearingGauss->ProjectionX()->GetBinCenter(iNumBins/2+1);
		Double_t dNdOmegaSmeared = smearingdNdOmegaAfter->GetBinContent(iNumBins/2+1+1+binCenterX,iNumBins/2+1);

		gdNdOmegaSmeared->SetPoint(binCenterX-1,theta,dNdOmegaSmeared);
	}

	SetIsdNdOmegaSmeared(1);
}

void JDOptimization::SetdNdOmegaSigma1Smeared()
{
	Double_t thetaMax=GetThetaMax();
	Double_t resolution = GetBinResolution();			//[deg/bin]
	Int_t iNumBins=thetaMax/resolution;
	Int_t numBins=iNumBins+1;

	TF1* pointSpreadFunction = new TF1("pointSpreadFunction","1/([1]*TMath::Sqrt(2*TMath::Pi()))*exp(-0.5*((x-[0])/[1])^2)",0,thetaMax);
	TString instrument = jdInstrument->GetInstrumentName();
	if(instrument=="MAGICPointLike")
	{
		pointSpreadFunction->SetParameter(0,0);
		pointSpreadFunction->SetParameter(1,0.1);
	}
	else
	{
		GetWarning();
	}


	TH2D* smearingdNdOmegaSigma1Before = new TH2D("smearingdNdOmegaSigma1Before","",numBins,-thetaMax,thetaMax,numBins,-thetaMax,thetaMax);
	TH2D* smearingdNdOmegaSigma1After = new TH2D("smearingdNdOmegaSigma1After","",numBins,-thetaMax,thetaMax,numBins,-thetaMax,thetaMax);
	TH2D* smearingGauss = new TH2D("smearingGauss","",numBins,-thetaMax,thetaMax,numBins,-thetaMax,thetaMax);

	TF1* ffdNdOmegaSigma1VsTheta=jdDarkMatter->GetTF1dNdOmegaSigma1VsTheta();

	Double_t gaussVolume=0;
	for(Int_t i=1; i<numBins+1; i++)
	{
		Double_t distSourceCenterX = smearingGauss->ProjectionX()->GetBinCenter(i);
		for(Int_t j=1; j<numBins+1; j++)
		{
			Double_t distSourceCenterY = smearingGauss->ProjectionX()->GetBinCenter(j);
			Double_t distanceSourceCenter = TMath::Sqrt(distSourceCenterX*distSourceCenterX + distSourceCenterY*distSourceCenterY);
			smearingGauss->SetBinContent(i,j,pointSpreadFunction->Eval(distanceSourceCenter));
			smearingdNdOmegaSigma1Before->SetBinContent(i,j,ffdNdOmegaSigma1VsTheta->Eval(distanceSourceCenter));
			smearingdNdOmegaSigma1After->SetBinContent(i,j,0.);
			gaussVolume=gaussVolume+smearingGauss->GetBinContent(i,j);
		}
	}
	smearingGauss->Scale(1/gaussVolume);

	for(Int_t binCenterX=1; binCenterX<numBins+1; binCenterX++)
	{
		for(Int_t binCenterY=1; binCenterY<numBins+1; binCenterY++)
		{
			for(Int_t binSmearX=1; binSmearX<numBins+1; binSmearX++)
			{
				Int_t binDeltaX = binCenterX - binSmearX;
				for(Int_t binSmearY=1; binSmearY<numBins+1; binSmearY++)
				{
					Int_t binDeltaY = binCenterY - binSmearY;

					smearingdNdOmegaSigma1After->SetBinContent(binSmearX,binSmearY,
							smearingdNdOmegaSigma1After->GetBinContent(binSmearX,binSmearY)+
							smearingdNdOmegaSigma1Before->GetBinContent(binCenterX,binCenterY)*
							smearingGauss->GetBinContent(iNumBins/2+1+binDeltaX,iNumBins/2+1+binDeltaY));
				}
			}
		}
	}

	gdNdOmegaSigma1Smeared = new TGraph();
	for(Int_t binCenterX=0; binCenterX<iNumBins/2+1; binCenterX++)
	{
		Double_t theta = smearingdNdOmegaSigma1After->ProjectionX()->GetBinCenter(iNumBins/2+1+binCenterX)-
				smearingGauss->ProjectionX()->GetBinCenter(iNumBins/2+1);
		Double_t dNdOmegaSigma1Smeared = smearingdNdOmegaSigma1After->GetBinContent(iNumBins/2+1+1+binCenterX,iNumBins/2+1);

		gdNdOmegaSigma1Smeared->SetPoint(binCenterX-1,theta,dNdOmegaSigma1Smeared);
	}

	SetIsdNdOmegaSigma1Smeared(1);
}

//----------------------------------------------------
//
//
//
//
//
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
	cout << "   ***      4 : Q4 = J_PSF/theta_PSF                           		***" << endl;
	cout << "   ***                                                         		***" << endl;
	cout << "   ***      12: Q12 = J_on_1sm-J_off_1sm/theta				       		***" << endl;
	cout << "   ***      13: Q13 = J_on_eff-J_off_eff/theta_eff						***" << endl;
	cout << "   ***      23: Q23 = J_1sm_eff/theta_eff								***" << endl;
	cout << "   ***      123: Q123 = J_on_1sm_eff-J_off_1sm_eff/theta_eff			***" << endl;
	cout << "   ***      123: Q1234 = ...											***" << endl;
	cout << "   ***                                                         		***" << endl;
	cout << "   ***********************************************************************" << endl;
	cout << endl;
}

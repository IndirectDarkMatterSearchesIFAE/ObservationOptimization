/*
 * JDOptimization.h
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




#ifndef JDOptimization_H_
#define JDOptimization_H_

#include "JDInstrument.h"
#include "JDDarkMatter.h"

#include "TString.h"


	//	IDEAL: 									Q0 = J_on/theta
	//	LEAKAGE EFFECT: 						Q1 = J_on-J_off/theta
	//  UNCERTAINTY EFFECT: 					Q2 = J_1sm/theta
	// 	ACCEPTANCE EFFECT:						Q3 = J_eff/theta_eff
	//  LEAKAGE + UNCERTAINTY:  				Q12 = J_on_1sm-J_off_1sm/theta
	//  LEAKAGE + ACCEPTANCE:   				Q13 = J_on_eff-J_off_eff/theta_eff
	//  UNCERTAINTY + ACCEPTANCE:   			Q23 = J_1sm_eff/theta_eff
	//  LEAKAGE + UNCERTAINTY + ACCEPTANCE: 	Q123 = J_on_1sm_eff-J_off_1sm_eff/theta_eff

class JDOptimization {
public:
	JDOptimization();
//	JDOptimization(TString txtFile, TString myInstrumentPath, TString instrumentName, Double_t distCameraCenter, Double_t wobble);
	JDOptimization(TString author, TString source, TString candidate, TString mySourcePath, TString myInstrumentPath, TString instrumentName, Double_t distCameraCenter, Double_t wobble);
	virtual ~JDOptimization();



	void GetListOfQFactors();

	Bool_t GetIsSphericalCoordinates() 			{return jdDarkMatter->GetIsSphericalCoordinates();}
//
	Double_t GetBinResolution()					{return dBinResolution;}
//
	TF1* GetTF1QFactorVsTheta(Int_t type=0, Double_t thetaNorm=0.4)
	{
		if(type==0)
		{
			fQ0FactorVsTheta->SetParameter(0, thetaNorm);
			return fQ0FactorVsTheta;
		}
		else if(type==1)
		{
			fQ1FactorVsTheta->SetParameter(0, thetaNorm);
			return fQ1FactorVsTheta;
		}

		else if(type==2)
		{
			fQ2FactorVsTheta->SetParameter(0, thetaNorm);
			return fQ2FactorVsTheta;
		}

		else if (type==3)
		{
			fQ3FactorVsTheta->SetParameter(0, thetaNorm);
			return fQ3FactorVsTheta;
		}

//		else if (type==12 || type==21)
//		{
//			fQ12FactorVsTheta->SetParameter(0, thetaNorm);
//			return fQ12FactorVsTheta;
//		}
//
//		else if (type==13 || type==31)
//		{
//			fQ13FactorVsTheta->SetParameter(0, thetaNorm);
//			return fQ13FactorVsTheta;
//		}
//
//		else if (type==23 || type==32)
//		{
//			fQ23FactorVsTheta->SetParameter(0, thetaNorm);
//			return fQ23FactorVsTheta;
//		}
//
//		else if (type==123 || type==132 || type==213 || type==231 || type==312 || type==321)
//		{
//			fQ123FactorVsTheta->SetParameter(0, thetaNorm);
//			return fQ123FactorVsTheta;
//		}
//
//		else if (type==124 || type==142 || type==214 || type==241 || type==412 || type==421)
//		{
//			fQ124FactorVsTheta->SetParameter(0, thetaNorm);
//			return fQ124FactorVsTheta;
//		}
//
//		else if (type==134 || type==143 || type==314 || type==341 || type==413 || type==431)
//		{
//			fQ134FactorVsTheta->SetParameter(0, thetaNorm);
//			return fQ134FactorVsTheta;
//		}
//
//		else if (type==234 || type==243 || type==324 || type==342 || type==423 || type==432)
//		{
//			fQ134FactorVsTheta->SetParameter(0, thetaNorm);
//			return fQ134FactorVsTheta;
//		}
//
		else if (type==1234 || type==1243 || type==1324 || type==1342 || type==1423 || type==1432 ||
				 type==2341 || type==2342 || type==2423 || type==2432 || type==2143 || type==2143 ||
				 type==3412 || type==3421 || type==3124 || type==3142 || type==3241 || type==3214 ||
				 type==4123 || type==4132 || type==4231 || type==4213 || type==4321 || type==4312 )

		{
			fQ1234FactorVsTheta->SetParameter(0, thetaNorm);
			return fQ1234FactorVsTheta;
		}

		else
		{
			TF1* f1;
			cout << endl;
			cout << "   ***************************************************************" << endl;
			cout << "   ***   WARNING: wrong Qfactor type!!!                        ***" << endl;
			cout << "   ***************************************************************" << endl;
			cout << endl;
			GetListOfQFactors();
			return f1;
		}
	}

//	///////////////////////////////////////////////////////////////////////////////////////
//	// This function gives a value and a range around this value of the theta optimal and the wobble optimal
void GetOptimalThetaAndWobble(Double_t &thetaOpt, Double_t &thetaOptRangMin, Double_t &thetaOptRangMax, Double_t &wobbleOpt, Double_t &wobbleOptRangMin, Double_t &wobbleOptRangMax, Int_t type=0)
{
	TH2D* h2;
	if(!th2QFactorVsThetaWobble) h2 = th2QFactorVsThetaWobble;
	else h2 = GetTH2QFactorVsThetaWobble(type);

	Int_t numBinsX = h2->GetNbinsX();
	Int_t numBinsY = h2->GetNbinsY();

	Double_t qfactorMax=0;
	Double_t qfactorRange=1;
	Double_t thetaBin=0;
	Double_t wobbleBin=0;

	for(Int_t i=1; i<numBinsX+1; i++)
	{
		for(Int_t j=0; j<numBinsY; j++)
			{
				Double_t qfactor = h2->GetBinContent(i,j);
				if(qfactor>qfactorMax)
				{
					qfactorMax=qfactor;
					thetaOpt=h2->ProjectionX()->GetBinLowEdge(i);
					wobbleOpt=h2->ProjectionY()->GetBinLowEdge(j);
					thetaBin=i;
					wobbleBin=j;
				}

			}
	}

	Double_t tolerance = 0.30; // QUIM: This is hardcoded...

	for(Int_t i=1; i<numBinsX+1; i++)
	{
		Double_t qfactorRange = h2->GetBinContent(i,wobbleBin);
		if(qfactorRange>=(1-tolerance)*qfactorMax)
		{
			thetaOptRangMin=h2->ProjectionX()->GetBinLowEdge(i);
			break;
		}
	}

	for(Int_t i=1; i<numBinsX+1; i++)
	{
		Double_t qfactorRange = h2->GetBinContent(numBinsX+1-i,wobbleBin);
		if(qfactorRange>=(1-tolerance)*qfactorMax)
		{
			thetaOptRangMax=h2->ProjectionX()->GetBinLowEdge(numBinsX+1-i);
			break;
		}
	}

	for(Int_t j=1; j<numBinsY+1; j++)
	{
		Double_t qfactorRange = h2->GetBinContent(thetaBin,j);
		if(qfactorRange>=(1-tolerance)*qfactorMax)
		{
			wobbleOptRangMin=h2->ProjectionY()->GetBinLowEdge(j);
			break;
		}
	}

	for(Int_t j=1; j<numBinsY+1; j++)
	{
		Double_t qfactorRange = h2->GetBinContent(thetaBin,numBinsY+1-j);
		if(qfactorRange>=(1-tolerance)*qfactorMax)
		{
			wobbleOptRangMax=h2->ProjectionY()->GetBinLowEdge(numBinsY+1-j);
			break;
		}
	}

	if(qfactorMax>0.)
	{
		cout << "   ****************************************" << endl;
		cout << "   ***                                  ***" << endl;
		cout << "   ***  Optimal theta and wobble found  ***" << endl;
		cout << "   ***                                  ***" << endl;
		cout << "   ****************************************" << endl;
	}
	else
	{
		cout << "   **************************************************" << endl;
		cout << "   ***                                            ***" << endl;
		cout << "   ***  WARNING:                                  ***" << endl;
		cout << "   ***  problem occurred maximizing the qfactor   ***" << endl;
		cout << "   ***                                            ***" << endl;
		cout << "   **************************************************" << endl;
	}

	return;
}

//	//////////////////////////////////////////////////////////////////////////////////////////////////////
//	// This TH2 is filled with the content of the TF2 corresponding to GetTF2QFactorvsThetaAndWobble
TH2D* GetTH2QFactorVsThetaWobble(Int_t type=0, Double_t thetaNorm=-1, Double_t wobbleNorm=-1)
{
	TF2* qFactor=GetTF2QFactorVsThetaWobble(type);

	Double_t resolution = GetBinResolution();			//[deg/bin]
	Double_t thetaMax = GetThetaMax();					// [deg]
	Int_t numBinsX = thetaMax/resolution; 				// [#bins]
	Double_t wobbleMax = GetDistCameraCenterMax();		// [deg]
	Int_t numBinsY = wobbleMax/resolution; 				// [#bins]

	Bool_t verbose=0;
	if (verbose==1)
	{
		cout << " resolution = " << resolution << endl;
		cout << " thetaMax = " << thetaMax << endl;
		cout << " numBinsX = " << numBinsX << endl;
		cout << " numBinsY = " << numBinsX << endl;
		cout << " wobbleMax = " << wobbleMax << endl;
	}


	th2QFactorVsThetaWobble = new TH2D("h2","",numBinsX,0.,thetaMax,numBinsY,0.,wobbleMax);
	for(Int_t i=1; i<numBinsX+1; i++)
	{
		for(Int_t j=1; j<numBinsY+1; j++)
		{
			th2QFactorVsThetaWobble->SetBinContent(i,j,qFactor->Eval(th2QFactorVsThetaWobble->ProjectionX()->GetBinCenter(i),th2QFactorVsThetaWobble->ProjectionY()->GetBinCenter(j)));
		}
	}

	if(thetaNorm <0. || wobbleNorm<0.)
	{
		th2QFactorVsThetaWobble->Scale(1/(th2QFactorVsThetaWobble->GetMaximum()));
	}
	else
	{
		Int_t iThetaNorm = th2QFactorVsThetaWobble->ProjectionX()->FindBin(thetaNorm);
		Int_t jWobbleNorm = th2QFactorVsThetaWobble->ProjectionY()->FindBin(wobbleNorm);
		Double_t normValue = th2QFactorVsThetaWobble->GetBinContent(iThetaNorm,jWobbleNorm);
		th2QFactorVsThetaWobble->Scale(1/(normValue));
	}
	return th2QFactorVsThetaWobble;
}
//
TF2* GetTF2QFactorVsThetaWobble(Int_t type=0)
{
		if(type==0)
		{
			return fQ0FactorVsThetaWobble;
		}
		else if(type==1)
		{
			return fQ1FactorVsThetaWobble;
		}

		else if(type==2)
		{
			return fQ2FactorVsThetaWobble;
		}

		else if(type==3)
		{
			return fQ3FactorVsThetaWobble;
		}

		else if(type==4)
		{
			return fQ4FactorVsThetaWobble;
		}

		else if(type==12 || type==21)
		{
			return fQ12FactorVsThetaWobble;
		}

		else if(type==13 || type==31)
		{
			return fQ13FactorVsThetaWobble;
		}

		else if (type==14 || type==41)
		{
			return fQ14FactorVsThetaWobble;
		}

		else if(type==23 || type==32)
		{
			return fQ23FactorVsThetaWobble;
		}

		else if(type==24 || type==42)
		{
			return fQ24FactorVsThetaWobble;
		}

		else if(type==34 || type==43)
		{
			return fQ34FactorVsThetaWobble;
		}

		else if (type==123 || type==132 || type==213 || type==231 || type==312 || type==321)
		{
			return fQ123FactorVsThetaWobble;
		}

		else if (type==124 || type==142 || type==214 || type==241 || type==412 || type==421)
		{
			return fQ124FactorVsThetaWobble;
		}

		else if (type==134 || type==143 || type==314 || type==341 || type==413 || type==431)
		{
			return fQ134FactorVsThetaWobble;
		}

		else if (type==234 || type==243 || type==324 || type==342 || type==423 || type==432)
		{
			return fQ234FactorVsThetaWobble;
		}

		else if (type==1234 || type==1243 || type==1324 || type==1342 || type==1423 || type==1432 ||
				 type==2341 || type==2342 || type==2423 || type==2432 || type==2143 || type==2143 ||
				 type==3412 || type==3421 || type==3124 || type==3142 || type==3241 || type==3214 ||
				 type==4123 || type==4132 || type==4231 || type==4213 || type==4321 || type==4312 )

		{
			return fQ1234FactorVsThetaWobble;
		}

		else
		{
			TF2* f2;
			cout << endl;
			cout << "   ***************************************************************" << endl;
			cout << "   ***   WARNING: wrong Qfactor type!!!                        ***" << endl;
			cout << "   ***************************************************************" << endl;
			cout << endl;
			GetListOfQFactors();
			return f2;
		}
	}

	/**************************************************/

//	//***** JDDarkMatter Getters
	void GetWarning()						{return jdDarkMatter->GetWarning();}
//
	Bool_t GetIsBonnivard()					{return jdDarkMatter->GetIsBonnivard();}
	Bool_t GetIsGeringer()					{return jdDarkMatter->GetIsGeringer();}
//	Bool_t GetIsJFactor()					{return jdDarkMatter->GetIsJFactor();}
//	Bool_t GetIsJFactorSigma1()				{return jdDarkMatter->GetIsJFactorSigma1();}
//	Bool_t GetIsdNdOmega()					{return jdDarkMatter->GetIsdNdOmega();}
//	Bool_t GetIsdNdOmegaSigma1()			{return jdDarkMatter->GetIsdNdOmegaSigma1();}
	Bool_t GetIsdNdOmegaSmeared() 			{return bIsdNdOmegaSmeared;}
	Bool_t GetIsdNdOmegaSigma1Smeared() 	{return bIsdNdOmegaSigma1Smeared;}
//
//
	TString GetSourceName()					{return jdDarkMatter->GetSourceName();}
	TString GetCandidate()					{return jdDarkMatter->GetCandidate();}
//	TString GetAuthor()						{return jdDarkMatter->GetAuthor();}
//	TString GetSourcePath()					{return jdDarkMatter->GetSourcePath();}
//
	Double_t GetThetaMax()						{return jdDarkMatter->GetThetaMax();}
//	Double_t GetJFactorMax()					{return jdDarkMatter->GetJFactorMax();}
//	Double_t GetJFactorSigma1Max()				{return jdDarkMatter->GetJFactorSigma1Max();}
//	Double_t GetJFactorMin()					{return jdDarkMatter->GetJFactorMin();}
//	Double_t GetJFactorSigma1Min()				{return jdDarkMatter->GetJFactorSigma1Min();}
//
//
//	TF1* GetTF1JFactorVsTheta()						{return jdDarkMatter->GetTF1JFactorVsTheta();}
//	TF1* GetTF1JFactorSigma1VsTheta()				{return jdDarkMatter->GetTF1JFactorSigma1VsTheta();}
	TF1* GetTF1IntegratedNdOmegaThetaVsTheta()		{return jdDarkMatter->GetTF1IntegratedNdOmegaThetaVsTheta();}
//	TF1* GetTF1IntegratedNdOmegaSigma1VsTheta()		{return jdDarkMatter->GetTF1IntegratedNdOmegaSigma1VsTheta();}
//	TF1* GetTF1IntegratedNdOmegaOffVsTheta()		{return jdDarkMatter->GetTF1IntegratedNdOmegaOffVsTheta(2*jdInstrument->GetWobbleDistance());}
//	TF1* GetTF1IntegratedNdOmegaSigma1OffVsTheta()	{return jdDarkMatter->GetTF1IntegratedNdOmegaSigma1OffVsTheta(2*jdInstrument->GetWobbleDistance());}
	TF1* GetTF1dNdOmegaVsTheta()					{return jdDarkMatter->GetTF1dNdOmegaVsTheta();}
//	//TF1* GetTF1LOSSigma1VsTheta()					{return jdDarkMatter->GetTF1LOSSigma1VsTheta();}
//	//TF1* GetTF1NormLOSVsTheta()						{return jdDarkMatter->GetTF1NormLOSVsTheta(1.0);}
//	//TF1* GetTF1NormLOS_m1VsTheta()					{return jdDarkMatter->GetTF1NormLOSSigma1VsTheta(1.0);}
//	TF2* GetTF2dNdOmegaThetaVSThetaPhi()			{return jdDarkMatter->GetTF2dNdOmegaThetaVSThetaPhi();}
//	TF2* GetTF2dNdOmegaSigma1ThetaVSThetaPhi()		{return jdDarkMatter->GetTF2dNdOmegaSigma1ThetaVSThetaPhi();}
//	TF2* GetTF2dNdOmegaOffVSThetaPhi()				{return jdDarkMatter->GetTF2dNdOmegaOffVSThetaPhi(2*jdInstrument->GetWobbleDistance());}
////
//
//	// QUIM NEW
//	TF2* GetTF2dNdOmegaEpsilonVsThetaPhi()
//	{
//		fdNdOmegaEpsilonVsThetaPhi->SetParameter(0,jdInstrument->GetWobbleDistance());
//		return fdNdOmegaEpsilonVsThetaPhi;
//	}
//	TF2* GetTF2dNdOmegaOffEpsilonVsThetaPhi()
//	{
//		fdNdOmegaOffEpsilonVsThetaPhi->SetParameter(0,jdInstrument->GetWobbleDistance());
//		return fdNdOmegaOffEpsilonVsThetaPhi;
//	}
//	TF2* GetTF2dNdOmegaEpsilonThetaVsThetaPhi()
//	{
//		fdNdOmegaEpsilonThetaVsThetaPhi->SetParameter(0,jdInstrument->GetWobbleDistance());
//		return fdNdOmegaEpsilonThetaVsThetaPhi;
//	}
//	TF2* GetTF2dNdOmegaOffEpsilonThetaVsThetaPhi()
//	{
//		fdNdOmegaOffEpsilonThetaVsThetaPhi->SetParameter(0,jdInstrument->GetWobbleDistance());
//		return fdNdOmegaOffEpsilonThetaVsThetaPhi;
//	}
//
//	//***** JDDarkMatter Setters
//	void SetCandidate(TString candidate)							{jdDarkMatter->SetCandidate(candidate);}
//
//	//************************************************
//
//	//***** JDInstrument Getters
	Bool_t GetIsIdeal()						{return jdInstrument->GetIsIdeal();}
	Bool_t GetIsIntegraldNdOmegaOnMinusOFF() 		{return bIsJFactorOnLessOff;}
//
	TF1* GetTF1EpsilonVsDcc()				{return jdInstrument->GetTF1EpsilonVsDcc();}
//	TF1* GetTF1EfficiencyVsTheta()			{return jdInstrument->GetTF1EfficiencyVsTheta(jdInstrument->GetWobbleDistance());}
//
//	TF2* GetTF2EpsilonVsThetaPhi()		{return jdInstrument->GetTF2EpsilonVsThetaPhi();}
//	TF2* GetTF2EpsilonThetaVsThetaPhi()	{return jdInstrument->GetTF2EpsilonThetaVsThetaPhi();}
//	TF2* GetTF2EpsilonVsXAndY()				{return jdInstrument->GetTF2EpsilonVsXAndY();}
//
//	TString GetInstrumentName()				{return jdInstrument->GetInstrumentName();}
//	TString GetInstrumentPath()				{return jdInstrument->GetInstrumentPath();}
//
	Double_t GetDistCameraCenterMax()		{return jdInstrument->GetDistCameraCenterMax();}
	Double_t GetWobbleDistance()			{return jdInstrument->GetWobbleDistance();}



protected:

	// Composition C++ scheme
	JDDarkMatter* jdDarkMatter;
	JDInstrument* jdInstrument;

	void CreateFunctions();
//
	void SetdNdOmegaSmeared();
	void SetdNdOmegaSigma1Smeared();
	void SetIsdNdOmegaSmeared(Bool_t isdNdOmegaSmeared)					{bIsdNdOmegaSmeared = isdNdOmegaSmeared;}
	void SetIsdNdOmegaSigma1Smeared(Bool_t isdNdOmegaSigma1Smeared)		{bIsdNdOmegaSigma1Smeared = isdNdOmegaSigma1Smeared;}
//
//
	Double_t Q0FactorVsTheta(Double_t* x, Double_t* par);
	Double_t Q1FactorVsTheta(Double_t* x, Double_t* par);
	Double_t Q2FactorVsTheta(Double_t* x, Double_t* par);
	Double_t Q3FactorVsTheta(Double_t* x, Double_t* par);
//
//	Double_t Q12FactorVsTheta(Double_t* x, Double_t* par);
//	Double_t Q13FactorVsTheta(Double_t* x, Double_t* par);
//	Double_t Q23FactorVsTheta(Double_t* x, Double_t* par);
//	Double_t Q123FactorVsTheta(Double_t* x, Double_t* par);
//
	Double_t Q0FactorVsThetaWobble(Double_t* x, Double_t* par);
	Double_t Q1FactorVsThetaWobble(Double_t* x, Double_t* par);
	Double_t Q2FactorVsThetaWobble(Double_t* x, Double_t* par);
	Double_t Q3FactorVsThetaWobble(Double_t* x, Double_t* par);
	Double_t Q4FactorVsThetaWobble(Double_t* x, Double_t* par);
	Double_t Q12FactorVsThetaWobble(Double_t* x, Double_t* par);
	Double_t Q13FactorVsThetaWobble(Double_t* x, Double_t* par);
	Double_t Q14FactorVsThetaWobble(Double_t* x, Double_t* par);
	Double_t Q23FactorVsThetaWobble(Double_t* x, Double_t* par);
	Double_t Q24FactorVsThetaWobble(Double_t* x, Double_t* par);
	Double_t Q34FactorVsThetaWobble(Double_t* x, Double_t* par);
	Double_t Q123FactorVsThetaWobble(Double_t* x, Double_t* par);
	Double_t Q124FactorVsThetaWobble(Double_t* x, Double_t* par);
	Double_t Q134FactorVsThetaWobble(Double_t* x, Double_t* par);
	Double_t Q234FactorVsThetaWobble(Double_t* x, Double_t* par);
	Double_t Q1234FactorVsThetaWobble(Double_t* x, Double_t* par);
//
//	// ...VsTheta
//	Double_t dNdOmegaEpsilonVsTheta(Double_t* x, Double_t* par);
	Double_t dNdOmegaSmearedVsTheta(Double_t* x, Double_t* par);
//	Double_t dNdOmegaSigma1SmearedVsTheta(Double_t* x, Double_t* par);
//
//	// ...VsThetaPhi
//	Double_t dNdOmegaSmearedVsThetaPhi(Double_t* x, Double_t* par);
	Double_t dNdOmegaEpsilonVsThetaPhi(Double_t* x, Double_t* par);
	Double_t dNdOmegaSmearedEpsilonVsThetaPhi(Double_t* x, Double_t* par);
//	Double_t dNdOmegaSigma1SmearedEpsilonVsThetaPhi(Double_t* x, Double_t* par);
	Double_t dNdOmegaSmearedEpsilonOffVsThetaPhi(Double_t* x, Double_t* par);
	Double_t dNdOmegaSigma1SmearedEpsilonOffVsThetaPhi(Double_t* x, Double_t* par);
	Double_t dNdOmegaSmearedOffVsThetaPhi(Double_t* x, Double_t* par);
//	Double_t dNdOmegaSigma1EpsilonVsThetaPhi(Double_t* x, Double_t* par);
//	Double_t dNdOmegaSigma1OffEpsilonVsThetaPhi(Double_t* x, Double_t* par);
	Double_t dNdOmegaSigma1SmearedOffVsThetaPhi(Double_t* x, Double_t* par);
//	Double_t dNdOmegaOffEpsilonVsThetaPhi(Double_t* x, Double_t* par);
//
//	// ...ThetaVsThetaPhi
	Double_t dNdOmegaSmearedThetaVsThetaPhi(Double_t* x, Double_t* par);
//	Double_t dNdOmegaSmearedOffThetaVsThetaPhi(Double_t* x, Double_t* par);
	Double_t dNdOmegaEpsilonThetaVsThetaPhi(Double_t* x, Double_t* par);
	Double_t dNdOmegaSmearedEpsilonThetaVsThetaPhi(Double_t* x, Double_t* par);
	Double_t dNdOmegaSigma1SmearedEpsilonThetaVsThetaPhi(Double_t* x, Double_t* par);
	Double_t dNdOmegaSmearedEpsilonOffThetaVsThetaPhi(Double_t* x, Double_t* par);
	Double_t dNdOmegaSigma1SmearedEpsilonOffThetaVsThetaPhi(Double_t* x, Double_t* par);
//	Double_t dNdOmegaOffEpsilonThetaVsThetaPhi(Double_t* x, Double_t* par);
//	Double_t dNdOmegaSigma1SmearedThetaVsThetaPhi(Double_t* x, Double_t* par);
//	Double_t dNdOmegaSigma1EpsilonThetaVsThetaPhi(Double_t* x, Double_t* par);
//	Double_t dNdOmegaSigma1OffEpsilonThetaVsThetaPhi(Double_t* x, Double_t* par);
//	Double_t dNdOmegaSigma1SmearedOffThetaVsThetaPhi(Double_t* x, Double_t* par);
//
//	// Integrated...ThetaVsTheta
//	Double_t IntegratedNdOmegaEpsilonThetaVsTheta(Double_t* x, Double_t* par);
	Double_t IntegratedNdOmegaSmearedThetaVsTheta(Double_t* x, Double_t* par);
	Double_t IntegratedNdOmegaSmearedEpsilonThetaVsTheta(Double_t* x, Double_t* par);
	Double_t IntegratedNdOmegaSigma1SmearedEpsilonThetaVsTheta(Double_t* x, Double_t* par);
	Double_t IntegratedNdOmegaSmearedEpsilonOffThetaVsTheta(Double_t* x, Double_t* par);
	Double_t IntegratedNdOmegaSigma1SmearedEpsilonOffThetaVsTheta(Double_t* x, Double_t* par);
//	Double_t IntegratedNdOmegaSmearedOffThetaVsTheta(Double_t* x, Double_t* par);
//	Double_t IntegratedNdOmegaSigma1EpsilonThetaVsTheta(Double_t* x, Double_t* par);
//	Double_t IntegratedNdOmegaOffEpsilonThetaVsTheta(Double_t* x, Double_t* par);
//	Double_t IntegratedNdOmegaSigma1OffEpsilonThetaVsTheta(Double_t* x, Double_t* par);
//	Double_t IntegratedNdOmegaSigma1SmearedThetaVsTheta(Double_t* x, Double_t* par);
//	Double_t IntegratedNdOmegaSigma1SmearedOffThetaVsTheta(Double_t* x, Double_t* par);
//
//	void SetIsJFactorOnLessOff(Bool_t IsJFactorOnLessOff) 			{bIsJFactorOnLessOff=IsJFactorOnLessOff;}

private:

	TF1* fQ0FactorVsTheta;
	TF1* fQ1FactorVsTheta;
	TF1* fQ2FactorVsTheta;
	TF1* fQ3FactorVsTheta;
//
//	TF1* fQ12FactorVsTheta;
//	TF1* fQ13FactorVsTheta;
//	TF1* fQ23FactorVsTheta;
//	TF1* fQ123FactorVsTheta;
//	TF1* fQ124FactorVsTheta;
//	TF1* fQ134FactorVsTheta;
//	TF1* fQ234FactorVsTheta;
	TF1* fQ1234FactorVsTheta;
//
	TF2* fQ0FactorVsThetaWobble;
	TF2* fQ1FactorVsThetaWobble;
	TF2* fQ2FactorVsThetaWobble;
	TF2* fQ3FactorVsThetaWobble;
	TF2* fQ4FactorVsThetaWobble;
	TF2* fQ12FactorVsThetaWobble;
	TF2* fQ13FactorVsThetaWobble;
	TF2* fQ14FactorVsThetaWobble;
	TF2* fQ23FactorVsThetaWobble;
	TF2* fQ24FactorVsThetaWobble;
	TF2* fQ34FactorVsThetaWobble;
	TF2* fQ123FactorVsThetaWobble;
	TF2* fQ124FactorVsThetaWobble;
	TF2* fQ134FactorVsThetaWobble;
	TF2* fQ234FactorVsThetaWobble;
	TF2* fQ1234FactorVsThetaWobble;
//
//
//	// IntegratedNdOmega...
	TF1* fIntegratedNdOmegaEpsilonThetaVsTheta;
	TF1* fIntegratedNdOmegaSmearedThetaVsTheta;
	TF1* fIntegratedNdOmegaSmearedEpsilonThetaVsTheta;
	TF1* fIntegratedNdOmegaSigma1SmearedEpsilonThetaVsTheta;
	TF1* fIntegratedNdOmegaSmearedEpsilonOffThetaVsTheta;
	TF1* fIntegratedNdOmegaSigma1SmearedEpsilonOffThetaVsTheta;
//	TF1* fIntegratedNdOmegaSmearedOffThetaVsTheta;
//	TF1* fIntegratedNdOmegaSigma1EpsilonThetaVsTheta;
//	TF1* fIntegratedNdOmegaOffEpsilonThetaVsTheta;
//	TF1* fIntegratedNdOmegaSigma1OffEpsilonThetaVsTheta;
//	TF1* fIntegratedNdOmegaSigma1SmearedThetaVsTheta;
//	TF1* fIntegratedNdOmegaSigma1SmearedOffThetaVsTheta;
//
//	// ...VsTheta
	TF1* fdNdOmegaSmearedVsTheta;
//	TF1* fdNdOmegaSigma1SmearedVsTheta;
//
//	// ...VsThetaPhi
	TF2* fdNdOmegaSmearedOffVsThetaPhi;
	TF2* fdNdOmegaEpsilonVsThetaPhi;
	TF2* fdNdOmegaSmearedEpsilonVsThetaPhi;
	TF2* fdNdOmegaSigma1SmearedEpsilonVsThetaPhi;
	TF2* fdNdOmegaSmearedEpsilonOffVsThetaPhi;
	TF2* fdNdOmegaSigma1SmearedEpsilonOffVsThetaPhi;
//	TF2* fdNdOmegaOffEpsilonVsThetaPhi;
//	TF2* fdNdOmegaSigma1EpsilonVsThetaPhi;
//	TF2* fdNdOmegaSigma1OffEpsilonVsThetaPhi;
	TF2* fdNdOmegaSigma1SmearedOffVsThetaPhi;
//	//TF2* fdNdOmegaSigma1SmearedOffVsThetaPhi;
//
//	// ...ThetaVsThetaPhi
	TF2* fdNdOmegaSmearedThetaVsThetaPhi;
//	TF2* fdNdOmegaSmearedOffThetaVsThetaPhi;
	TF2* fdNdOmegaEpsilonThetaVsThetaPhi;
	TF2* fdNdOmegaSmearedEpsilonThetaVsThetaPhi;
	TF2* fdNdOmegaSigma1SmearedEpsilonThetaVsThetaPhi;
	TF2* fdNdOmegaSmearedEpsilonOffThetaVsThetaPhi;
	TF2* fdNdOmegaSigma1SmearedEpsilonOffThetaVsThetaPhi;
//	TF2* fdNdOmegaOffEpsilonThetaVsThetaPhi;
//	TF2* fdNdOmegaSigma1EpsilonThetaVsThetaPhi;
//	TF2* fdNdOmegaSigma1OffEpsilonThetaVsThetaPhi;
//	TF2* fdNdOmegaSigma1SmearedThetaVsThetaPhi;
//	TF2* fdNdOmegaSigma1SmearedOffThetaVsThetaPhi;
//
//
	TGraph* gdNdOmegaSmeared;
	TGraph* gdNdOmegaSigma1Smeared;
//
	Double_t dDeg2Rad;
	Double_t dBinResolution;
//
	Bool_t bIsJFactorOnLessOff;
	Bool_t bIsdNdOmegaSmeared;
	Bool_t bIsdNdOmegaSigma1Smeared;
//
	TH2D* th2QFactorVsThetaWobble;
};

#endif /* 	JDOptimitzation_H_ */

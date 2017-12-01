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


class JDOptimization {
public:
	JDOptimization(TString txtFile, TString myInstrumentPath, TString instrumentName, Double_t distCameraCenter, Double_t wobble);
	JDOptimization(TString author, TString source, TString candidate, TString mySourcePath, TString myInstrumentPath, TString instrumentName, Double_t distCameraCenter, Double_t wobble);
	virtual ~JDOptimization();

	void GetListOfQFactors();

	//	IDEAL: 									Q0 = J_on/theta
	//	LEAKAGE EFFECT: 						Q1 = J_on-J_off/theta
	//  UNCERTAINTY EFFECT: 					Q2 = J_1sm/theta
	// 	ACCEPTANCE EFFECT:						Q3 = J_eff/theta_eff
	//  LEAKAGE + UNCERTAINTY:  				Q12 = J_on_1sm-J_off_1sm/theta
	//  LEAKAGE + ACCEPTANCE:   				Q13 = J_on_eff-J_off_eff/theta_eff
	//  UNCERTAINTY + ACCEPTANCE:   			Q23 = J_1sm_eff/theta_eff
	//  LEAKAGE + UNCERTAINTY + ACCEPTANCE: 	Q123 = J_on_1sm_eff-J_off_1sm_eff/theta_eff

	TF1* GetTF1QFactorVsTheta(Int_t type=0, Double_t thetaNorm=0.4)
	{
		if(type==0)
		{
			fEvaluateQ0FactorVsTheta->SetParameter(0, thetaNorm);
			return fEvaluateQ0FactorVsTheta;
		}
		else if(type==1)
		{
			fEvaluateQ1FactorVsTheta->SetParameter(0, thetaNorm);
			return fEvaluateQ1FactorVsTheta;
		}

		else if(type==2)
		{
			fEvaluateQ2FactorVsTheta->SetParameter(0, thetaNorm);
			return fEvaluateQ2FactorVsTheta;
		}

		else if (type==3)
		{
			fEvaluateQ3FactorVsTheta->SetParameter(0, thetaNorm);
			return fEvaluateQ3FactorVsTheta;
		}

		else if (type==12 || type==21)
		{
			fEvaluateQ12FactorVsTheta->SetParameter(0, thetaNorm);
			return fEvaluateQ12FactorVsTheta;
		}

		else if (type==13 || type==31)
		{
			fEvaluateQ13FactorVsTheta->SetParameter(0, thetaNorm);
			return fEvaluateQ13FactorVsTheta;
		}

		else if (type==23 || type==32)
		{
			fEvaluateQ23FactorVsTheta->SetParameter(0, thetaNorm);
			return fEvaluateQ23FactorVsTheta;
		}

		else if (type==123 || type==132 || type==213 || type==231 || type==312 || type==321)
		{
			fEvaluateQ123FactorVsTheta->SetParameter(0, thetaNorm);
			return fEvaluateQ123FactorVsTheta;
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

	///////////////////////////////////////////////////////////////////////////////////////
	// ************************************************************************************
	//WARNING  THE GetOptimalThetaAndWobble() AND THE GetTH2QFactorVsThetaWobble() SHOULD BE ON THE SOURCE CODE
	// ************************************************************************************
	///////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////
	// ************************************************************************************
	//WARNING  THE TYPE IS THE ONLY VALUE AS AN INPUT
	// ************************************************************************************
	///////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////
	// This function gives a value and a range around this value of the theta optimal and the wobble optimal
	void GetOptimalThetaAndWobble(Double_t &thetaOpt, Double_t &thetaOptRangMin, Double_t &thetaOptRangMax, Double_t &wobbleOpt, Double_t &wobbleOptRangMin, Double_t &wobbleOptRangMax, Int_t type=0)
	{
		TH2D* h2 = GetTH2QFactorVsThetaWobble(type);
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

		Double_t tolerance = 0.30;

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

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// This TH2 is filled with the content of the TF2 corresponding to GetTF2QFactorvsThetaAndWobble
	TH2D* GetTH2QFactorVsThetaWobble(Int_t type=0, Double_t thetaNorm=0.4, Double_t wobbleNorm=0.4)
	{
		TF2* qFactor=GetTF2QFactorVsThetaWobble(type, thetaNorm, wobbleNorm);

		Double_t resolution = 0.05; 			//[deg/nºbin]
		Double_t thetaMax = GetThetaMax();		// [deg]
		Int_t numBinsX = thetaMax/resolution; 	// [nºbins]
		Double_t wobbleMax = GetDistCameraCenterMax();		// [deg]
		Int_t numBinsY = wobbleMax/resolution; 				// [nºbins]

		TH2D* h2 = new TH2D("h2","",numBinsX,0.,thetaMax,numBinsY,0.,wobbleMax);
		for(Int_t i=1; i<numBinsX+1; i++)
		{
			for(Int_t j=1; j<numBinsY+1; j++)
				{

				h2->SetBinContent(i,j,qFactor->Eval(h2->ProjectionX()->GetBinCenter(i),h2->ProjectionY()->GetBinCenter(j)));
				}
		}

		h2->Scale(1/(h2->GetMaximum()));
		return h2;
	}

	TF2* GetTF2QFactorVsThetaWobble(Int_t type=0, Double_t thetaNorm=0.4, Double_t wobbleNorm=0.4)
	{
		if(type==0)
		{
			fEvaluateQ0FactorVsThetaWobble->SetParameter(0, thetaNorm);
			fEvaluateQ0FactorVsThetaWobble->SetParameter(1, wobbleNorm);
			return fEvaluateQ0FactorVsThetaWobble;
		}
		else if(type==1)
		{
			fEvaluateQ1FactorVsThetaWobble->SetParameter(0, thetaNorm);
			fEvaluateQ1FactorVsThetaWobble->SetParameter(1, wobbleNorm);
			return fEvaluateQ1FactorVsThetaWobble;
		}

		else if(type==2)
		{
			fEvaluateQ2FactorVsThetaWobble->SetParameter(0, thetaNorm);
			fEvaluateQ2FactorVsThetaWobble->SetParameter(1, wobbleNorm);
			return fEvaluateQ2FactorVsThetaWobble;
		}

		else if(type==3)
		{
			fEvaluateQ3FactorVsThetaWobble->SetParameter(0, thetaNorm);
			fEvaluateQ3FactorVsThetaWobble->SetParameter(1, wobbleNorm);
			return fEvaluateQ3FactorVsThetaWobble;
		}

		else if(type==12 || type==21)
		{
			fEvaluateQ12FactorVsThetaWobble->SetParameter(0, thetaNorm);
			fEvaluateQ12FactorVsThetaWobble->SetParameter(1, wobbleNorm);
			return fEvaluateQ12FactorVsThetaWobble;
		}

		else if(type==13 || type==31)
		{
			fEvaluateQ13FactorVsThetaWobble->SetParameter(0, thetaNorm);
			fEvaluateQ13FactorVsThetaWobble->SetParameter(1, wobbleNorm);
			return fEvaluateQ13FactorVsThetaWobble;
		}

		else if(type==23 || type==32)
		{
			fEvaluateQ23FactorVsThetaWobble->SetParameter(0, thetaNorm);
			fEvaluateQ23FactorVsThetaWobble->SetParameter(1, wobbleNorm);
			return fEvaluateQ23FactorVsThetaWobble;
		}

		else if (type==123 || type==132 || type==213 || type==231 || type==312 || type==321)
		{
			fEvaluateQ123FactorVsThetaWobble->SetParameter(0, thetaNorm);
			fEvaluateQ123FactorVsThetaWobble->SetParameter(1, wobbleNorm);
			return fEvaluateQ123FactorVsThetaWobble;
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

	//***** JDDarkMatter Getters
	void GetWarning()						{return jdDarkMatter->GetWarning();}

	Bool_t GetIsBonnivard()					{return jdDarkMatter->GetIsBonnivard();}
	Bool_t GetIsGeringer()					{return jdDarkMatter->GetIsGeringer();}
	Bool_t GetIsJFactor()					{return jdDarkMatter->GetIsJFactor();}

	TString GetSourceName()					{return jdDarkMatter->GetSourceName();}
	TString GetCandidate()					{return jdDarkMatter->GetCandidate();}
	TString GetAuthor()						{return jdDarkMatter->GetAuthor();}
	TString GetSourcePath()					{return jdDarkMatter->GetSourcePath();}

	Double_t GetThetaMax()					{return jdDarkMatter->GetThetaMax();}
	Double_t GetJFactorMax()				{return jdDarkMatter->GetJFactorMax();}
	Double_t GetJFactor_m1Max()				{return jdDarkMatter->GetJFactor_m1Max();}
	Double_t GetJFactorMin()				{return jdDarkMatter->GetJFactorMin();}
	Double_t GetJFactor_m1Min()				{return jdDarkMatter->GetJFactor_m1Min();}


	TF1* GetTF1JFactorVsTheta()				{return jdDarkMatter->GetTF1JFactorVsTheta();}
	TF1* GetTF1JFactor_m1VsTheta()			{return jdDarkMatter->GetTF1JFactor_m1VsTheta();}
	TF1* GetTF1JFactorFromLOSVsTheta()		{return jdDarkMatter->GetTF1JFactorFromLOSVsTheta();}
	TF1* GetTF1JFactor_m1FromLOSVsTheta()	{return jdDarkMatter->GetTF1JFactor_m1FromLOSVsTheta();}
	TF1* GetTF1JFactorOffFromLOSVsTheta()	{return jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*jdInstrument->GetWobbleDistance());}
	TF1* GetTF1JFactor_m1OffFromLOSVsTheta(){return jdDarkMatter->GetTF1JFactor_m1OffFromLOSVsTheta(2*jdInstrument->GetWobbleDistance());}
	TF1* GetTF1LOSVsTheta()					{return jdDarkMatter->GetTF1LOSVsTheta();}
	TF1* GetTF1LOS_m1VsTheta()				{return jdDarkMatter->GetTF1LOS_m1VsTheta();}
	TF1* GetTF1NormLOSVsTheta()				{return jdDarkMatter->GetTF1NormLOSVsTheta(1.0);}
	TF1* GetTF1NormLOS_m1VsTheta()			{return jdDarkMatter->GetTF1NormLOS_m1VsTheta(1.0);}
	TF2* GetTF2LOSThetaVSThetaPhi()			{return jdDarkMatter->GetTF2LOSThetaVSThetaPhi();}
	TF2* GetTF2LOS_m1ThetaVSThetaPhi()		{return jdDarkMatter->GetTF2LOS_m1ThetaVSThetaPhi();}
	TF2* GetTF2LOSOffThetaVSThetaPhi()		{return jdDarkMatter->GetTF2LOSOffThetaVSThetaPhi(2*jdInstrument->GetWobbleDistance());}
	TF2* GetTF2LOS_m1OffThetaVSThetaPhi()	{return jdDarkMatter->GetTF2LOS_m1OffThetaVSThetaPhi(2*jdInstrument->GetWobbleDistance());}

	//***** JDDarkMatter Setters
	void SetCandidate(TString candidate)							{jdDarkMatter->SetCandidate(candidate);}

	//************************************************

	//***** JDInstrument Getters
	Bool_t GetIsIdeal()						{return jdInstrument->GetIsIdeal();}
	Bool_t GetIsJFactorOnLessOff() 			{return bIsJFactorOnLessOff;}

	TF1* GetTF1EpsilonVsDcc()				{return jdInstrument->GetTF1EpsilonVsDcc();}
	TF1* GetTF1EfficiencyVsTheta()			{return jdInstrument->GetTF1EfficiencyVsTheta(jdInstrument->GetWobbleDistance());}

	TF2* GetTF2EpsilonVsThetaAndPhi()		{return jdInstrument->GetTF2EpsilonVsThetaAndPhi();}
	TF2* GetTF2EpsilonThetaVsThetaAndPhi()	{return jdInstrument->GetTF2EpsilonThetaVsThetaAndPhi();}
	TF2* GetTF2EpsilonVsXAndY()				{return jdInstrument->GetTF2EpsilonVsXAndY();}

	TString GetInstrumentName()				{return jdInstrument->GetInstrumentName();}
	TString GetInstrumentPath()				{return jdInstrument->GetInstrumentPath();}

	Double_t GetDistCameraCenterMax()		{return jdInstrument->GetDistCameraCenterMax();}
	Double_t GetWobbleDistance()			{return jdInstrument->GetWobbleDistance();}



protected:

	// Composition C++ scheme
	JDDarkMatter* jdDarkMatter;
	JDInstrument* jdInstrument;

	void CreateFunctions();

	Double_t EvaluateQ0FactorVsTheta(Double_t* x, Double_t* par);
	Double_t EvaluateQ1FactorVsTheta(Double_t* x, Double_t* par);
	Double_t EvaluateQ2FactorVsTheta(Double_t* x, Double_t* par);
	Double_t EvaluateQ3FactorVsTheta(Double_t* x, Double_t* par);

	Double_t EvaluateQ12FactorVsTheta(Double_t* x, Double_t* par);
	Double_t EvaluateQ13FactorVsTheta(Double_t* x, Double_t* par);
	Double_t EvaluateQ23FactorVsTheta(Double_t* x, Double_t* par);
	Double_t EvaluateQ123FactorVsTheta(Double_t* x, Double_t* par);

	Double_t EvaluateQ0FactorVsThetaWobble(Double_t* x, Double_t* par);
	Double_t EvaluateQ1FactorVsThetaWobble(Double_t* x, Double_t* par);
	Double_t EvaluateQ2FactorVsThetaWobble(Double_t* x, Double_t* par);
	Double_t EvaluateQ3FactorVsThetaWobble(Double_t* x, Double_t* par);
	Double_t EvaluateQ12FactorVsThetaWobble(Double_t* x, Double_t* par);
	Double_t EvaluateQ13FactorVsThetaWobble(Double_t* x, Double_t* par);
	Double_t EvaluateQ23FactorVsThetaWobble(Double_t* x, Double_t* par);
	Double_t EvaluateQ123FactorVsThetaWobble(Double_t* x, Double_t* par);

	void SetIsJFactorOnLessOff(Bool_t IsJFactorOnLessOff) 			{bIsJFactorOnLessOff=IsJFactorOnLessOff;}

private:

	TF1* fEvaluateQ0FactorVsTheta;
	TF1* fEvaluateQ1FactorVsTheta;
	TF1* fEvaluateQ2FactorVsTheta;
	TF1* fEvaluateQ3FactorVsTheta;

	TF1* fEvaluateQ12FactorVsTheta;
	TF1* fEvaluateQ13FactorVsTheta;
	TF1* fEvaluateQ23FactorVsTheta;
	TF1* fEvaluateQ123FactorVsTheta;

	TF2* fEvaluateQ0FactorVsThetaWobble;
	TF2* fEvaluateQ1FactorVsThetaWobble;
	TF2* fEvaluateQ2FactorVsThetaWobble;
	TF2* fEvaluateQ3FactorVsThetaWobble;
	TF2* fEvaluateQ12FactorVsThetaWobble;
	TF2* fEvaluateQ13FactorVsThetaWobble;
	TF2* fEvaluateQ23FactorVsThetaWobble;
	TF2* fEvaluateQ123FactorVsThetaWobble;


	Double_t dDeg2Rad;

	Bool_t bIsJFactorOnLessOff;

};

#endif /* 	JDOptimitzation_H_ */

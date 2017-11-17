/*
 * Optimization.h
 *
 *  Created on: 03/07/2017
 *  Authors: David Navarro Gironés 	<<david.navarrogir@e-campus.uab.cat>>
 *  		 Joaquim Palacio 		<<jpalacio@ifae.es>>
 *
 *  		 ADD A GENERAL DESCRIPTION ON THE CLASS, THE MAIN FUNCTIONS, THE VARIABLES
 *  		 AND MENTION THE "runExample#.C" THAT SHOWS HOW TO USE IT
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



//	TF1* GetTF1EffectiveJFactorFromLOSVsTheta()
//	{
//		if(!GetIsJFactor()) GetWarning();
//		return fIntegrateEffectiveJFactorFromLOSVsTheta;
//	}


	TF1* GetTF1QFactorVsTheta(Int_t type=0, Double_t thetaNorm=0.4)
	{
		if(type==0)	// J_on/theta
		{
			fEvaluateQ0FactorVsTheta->SetParameter(0, thetaNorm);
			return fEvaluateQ0FactorVsTheta;
		}
		else if(type==1)	// J_on/Sqrt{theta^2+J_off}
		{
			fEvaluateQ1FactorVsTheta->SetParameter(0, thetaNorm);
			return fEvaluateQ1FactorVsTheta;
		}

		else if(type==2)	//J_1sm/theta
		{
			fEvaluateQ2FactorVsTheta->SetParameter(0, thetaNorm);
			return fEvaluateQ2FactorVsTheta;
		}

		else if (type==3)	//J_eff/theta_eff
		{
			fEvaluateQ3FactorVsTheta->SetParameter(0, thetaNorm);
			return fEvaluateQ3FactorVsTheta;
		}

		else if (type==12 || type==21)	//J_on_1sm/Sqrt{theta^2 + J_off_1sm}
		{
			fEvaluateQ12FactorVsTheta->SetParameter(0, thetaNorm);
			return fEvaluateQ12FactorVsTheta;
		}

		else if (type==13 || type==31)	//J_on_eff/Sqrt{(theta_eff)^2 + J_off_eff}
		{
			fEvaluateQ13FactorVsTheta->SetParameter(0, thetaNorm);
			return fEvaluateQ13FactorVsTheta;
		}

		else if (type==23 || type==32)	//J_1sm_eff/theta_eff
		{
			fEvaluateQ23FactorVsTheta->SetParameter(0, thetaNorm);
			return fEvaluateQ23FactorVsTheta;
		}

		else if (type==123 || type==132 || type==213 || type==231 || type==312 || type==321)	//J_on_1sm_eff/Sqrt{(theta_eff)^2 + J_off_1sm_eff}
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

//	TH2D* GetTH2DQFactorVsThetaWobble()
//	void* GetOptimalThetaAndWobble(Double_t &theta, Double_t &wobble)

	///////////////////////////////////////////////////////////////////////////////////////
	// ************************************************************************************
	//WARNING  THE TYPE IS THE ONLY VALUE AS AN INPUT
	// ************************************************************************************
	///////////////////////////////////////////////////////////////////////////////////////
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
//			cout << i << " " << numBinsX << endl;
			for(Int_t j=1; j<numBinsY+1; j++)
				{
//					cout << j << " " << numBinsY << endl;
//				cout<<h2->ProjectionX()->GetBinCenter(i)<<" ";
//				cout<<h2->ProjectionY()->GetBinCenter(j)<<endl;

				h2->SetBinContent(i,j,qFactor->Eval(h2->ProjectionX()->GetBinCenter(i),h2->ProjectionY()->GetBinCenter(j)));
				}
		}

		h2->Scale(1/(h2->GetMaximum()));
		return h2;
	}

	TF2* GetTF2QFactorVsThetaWobble(Int_t type=0, Double_t thetaNorm=0.4, Double_t wobbleNorm=0.4)
	{
		if(type==0)	// J_on/theta
		{
			fEvaluateQ0FactorVsThetaWobble->SetParameter(0, thetaNorm);
			fEvaluateQ0FactorVsThetaWobble->SetParameter(1, wobbleNorm);
			return fEvaluateQ0FactorVsThetaWobble;
		}
		else if(type==1)	// J_on/theta
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


	// POSAR AQUESTS GETTERS COM ELS GETTERS QUE HI HA ABAIX, EN PRINCIPI LA FUNCIO OPTIMIZATION NOMÉS HAURIA DE DONAR LES Q FACTOR

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
	Double_t GetJFactorMin()				{return jdDarkMatter->GetJFactorMin();}


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
	TF2* GetTF2LOSOffThetaVSThetaPhi()		{return jdDarkMatter->GetTF2LOSOffThetaVSThetaPhi();}
	TF2* GetTF2LOS_m1OffThetaVSThetaPhi()	{return jdDarkMatter->GetTF2LOS_m1OffThetaVSThetaPhi();}

	//***** JDDarkMatter Setters
	void SetCandidate(TString candidate)							{jdDarkMatter->SetCandidate(candidate);}

	//************************************************

	//***** JDInstrument Getters
	Bool_t GetIsIdeal()						{return jdInstrument->GetIsIdeal();}
	Bool_t GetIsJFactorOnLessOff() 			{return bIsJFactorOnLessOff;}

	TF1* GetTF1EpsilonVsTheta()				{return jdInstrument->GetTF1EpsilonVsTheta();}
	TF1* GetTF1EfficiencyVsTheta()			{return jdInstrument->GetTF1EfficiencyVsTheta(jdInstrument->GetWobbleDistance());}

	TF2* GetTF2EpsilonVsThetaAndPhi()		{return jdInstrument->GetTF2EpsilonVsThetaAndPhi();}
	TF2* GetTF2EpsilonThetaVsThetaAndPhi()	{return jdInstrument->GetTF2EpsilonThetaVsThetaAndPhi();}
	TF2* GetTF2EpsilonVsXAndY()				{return jdInstrument->GetTF2EpsilonVsXAndY();}

	TString GetInstrumentName()				{return jdInstrument->GetInstrumentName();}
	TString GetInstrumentPath()				{return jdInstrument->GetInstrumentPath();}

	Double_t GetDistCameraCenterMax()		{return jdInstrument->GetDistCameraCenterMax();}
	Double_t GetWobbleDistance()			{return jdInstrument->GetWobbleDistance();}
	//***** JDInstrument Setters



protected:

	// Composition C++ scheme
	JDDarkMatter* jdDarkMatter;
	JDInstrument* jdInstrument;

	void CreateFunctions();

	Double_t EvaluateQ0FactorVsTheta(Double_t* x, Double_t* par);			// J_on/theta
	Double_t EvaluateQ1FactorVsTheta(Double_t* x, Double_t* par);			// J_on/Sqrt{theta^2+J_off}
	Double_t EvaluateQ2FactorVsTheta(Double_t* x, Double_t* par);			// J_1sm/theta
	Double_t EvaluateQ3FactorVsTheta(Double_t* x, Double_t* par);			// J_eff/theta_eff

	Double_t EvaluateQ12FactorVsTheta(Double_t* x, Double_t* par);			// J_eff/theta_eff
	Double_t EvaluateQ13FactorVsTheta(Double_t* x, Double_t* par);			// J_eff/theta_eff
	Double_t EvaluateQ23FactorVsTheta(Double_t* x, Double_t* par);			// J_eff/theta_eff
	Double_t EvaluateQ123FactorVsTheta(Double_t* x, Double_t* par);			// J_eff/theta_eff

	Double_t EvaluateQ0FactorVsThetaWobble(Double_t* x, Double_t* par);		// J_on/theta
	Double_t EvaluateQ1FactorVsThetaWobble(Double_t* x, Double_t* par);		// J_on/Sqrt{theta^2+J_off}
	Double_t EvaluateQ2FactorVsThetaWobble(Double_t* x, Double_t* par);		// J_eff/theta_eff
	Double_t EvaluateQ3FactorVsThetaWobble(Double_t* x, Double_t* par);		// J_eff/theta_eff
	Double_t EvaluateQ12FactorVsThetaWobble(Double_t* x, Double_t* par);		// J_eff/theta_eff
	Double_t EvaluateQ13FactorVsThetaWobble(Double_t* x, Double_t* par);		// J_eff/theta_eff
	Double_t EvaluateQ23FactorVsThetaWobble(Double_t* x, Double_t* par);		// J_eff/theta_eff
	Double_t EvaluateQ123FactorVsThetaWobble(Double_t* x, Double_t* par);		// J_eff/theta_eff

	void SetIsJFactorOnLessOff(Bool_t IsJFactorOnLessOff) 			{bIsJFactorOnLessOff=IsJFactorOnLessOff;}

private:

	TF1* fEvaluateQ0FactorVsTheta;							// J_on/theta
	TF1* fEvaluateQ1FactorVsTheta;							// J_on/Sqrt{theta^2+J_off}
	TF1* fEvaluateQ2FactorVsTheta;							// J_1sm/theta
	TF1* fEvaluateQ3FactorVsTheta;							// J_eff/theta_eff

	TF1* fEvaluateQ12FactorVsTheta;							// J_on_1sm/Sqrt{theta^2 + J_off_1sm}
	TF1* fEvaluateQ13FactorVsTheta;							// J_on_eff/Sqrt{(theta_eff)^2 + J_off_eff}
	TF1* fEvaluateQ23FactorVsTheta;							// J_1sm_eff/theta_eff
	TF1* fEvaluateQ123FactorVsTheta;						// J_on_1sm_eff/Sqrt{(theta_eff)^2 + J_off_1sm_eff}

	TF2* fEvaluateQ0FactorVsThetaWobble;					// J_on/theta
	TF2* fEvaluateQ1FactorVsThetaWobble;					// J_on/Sqrt{theta^2+J_off}
	TF2* fEvaluateQ2FactorVsThetaWobble;					// J_1sm/theta
	TF2* fEvaluateQ3FactorVsThetaWobble;					// J_eff/theta_eff
	TF2* fEvaluateQ12FactorVsThetaWobble;					// J_on_1sm/Sqrt{theta^2 + J_off_1sm}
	TF2* fEvaluateQ13FactorVsThetaWobble;					// J_on_eff/Sqrt{(theta_eff)^2 + J_off_eff}
	TF2* fEvaluateQ23FactorVsThetaWobble;					// J_1sm_eff/theta_eff
	TF2* fEvaluateQ123FactorVsThetaWobble;					// J_on_1sm_eff/Sqrt{(theta_eff)^2 + J_off_1sm_eff}


	Double_t dDeg2Rad;

	Bool_t bIsJFactorOnLessOff;

};

#endif /* 	JDOptimitzation_H_ */

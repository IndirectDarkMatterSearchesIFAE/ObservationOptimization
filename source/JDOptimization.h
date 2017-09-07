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


//class JDOptimization: public JDDarkMatter, public JDInstrument {
class JDOptimization {
public:
	JDOptimization(TString txtFile, TString instrumentName, Double_t distCameraCenter, Double_t wobble);
	JDOptimization(TString author, TString source, TString candidate, TString mySourcePath, TString instrumentName, Double_t distCameraCenter, Double_t wobble);
	virtual ~JDOptimization();

	void GetListOfQFactors();


//	TF1* GetTF1EffectiveJFactorFromLOSVsTheta()
//	{
//		if(!GetIsJFactor()) GetWarning();
//		return fIntegrateEffectiveJFactorFromLOSVsTheta;
//	}


	TF1* GetTF1QFactorVsTheta(Int_t type=0, Double_t thetaNorm=1)
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

	TF1* GetTF1QFactor0VsTheta(Double_t thetaNorm=0.1)	// J_on/theta
	{
		fEvaluateQ0FactorVsTheta->SetParameter(0, thetaNorm);
		return fEvaluateQ0FactorVsTheta;
	}

	TF1* GetTF1QFactor1VsTheta(Double_t thetaNorm=0.1)	// J_on/Sqrt{theta^2+J_off}
	{
		fEvaluateQ1FactorVsTheta->SetParameter(0, thetaNorm);
		return fEvaluateQ1FactorVsTheta;
	}

	void GetOptimalThetaAndWobble(Double_t &thetaOpt, Double_t &wobbleOpt)
	{
		TH2D* h2 = GetTH2QFactorVsThetaWobble();
		Int_t numBinsX = h2->GetNbinsX();
		Int_t numBinsY = h2->GetNbinsY();

		Double_t qfactorMax=0;

		for(Int_t i=1; i<numBinsX+1; i++)
		{
			for(Int_t j=1; j<numBinsY+1; j++)
				{
					Double_t qfactor = h2->GetBinContent(i,j);
					if(qfactor>qfactorMax)
					{
						qfactorMax=qfactor;
						thetaOpt=h2->ProjectionX()->GetBinCenter(i);
						wobbleOpt=h2->ProjectionY()->GetBinCenter(j);
					}
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

	TH2D* GetTH2QFactorVsThetaWobble(Int_t type=0, Double_t thetaNorm=1, Double_t wobbleNorm=0.4)
	{
		TF2* qFactor=GetTF2QFactorVsThetaWobble(type, thetaNorm, wobbleNorm);
		Double_t resolution = 0.1; 			//[deg/nºbin]
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
					h2->SetBinContent(i,j,qFactor->Eval(h2->ProjectionX()->GetBinCenter(i),h2->ProjectionY()->GetBinCenter(j)));
				}
		}

		return h2;
	}


	TF2* GetTF2QFactorVsThetaWobble(Int_t type=0, Double_t thetaNorm=1, Double_t wobbleNorm=0.4)
	{
		if(type==0)	// J_on/theta
		{
			fEvaluateQ0FactorVsThetaWobble->SetParameter(0, thetaNorm);
			fEvaluateQ0FactorVsThetaWobble->SetParameter(1, wobbleNorm);
			return fEvaluateQ0FactorVsThetaWobble;
		}
		if(type==1)	// J_on/theta
		{
			fEvaluateQ1FactorVsThetaWobble->SetParameter(0, thetaNorm);
			fEvaluateQ1FactorVsThetaWobble->SetParameter(1, wobbleNorm);
			return fEvaluateQ1FactorVsThetaWobble;
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
	TF1* GetTF1JFactorFromLOS_OnVsTheta()
	{
		return fEvaluateJFactorFromLOS_OnVsTheta;
	}

	TF1* GetTF1JFactorFromLOS_OffVsTheta()
	{
		return fEvaluateJFactorFromLOS_OffVsTheta;
	}

	TF1* GetTF1JFactorFromLOS_TotalVsTheta()
	{
		return fEvaluateJFactorFromLOS_TotalVsTheta;
	}

	TF1* GetTF1QFactorFromLOS_TotalVsTheta(Double_t thetaNorm)
	{
		fEvaluateQFactorFromLOS_TotalVsTheta->SetParameter(0, thetaNorm);
		return fEvaluateQFactorFromLOS_TotalVsTheta;
	}

	TF1* GetTF1JFactorEffectiveVsTheta()
	{
		return fEvaluateJFactorEffectiveVsTheta;
	}

	TF1* GetTF1QFactorEffectiveVsTheta(Double_t thetaNorm)
	{
		fEvaluateQFactorEffectiveVsTheta->SetParameter(0, thetaNorm);
		return fEvaluateQFactorEffectiveVsTheta;
	}
	/**************************************************/

	//***** JDDarkMatter Getters
	void GetWarning()						{return jdDarkMatter->GetWarning();}

	Bool_t GetIsBonnivard()					{return jdDarkMatter->GetIsBonnivard();}
	Bool_t GetIsGeringer()					{return jdDarkMatter->GetIsGeringer();}
	Bool_t GetIsJFactor()					{return jdDarkMatter->GetIsJFactor();}

	TString GetSourceName()					{return jdDarkMatter->GetSourceName();}
	TString GetCandidate()					{return jdDarkMatter->GetCandidate();}

	Double_t GetThetaMax()					{return jdDarkMatter->GetThetaMax();}
	Double_t GetJFactorMax()				{return jdDarkMatter->GetJFactorMax();}
	Double_t GetJFactorMin()				{return jdDarkMatter->GetJFactorMin();}


	TF1* GetTF1JFactorFromLOSVsTheta()
	{
		return jdDarkMatter->GetTF1JFactorFromLOSVsTheta();
	}

	TF1* GetTF1JFactorOffFromLOSVsTheta(){return jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*jdInstrument->GetWobbleDistance());}


	TF1* GetJFactorVsTheta(Int_t type=0)
	{
		if(type==0)return jdDarkMatter->GetTF1JFactorVsTheta();
		else return jdDarkMatter->GetTF1JFactorFromLOSVsTheta();
	}

	//***** JDDarkMatter Setters
	void SetCandidate(TString candidate)		{jdDarkMatter->SetCandidate(candidate);}


	//************************************************

	//***** JDInstrument Getters
	Bool_t GetIsIdeal()							{return jdInstrument->GetIsIdeal();}

	Double_t GetDistCameraCenterMax()			{return jdInstrument->GetDistCameraCenterMax();}
	Double_t GetWobbleDistance()				{return jdInstrument->GetWobbleDistance();}
	//***** JDInstrument Setters



protected:

	// Composition C++ scheme
	JDDarkMatter* jdDarkMatter;
	JDInstrument* jdInstrument;

	void CreateFunctions();

	Double_t EvaluateQ0FactorVsTheta(Double_t* x, Double_t* par);			// J_on/theta
	Double_t EvaluateQ1FactorVsTheta(Double_t* x, Double_t* par);			// J_on/Sqrt{theta^2+J_off}
	Double_t EvaluateQ2FactorVsTheta(Double_t* x, Double_t* par);			// J_eff/theta_eff

	Double_t EvaluateQ0FactorVsThetaWobble(Double_t* x, Double_t* par);		// J_on/theta
	Double_t EvaluateQ1FactorVsThetaWobble(Double_t* x, Double_t* par);		// J_on/Sqrt{theta^2+J_off}
	Double_t EvaluateQ2FactorVsThetaWobble(Double_t* x, Double_t* par);		// J_eff/theta_eff

	Double_t EvaluateLOSPerSinusThetaVsDcg(Double_t* x, Double_t* par);
	Double_t EvaluateJFactorFromLOS_OnVsTheta(Double_t* x, Double_t* par);
	Double_t EvaluateJFactorFromLOS_OffVsTheta(Double_t* x, Double_t* par);
	Double_t EvaluateJFactorFromLOS_TotalVsTheta(Double_t* x, Double_t* par);
	Double_t EvaluateQFactorFromLOS_TotalVsTheta (Double_t* x, Double_t* par);

	Double_t EvaluateJFactorEffectiveVsTheta(Double_t* x, Double_t* par);
	Double_t EvaluateQFactorEffectiveVsTheta(Double_t* x, Double_t* par);

private:

	TF1* fEvaluateQ0FactorVsTheta;						// J_on/theta
	TF1* fEvaluateQ1FactorVsTheta;						// J_on/Sqrt{theta^2+J_off}
	TF1* fEvaluateQ2FactorVsTheta;						// J_eff/theta_eff

	TF1* fEvaluateJFactorFromLOS_OnVsTheta;
	TF1* fEvaluateJFactorFromLOS_OffVsTheta;
	TF1* fEvaluateJFactorFromLOS_TotalVsTheta;
	TF1* fEvaluateQFactorFromLOS_TotalVsTheta;
	TF1* fEvaluateJFactorEffectiveVsTheta;
	TF1* fEvaluateQFactorEffectiveVsTheta;

	TF2* fEvaluateLOSPerSinusThetaVsDcg;

	TF2* fEvaluateQ0FactorVsThetaWobble;				// J_on/theta
	TF2* fEvaluateQ1FactorVsThetaWobble;				// J_on/Sqrt{theta^2+J_off}
	TF2* fEvaluateQ2FactorVsThetaWobble;				// J_eff/theta_eff


	Double_t dDeg2Rad;
};

#endif /* 	JDOptimitzation_H_ */

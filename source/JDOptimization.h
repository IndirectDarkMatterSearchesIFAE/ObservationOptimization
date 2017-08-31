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
	JDOptimization(TString author, TString source, TString candidate, TString mySourcePath, TString instrumentName, Double_t distCameraCenter, Double_t wobble);
	virtual ~JDOptimization();

	TF1* GetTF1QFactorVsTheta(Double_t thetaNorm=1, Int_t type=0)
	{
		if(type==0)	// J_on/theta
		{
			fEvaluateQ0FactorVsTheta->SetParameter(0, thetaNorm);
			return fEvaluateQ0FactorVsTheta;
		}
		if(type==1)	// J_on/Sqrt{theta^2+J_off}
		{
			fEvaluateQ1FactorVsTheta->SetParameter(0, thetaNorm);
			return fEvaluateQ1FactorVsTheta;
		}
		else
		{
			// (QUIM) To be corrected
			fEvaluateQ0FactorVsTheta->SetParameter(0, thetaNorm);
			return fEvaluateQ0FactorVsTheta;
		}
	}

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

	//***** JDDarkMatter Getters
	Bool_t GetIsBonnivard()			{return jdDarkMatter->GetIsBonnivard();}
	Bool_t GetIsGeringer()			{return jdDarkMatter->GetIsGeringer();}
	Bool_t GetIsJFactor()			{return jdDarkMatter->GetIsJFactor();}

	TString GetSourceName()			{return jdDarkMatter->GetSourceName();}
	TString GetCandidate()			{return jdDarkMatter->GetCandidate();}

	Double_t GetThetaMax()			{return jdDarkMatter->GetThetaMax();}
	Double_t GetJFactorMax()		{return jdDarkMatter->GetJFactorMax();}
	Double_t GetJFactorMin()		{return jdDarkMatter->GetJFactorMin();}

	TF1* GetJFactorVsTheta()		{return jdDarkMatter->GetTF1JFactorVsTheta();}

	//***** JDDarkMatter Getters
	Bool_t GetIsIdeal()			{return jdInstrument->GetIsIdeal();}



protected:

	// Composition C++ scheme
	JDDarkMatter* jdDarkMatter;
	JDInstrument* jdInstrument;

	void CreateFunctions();

	Double_t EvaluateQ0FactorVsTheta(Double_t* x, Double_t* par);	// J_on/theta
	Double_t EvaluateQ1FactorVsTheta(Double_t* x, Double_t* par);	// J_on/Sqrt{theta^2+J_off}


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
	TF1* fEvaluateJFactorFromLOS_OnVsTheta;
	TF1* fEvaluateJFactorFromLOS_OffVsTheta;
	TF1* fEvaluateJFactorFromLOS_TotalVsTheta;
	TF1* fEvaluateQFactorFromLOS_TotalVsTheta;
	TF1* fEvaluateJFactorEffectiveVsTheta;
	TF1* fEvaluateQFactorEffectiveVsTheta;

	TF2* fEvaluateLOSPerSinusThetaVsDcg;

};

#endif /* 	JDOptimitzation_H_ */

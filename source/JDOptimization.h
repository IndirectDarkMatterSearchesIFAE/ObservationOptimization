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


class JDOptimization: public JDDarkMatter, public JDInstrument {
public:
	JDOptimization(TString author, TString source, TString candidate, TString mySourcePath, TString instrumentName, Double_t wobble, TString myInstrumentPath);
	virtual ~JDOptimization();

	TF1* GetTF1QFactorVsTheta(Double_t thetaNorm)
	{
		fEvaluateQFactorVsTheta->SetParameter(0, thetaNorm);
		return fEvaluateQFactorVsTheta;
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

protected:

	void CreateFunctions();

	Double_t EvaluateQFactorVsTheta(Double_t* x, Double_t* par);

	Double_t EvaluateLOSPerSinusThetaVsDcg(Double_t* x, Double_t* par);
	Double_t EvaluateJFactorFromLOS_OnVsTheta(Double_t* x, Double_t* par);
	Double_t EvaluateJFactorFromLOS_OffVsTheta(Double_t* x, Double_t* par);
	Double_t EvaluateJFactorFromLOS_TotalVsTheta(Double_t* x, Double_t* par);
	Double_t EvaluateQFactorFromLOS_TotalVsTheta (Double_t* x, Double_t* par);

	Double_t EvaluateJFactorEffectiveVsTheta(Double_t* x, Double_t* par);
	Double_t EvaluateQFactorEffectiveVsTheta(Double_t* x, Double_t* par);

private:

	TF1* fEvaluateQFactorVsTheta;
	TF1* fEvaluateJFactorFromLOS_OnVsTheta;
	TF1* fEvaluateJFactorFromLOS_OffVsTheta;
	TF1* fEvaluateJFactorFromLOS_TotalVsTheta;
	TF1* fEvaluateQFactorFromLOS_TotalVsTheta;
	TF1* fEvaluateJFactorEffectiveVsTheta;
	TF1* fEvaluateQFactorEffectiveVsTheta;

	TF2* fEvaluateLOSPerSinusThetaVsDcg;

};

#endif /* 	JDOptimitzation_H_ */

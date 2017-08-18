/*
 * Optimization.h
 *
 *  Created on: 03/07/2017
 *      Author: david
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

	TF1* GetTF1JFactorFromLOS_On()
	{
		return fEvaluateJFactorFromLOS_OnVsTheta;
	}

	TF1* GetTF1JFactorFromLOS_Off()
	{
		return fEvaluateJFactorFromLOS_OffVsTheta;
	}

	TF1* GetTF1JFactorFromLOS_Total()
	{
		return fEvaluateJFactorFromLOS_TotalVsTheta;
	}

	TF1* GetTF1QFactorFromLOS_Total(Double_t thetaNorm)
	{
		fEvaluateQFactorFromLOS_TotalVsTheta->SetParameter(0, thetaNorm);
		return fEvaluateQFactorFromLOS_TotalVsTheta;
	}

	TF1* GetTF1JFactorEffectiveVsTheta()
	{
		return fEvaluateJFactorEffectiveVsTheta;
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

private:

	TF1* fEvaluateQFactorVsTheta;
	TF1* fEvaluateJFactorFromLOS_OnVsTheta;
	TF1* fEvaluateJFactorFromLOS_OffVsTheta;
	TF1* fEvaluateJFactorFromLOS_TotalVsTheta;
	TF1* fEvaluateQFactorFromLOS_TotalVsTheta;
	TF1* fEvaluateJFactorEffectiveVsTheta;

	TF2* fEvaluateLOSPerSinusThetaVsDcg;

};

#endif /* 	JDOptimitzation_H_ */

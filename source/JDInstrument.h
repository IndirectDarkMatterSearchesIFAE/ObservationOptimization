/*
 * Instrument.h
 *
 *  Created on: 03/07/2017
 *  Authors: David Navarro Gironés 	<<david.navarrogir@e-campus.uab.cat>>
 *  		 Joaquim Palacio 		<<jpalacio@ifae.es>>
 *
 *  		 ADD A GENERAL DESCRIPTION ON THE CLASS, THE MAIN FUNCTIONS, THE VARIABLES
 *  		 AND MENTION THE "runExample#.C" THAT SHOWS HOW TO USE IT
 */

#ifndef INSTRUMENT_H_
#define INSTRUMENT_H_

#include <TF1.h>
#include <TF2.h>
#include <TH2.h>



class JDInstrument {
public:
	JDInstrument(TString instrumentName, Double_t wobble, TString myInstrumentPath);
	virtual ~JDInstrument();

TF2* GetTF2EpsilonVsThetaAndPhi()
{
	return fEvaluateEpsilonVsThetaAndPhi;
}

TF1* GetTF1EfficiencyVsTheta()
{
	return fEvaluateEfficiencyVsTheta;
}

Double_t GetDcc()
{
	return dDcc;
}

Double_t GetThetaMax()
{
	return dThetaMax;
}

Double_t GetWobble()
{
	return dWobble;
}

TString GetInstrumentName()
{
	return sInstrumentName;
}

protected:

	void CreateFunctionsInstrument();
	void SetEpsilon();

Double_t EvaluateEpsilonVsThetaAndPhi(Double_t* x, Double_t* par);
Double_t EvaluateEpsilonPerThetaVsThetaAndPhi(Double_t* x, Double_t* par);
Double_t EvaluateEfficiencyVsTheta(Double_t* x, Double_t* par);



private:

///////////////////////////////////////////////////////
//TString
///////////////////////////////////////////////////////

TString sInstrumentName;
TString sMyInstrumentPath;

///////////////////////////////////////////////////////
//Double_t
///////////////////////////////////////////////////////

Double_t dDcc;
Double_t dWobble;
Double_t dThetaMax;

///////////////////////////////////////////////////////
//TGraph
///////////////////////////////////////////////////////

TGraph* gEpsilon;

///////////////////////////////////////////////////////
//TF1
///////////////////////////////////////////////////////

TF1* fEvaluateEfficiencyVsTheta;

///////////////////////////////////////////////////////
//TF2
///////////////////////////////////////////////////////

TF2* fEvaluateEpsilonVsThetaAndPhi;
TF2* fEvaluateEpsilonPerThetaVsThetaAndPhi;
};


#endif /* INSTRUMENT_H_ */

/*
 * Instrument.h
 *
 *  Created on: 03/07/2017
 *      Author: david
 */

#ifndef INSTRUMENT_H_
#define INSTRUMENT_H_

#include <TF1.h>
#include <TF2.h>
#include <TH2.h>



class JDInstrument {
public:
	JDInstrument(TString instrumentName, Double_t wobble);
	virtual ~JDInstrument();

TF2* GetEpsilon()
{
	return fEvaluateEpsilonVsTheta;
}

TF1* GetEfficiency()
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

protected:

	void CreateFunctionsInstrument();
	void SetEpsilon();

Double_t EvaluateEpsilonVsTheta(Double_t* x, Double_t* par);
Double_t EvaluateEpsilonPerThetaVsTheta(Double_t* x, Double_t* par);
Double_t EvaluateEfficiencyVsTheta(Double_t* x, Double_t* par);



private:

///////////////////////////////////////////////////////
//TString
///////////////////////////////////////////////////////

TString sInstrumentName;

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

TF2* fEvaluateEpsilonVsTheta;
TF2* fEvaluateEpsilonPerThetaVsTheta;
};


#endif /* INSTRUMENT_H_ */
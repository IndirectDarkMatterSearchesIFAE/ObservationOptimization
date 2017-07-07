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



class Instrument {
public:
	Instrument(TString InstrumentName, Double_t Wobble);
	virtual ~Instrument();

TF2* GetEpsilon()
{
	return fEpsilon;
}

TF1* GetEfficiency()
{
	return fEfficiency;
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

Double_t dEfficiency(Double_t* x, Double_t* par);
Double_t dEpsilon(Double_t* x, Double_t* par);
Double_t dEpsilonPerTheta(Double_t* x, Double_t* par);



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

TF1* fEfficiency;

///////////////////////////////////////////////////////
//TF2
///////////////////////////////////////////////////////

TF2* fEpsilon;
TF2* fEpsilonPerTheta;
};


#endif /* INSTRUMENT_H_ */

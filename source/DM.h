/*
 * DM.h
 *
 *  Created on: 03/07/2017
 *      Author: david
 */

#ifndef DM_H_
#define DM_H_

#include <TF1.h>

class DM {
public:
	DM();
	virtual ~DM();


TF1* GetJFactor()
{
	return fJFactor;
}

TF1* GetQFactor()
{
	return fQFactor;
}

TF1* GetLOS()
{
	return fLOS;
}

TF1* GetJFactorFromLOS()
{
	return fJFactorFromLOS;
}

protected:

void CreateFunctionsDM();
void SetJFactor();

Double_t dJFactor(Double_t* x, Double_t* par);
Double_t dQFactor(Double_t* x, Double_t* par);
Double_t dLOS(Double_t* x, Double_t* par);
Double_t dLOSPerSin(Double_t* x, Double_t* par);
Double_t dJFactorFromLOS(Double_t* x, Double_t* par);



private:

///////////////////////////////////////////////////////
//TString
///////////////////////////////////////////////////////

TString sCandidate;
TString sSource;

///////////////////////////////////////////////////////
//Double_t
///////////////////////////////////////////////////////

Double_t dTheta;


///////////////////////////////////////////////////////
//TGraph
///////////////////////////////////////////////////////

TGraph* gJFactor;

///////////////////////////////////////////////////////
//TF1
///////////////////////////////////////////////////////
TF1* fJFactor;
TF1* fQFactor;
TF1* fLOS;
TF1* fLOSPerSin;
TF1* fJFactorFromLOS;


};



#endif /* DM_H_ */

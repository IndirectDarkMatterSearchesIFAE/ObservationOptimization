/*
 * DM.h
 *
 *  Created on: 03/07/2017
 *      Author: david
 */

#ifndef DM_H_
#define DM_H_

#include <TF1.h>
using namespace std;

class DM {
public:

	DM(TString Candidate, TString Source, TString Form);
		virtual ~DM();

TF1* GetJFactor()
{
	return fJFactor;
}

TF1* GetQFactor()
{
	return fQFactor;
}

void SetCandidate(TString Candidate)
{
	 sCandidate=Candidate;
}

void SetSource(TString Source)
{
	sSource = Source;
}

void SetForm(TString Form)
{
	sForm = Form;
}

TString GetCandidate()
{
	return sCandidate;
}

TString GetSource()
{
	return sSource;
}

TString GetForm()
{
	return sForm;
}


protected:

void CreateFunctionsDM();
void SetJFactor();

Double_t dJFactor(Double_t* x, Double_t* par);
Double_t dQFactor(Double_t* x, Double_t* par);


private:

///////////////////////////////////////////////////////
//TString
///////////////////////////////////////////////////////

TString sCandidate;
TString sSource;
TString sForm;

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


};



#endif /* DM_H_ */

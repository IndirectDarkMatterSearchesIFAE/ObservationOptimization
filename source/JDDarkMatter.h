/*
 * DM.h
 *
 *  Created on: 03/07/2017
 *      Author: david
 */

#ifndef JDDarkMatter_H_
#define JDDarkMatter_H_

#include <TF1.h>
using namespace std;

class JDDarkMatter {
public:

	JDDarkMatter(TString author, TString source, TString candidate);
		virtual ~JDDarkMatter();

TF1* GetTF1JFactorVsTheta()
{
	return fEvaluateJFactorVsTheta;
}

// x[0] == normalization point [deg]
//TF1* GetQFactor(Double_t thetaNorm)
TF1* GetTF1QFactorVsTheta(Double_t thetaNorm)
{
	fEvaluateQFactorVsTheta->SetParameter(0, thetaNorm);
	return fEvaluateQFactorVsTheta;
}

Double_t GetTheta()
{
	return dTheta;
}



//GetJFactorName
//GetAuthorName
//GetIsAnnihilation

protected:

void CreateFunctionsDM();
void SetJFactor();
void ReadJFactorBonnivard();
void ReadJFactorGeringer();
void SetLOS();


Double_t TGraphEvaluateJFactorVsTheta(Double_t* x, Double_t* par);
Double_t EvaluateQFactorVsTheta(Double_t* x, Double_t* par);


private:

///////////////////////////////////////////////////////
//TString
///////////////////////////////////////////////////////

TString sAuthor;
TString sSource;
TString sCandidate;

///////////////////////////////////////////////////////
//Double_t
///////////////////////////////////////////////////////

Double_t dTheta;
Double_t dThetaNorm;
Double_t contadorMax;

///////////////////////////////////////////////////////
//TGraph
///////////////////////////////////////////////////////

TGraph* gJFactor;
TGraph* gLOS;

///////////////////////////////////////////////////////
//TF1
///////////////////////////////////////////////////////

TF1* fEvaluateJFactorVsTheta;
TF1* fEvaluateQFactorVsTheta;

};



#endif /* JDDarkMatter_H_ */

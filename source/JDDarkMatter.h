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

TF1* GetTF1LOSVsTheta()
{
	return fEvaluateLOSVsTheta;
}

TF1* GetTF1JFactorFromLOSVsTheta()
{
	return fEvaluateJFactorFromLOSVsTheta;
}

Double_t GetTheta()
{
	return dTheta;
}

TString GetAuthor()
{
	return sAuthor;
}
TString GetSource()
{
	return sSource;
}

TString GetCandidate()
{
	return sCandidate;
}



//GetJFactorName
//GetAuthorName
//GetIsAnnihilation

protected:

void CreateFunctionsDM();
void SetJFactor();
void ReadJFactorBonnivard();
void ReadJFactorGeringer();



Double_t TGraphEvaluateJFactorVsTheta(Double_t* x, Double_t* par);
Double_t EvaluateQFactorVsTheta(Double_t* x, Double_t* par);
Double_t EvaluateLOSVsTheta(Double_t* x, Double_t* par);
Double_t EvaluateLOSPerSinusVsTheta(Double_t* x, Double_t* par);
Double_t EvaluateJFactorFromLOSVsTheta(Double_t* x, Double_t* par);

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

///////////////////////////////////////////////////////
//TGraph
///////////////////////////////////////////////////////

TGraph* gJFactor;

///////////////////////////////////////////////////////
//TF1
///////////////////////////////////////////////////////

TF1* fEvaluateJFactorVsTheta;
TF1* fEvaluateQFactorVsTheta;
TF1* fEvaluateLOSVsTheta;
TF1* fEvaluateLOSPerSinusVsTheta;
TF1* fEvaluateJFactorFromLOSVsTheta;

};



#endif /* JDDarkMatter_H_ */

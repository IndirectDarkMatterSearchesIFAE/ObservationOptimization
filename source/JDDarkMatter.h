/*
 * DM.h
 *
 *  Created on: 03/07/2017
 *  Authors: David Navarro Gironés 	<<david.navarrogir@e-campus.uab.cat>>
 *  		 Joaquim Palacio 		<<jpalacio@ifae.es>>
 *
 *  		 ADD A GENERAL DESCRIPTION ON THE CLASS, THE MAIN FUNCTIONS, THE VARIABLES
 *  		 AND MENTION THE "runExample#.C" THAT SHOWS HOW TO USE IT
 */

#ifndef JDDarkMatter_H_
#define JDDarkMatter_H_

#include <TF1.h>
using namespace std;

class JDDarkMatter {
public:

	JDDarkMatter(TString author, TString source, TString candidate, TString mySourcePath);
		virtual ~JDDarkMatter();


///////////////////////////////////////////////////////
//TF1
///////////////////////////////////////////////////////

TF1* GetTF1JFactorVsTheta()
{
	return fEvaluateJFactorVsTheta;
}

TF1* GetTF1LOSVsTheta()
{
	return fEvaluateLOSVsTheta;
}

///////////////////////////////////////////////////////
//Double_t
///////////////////////////////////////////////////////

Double_t GetTheta()
{
	return dTheta;
}

///////////////////////////////////////////////////////
//TString
///////////////////////////////////////////////////////

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

protected:

void CreateFunctionsDM();
void SetJFactor();
void ReadJFactorBonnivard();
void ReadJFactorGeringer();



Double_t TGraphEvaluateJFactorVsTheta(Double_t* x, Double_t* par);
Double_t EvaluateLOSVsTheta(Double_t* x, Double_t* par);

private:

///////////////////////////////////////////////////////
//TString
///////////////////////////////////////////////////////

TString sAuthor;
TString sSource;
TString sCandidate;
TString sMySourcePath;

///////////////////////////////////////////////////////
//Double_t
///////////////////////////////////////////////////////

Double_t dTheta;

///////////////////////////////////////////////////////
//TGraph
///////////////////////////////////////////////////////

// (QUIM) you can specify here how this JFactor is going to be filled,
// make explicit that the same TGraph is used for Bon/Ger & Ann/Dec
TGraph* gJFactor;

///////////////////////////////////////////////////////
//TF1
///////////////////////////////////////////////////////

TF1* fEvaluateJFactorVsTheta;
TF1* fEvaluateLOSVsTheta;

};



#endif /* JDDarkMatter_H_ */

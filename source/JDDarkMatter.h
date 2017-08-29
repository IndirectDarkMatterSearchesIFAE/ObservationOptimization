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
#include <TGraph.h>
using namespace std;

class JDDarkMatter {
public:

	JDDarkMatter(TGraph* jfactor);
	JDDarkMatter(TString author, TString source, TString candidate, TString mySourcePath);
	virtual ~JDDarkMatter();

	void SetNumPointsJFactorGraph(Int_t numPoints) 		{iNumPointsJFactorGraph=numPoints;}
	void SetSourcePath(TString sourcePath) 				{sMySourcePath=sourcePath;}
	void SetSourceName(TString sourceName) 				{sSource=sourceName;}
	void SetCandidate(TString candidate) 				{sCandidate=candidate;}
	void SetAuthor(TString author) 						{sAuthor=author;}
	void SetThetaMax(Double_t thetaMax) 				{dThetaMax=thetaMax;}
	void SetJFactorMax(Double_t jFactorMax) 			{dJFactorMax=jFactorMax;}
	void SetJFactorMin(Double_t jFactorMin) 			{dJFactorMin=jFactorMin;}
	void SetIsBonnivard(Bool_t isBonnivard) 			{bIsBonnivard=isBonnivard;}
	void SetIsGeringer(Bool_t isGeringer) 				{bIsGeringer=isGeringer;}


	///////////////////////////////////////////////////////
	//void
	///////////////////////////////////////////////////////
	void GetListOfCandidates();
	void GetListOfSources();
	void GetListOfAuthors();

	///////////////////////////////////////////////////////
	//Int
	///////////////////////////////////////////////////////
	Int_t GetNumPointsJFactorGraph() 	{return iNumPointsJFactorGraph;}

	///////////////////////////////////////////////////////
	//TF1
	///////////////////////////////////////////////////////

	TF1* GetTF1JFactorVsTheta()		{return fEvaluateJFactorVsTheta;}
	TF1* GetTF1LOSVsTheta() 		{return fEvaluateLOSVsTheta;}
	TF1* GetTF1NormLOSVsTheta(Double_t normTheta)
	{
		fEvaluateNormLOSVsTheta->SetParameter(0,normTheta);
		return fEvaluateNormLOSVsTheta;
	}

	///////////////////////////////////////////////////////
	//Double_t
	///////////////////////////////////////////////////////

	Double_t GetThetaMax() 		{return dThetaMax;}			// [deg]
	Double_t GetJFactorMax() 	{return dJFactorMax;}		// [~GeV,~cm]
	Double_t GetJFactorMin() 	{return dJFactorMin;}		// [~GeV,~cm]


	///////////////////////////////////////////////////////
	//TString
	///////////////////////////////////////////////////////

	TString GetAuthor()			{return sAuthor;}
	TString GetCandidate()		{return sCandidate;}
	TString GetSourcePath() 	{return sMySourcePath;}
	TString GetSourceName() 	{return sSource;}

	///////////////////////////////////////////////////////
	//Boolt_T
	///////////////////////////////////////////////////////

	Bool_t GetIsBonnivard() 	{return bIsBonnivard;}
	Bool_t GetIsGeringer() 		{return bIsGeringer;}

protected:

	void CreateFunctionsDM();
	void ReadJFactorBonnivard(Bool_t verbose=0);
	void ReadJFactorGeringer(Bool_t verbose=0);

	Bool_t SetJFactorFromReferences(Bool_t verbose=0);
	Bool_t SetJFactorFromTGraph(TGraph* jfactor);


	Double_t TGraphEvaluateJFactorVsTheta(Double_t* x, Double_t* par);
	Double_t EvaluateLOSVsTheta(Double_t* x, Double_t* par);
	Double_t EvaluateNormLOSVsTheta(Double_t* x, Double_t* par);

private:

	///////////////////////////////////////////////////////
	//TString
	///////////////////////////////////////////////////////

	TString sAuthor;
	TString sSource;
	TString sCandidate;
	TString sMySourcePath;

	///////////////////////////////////////////////////////
	//Int_t
	///////////////////////////////////////////////////////

	Int_t iNumPointsJFactorGraph;

	///////////////////////////////////////////////////////
	//Double_t
	///////////////////////////////////////////////////////

	Double_t dThetaMax;
	Double_t dJFactorMax;
	Double_t dJFactorMin;

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
	TF1* fEvaluateNormLOSVsTheta;

	///////////////////////////////////////////////////////
	//Bool_t
	///////////////////////////////////////////////////////

	Bool_t bIsBonnivard=0;
	Bool_t bIsGeringer=0;


};



#endif /* JDDarkMatter_H_ */

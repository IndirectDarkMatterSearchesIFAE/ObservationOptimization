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
#include <TF2.h>

using namespace std;

class JDDarkMatter {
public:

	JDDarkMatter();
	JDDarkMatter(TGraph* jfactor);
	JDDarkMatter(TString txtFile);
	JDDarkMatter(TString author, TString source, TString candidate, TString mySourcePath);
	virtual ~JDDarkMatter();


	//Setters********

	void SetSourcePath(TString sourcePath) 				{sMySourcePath=sourcePath;}
	void SetSourceName(TString sourceName) 				{sSource=sourceName;}
	void SetCandidate(TString candidate) 				{sCandidate=candidate;}
	void SetAuthor(TString author) 						{sAuthor=author;}
	void SetThetaMax(Double_t thetaMax) 				{dThetaMax=thetaMax;}

	Bool_t SetJFactorFromTGraph(TGraph* jfactor);//, Bool_t verbose);
	Bool_t SetJFactorFromTxtFile(TString txtFile);//, Bool_t verbose);

	//Getters********

	///////////////////////////////////////////////////////
	//void
	///////////////////////////////////////////////////////
	void GetListOfCandidates();
	void GetListOfSources();
	void GetListOfAuthors();
	void GetListOfConstructors();
	void GetUnits();
	void GetWarning();
	///////////////////////////////////////////////////////
	//Int
	///////////////////////////////////////////////////////
	Int_t GetNumPointsJFactorGraph() 	{return iNumPointsJFactorGraph;}

	///////////////////////////////////////////////////////
	//TF1
	///////////////////////////////////////////////////////

	TF1* GetTF1JFactorVsTheta()
	{
		if(!GetIsJFactor()) GetWarning();
		return fEvaluateJFactorVsTheta;
	}

	TF1* GetTF1JFactorFromLOSVsTheta()
	{
		if(!GetIsJFactor()) GetWarning();
		return fIntegrateJFactorFromLOSVsTheta;
	}

	TF1* GetTF1JFactorOffFromLOSVsTheta(Double_t offset=20)
	{
		if(!GetIsJFactor()) GetWarning();
		fIntegrateJFactorOffFromLOSVsTheta->SetParameter(0,offset);
		return fIntegrateJFactorOffFromLOSVsTheta;
	}

	TF1* GetTF1LOSVsTheta()
	{
		if(!GetIsJFactor()) GetWarning();
		return fEvaluateLOSVsTheta;
	}

	TF1* GetTF1NormLOSVsTheta(Double_t normTheta)
	{
		if(!GetIsJFactor()) GetWarning();
		fEvaluateNormLOSVsTheta->SetParameter(0,normTheta);
		return fEvaluateNormLOSVsTheta;
	}

	TF1* GetTF2LOSVsTheta()
	{
		if(!GetIsJFactor()) GetWarning();
		return fEvaluateLOSVsTheta;
	}
	///////////////////////////////////////////////////////
	//TF2
	///////////////////////////////////////////////////////



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
	//Bool_t
	///////////////////////////////////////////////////////

	Bool_t GetIsBonnivard() 	{return bIsBonnivard;}
	Bool_t GetIsGeringer() 		{return bIsGeringer;}
	Bool_t GetIsJFactor()		{return bIsJFactor;}


protected:

	//Setters********
	void SetIsBonnivard(Bool_t isBonnivard) 			{bIsBonnivard=isBonnivard;}
	void SetIsGeringer(Bool_t isGeringer) 				{bIsGeringer=isGeringer;}
	void SetIsJFactor(Bool_t isJFactor)					{bIsJFactor=isJFactor;}
	void SetNumPointsJFactorGraph(Int_t numPoints) 		{iNumPointsJFactorGraph=numPoints;}

	void SetJFactorMax(Double_t jFactorMax) 			{dJFactorMax=jFactorMax;}
	void SetJFactorMin(Double_t jFactorMin) 			{dJFactorMin=jFactorMin;}

	Bool_t SetJFactorFromReferences(Bool_t verbose=0);

	//OTHERS********

	void CreateFunctionsDM();
	void ReadJFactorBonnivard(Bool_t verbose=0);
	void ReadJFactorGeringer(Bool_t verbose=0);

	Double_t TGraphEvaluateJFactorVsTheta(Double_t* x, Double_t* par);
	Double_t IntegrateJFactorFromLOSVsTheta(Double_t* x, Double_t* par);
	Double_t IntegrateJFactorOffFromLOSVsTheta(Double_t* x, Double_t* par);
	Double_t EvaluateLOSVsTheta(Double_t* x, Double_t* par);
	Double_t EvaluateLOSThetaVsTheta(Double_t* x, Double_t* par);
	Double_t EvaluateNormLOSVsTheta(Double_t* x, Double_t* par);

	Double_t EvaluateLOSThetaVsThetaPhi(Double_t* x, Double_t* par);
	Double_t EvaluateLOSOffThetaVsThetaPhi(Double_t* x, Double_t* par);

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

	Double_t dDeg2Rad;

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
	TF1* fEvaluateLOSThetaVsTheta;
	TF1* fEvaluateNormLOSVsTheta;
	TF1* fIntegrateJFactorFromLOSVsTheta;
	TF1* fIntegrateJFactorOffFromLOSVsTheta;

	///////////////////////////////////////////////////////
	//TF2
	///////////////////////////////////////////////////////

	TF2* fEvaluateLOSThetaVsThetaPhi;
	TF2* fEvaluateLOSOffThetaVsThetaPhi;

	///////////////////////////////////////////////////////
	//Bool_t
	///////////////////////////////////////////////////////

	Bool_t bIsBonnivard;
	Bool_t bIsGeringer;
	Bool_t bIsJFactor;

};



#endif /* JDDarkMatter_H_ */

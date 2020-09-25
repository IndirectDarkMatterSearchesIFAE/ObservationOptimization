/*
 * JDDarkMatter.h
 *
 *  Created on: 03/07/2017
 *  Last revision: 01/12/2017
 *
 *  Authors: David Navarro Gironés 	<<david.navarro.girones@gmail.com>>
 *  		 Joaquim Palacio 		<<jpalacio@ifae.es>>
 *
 *			 THIS CLASS IS THE ONE RELATED WITH THE DARK MATTER DATA.
 *			 WITH THIS CLASS YOU CAN EVALUATE THE JFACTOR VS THETA, THE LOS VS THETA AND PHI, THE JFACTOR OBTAINED BY INTEGRATING THE LOS.
 *  		 VARIABLES:
 *  		 	THETA 	[DEG]
 *  		 	PHI	  	[RAD]
 *  		 	OFFSET	[DEG]
 *  		 The macro "exampleJDDarkMatter.cxx" shows how to use this class.
 */

#ifndef JDDarkMatter_H_
#define JDDarkMatter_H_

//#include "JDAstroProfile.h"

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

	void SetSourcePath(TString sourcePath) 										{sMySourcePath=sourcePath;}
	void SetSourceName(TString sourceName) 										{sSource=sourceName;}
	void SetCandidate(TString candidate) 										{sCandidate=candidate;}
	void SetAuthor(TString author) 												{sAuthor=author;}
	// Michele
	void SetIsSphericalCoordinates(Bool_t isSphericalCoordinates)				{bIsSphericalCoordinates=isSphericalCoordinates;}



	Bool_t SetJFactorFromTGraph(TGraph* jfactor, Bool_t verbose=0);//, Bool_t verbose);
	Bool_t SetJFactorFromTxtFile(TString txtFile, Bool_t verbose=0);//, Bool_t verbose);

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

	TF1* GetTF1JFactorSigma1VsTheta()
	{
		if(!GetIsJFactor()) GetWarning();
		return fEvaluateJFactorSigma1VsTheta;
	}
	// Michele
	TF1* GetTF1dNdOmegaVsTheta()
	{
		if(!GetIsdNdOmega()) GetWarning();
		return fdNdOmegaVsTheta;
	}

	TF1* GetTF1dNdOmegaSigma1VsTheta()
	{
		if(!GetIsdNdOmega()) GetWarning();
		return fdNdOmegaSigma1VsTheta;
	}

	TF1* GetTF1NormdNdOmegaVsTheta(Double_t normTheta)
	{
		if(!GetIsdNdOmega()) GetWarning();
		fNormdNdOmegaVsTheta->SetParameter(0,normTheta);
		return fNormdNdOmegaVsTheta;
	}

	TF1* GetTF1NormdNdOmegaSigma1VsTheta(Double_t normTheta)
	{
		if(!GetIsdNdOmega()) GetWarning();
		fNormdNdOmegaSigma1VsTheta->SetParameter(0,normTheta);
		return fNormdNdOmegaSigma1VsTheta;
	}

	//QUIM NEW
//	TF1* GetTF1IntegratedNdOmegaVsTheta()
//	{
//		if(!GetIsdNdOmega()) GetWarning();
//		return fIntegratedNdOmegaVsTheta;
//	}

	TF1* GetTF1IntegratedNdOmegaThetaVsTheta()
	{
		if(!GetIsdNdOmega()) GetWarning();
		return fIntegratedNdOmegaThetaVsTheta;
	}

	TF1* GetTF1IntegratedNdOmegaOffThetaVsTheta(Double_t offsetDistance)
	{
		if(!GetIsdNdOmega()) GetWarning();
		fIntegratedNdOmegaOffThetaVsTheta->SetParameter(0,offsetDistance);
		return fIntegratedNdOmegaOffThetaVsTheta;
	}

	TF1* GetTF1IntegratedNdOmegaSigma1ThetaVsTheta()
	{
		if(!GetIsdNdOmega()) GetWarning();
		return fIntegratedNdOmegaSigma1ThetaVsTheta;
	}

	TF1* GetTF1IntegratedNdOmegaSigma1OffThetaVsTheta()
	{
		if(!GetIsdNdOmega()) GetWarning();
		return fIntegratedNdOmegaSigma1OffThetaVsTheta;
	}

	///////////////////////////////////////////////////////
	//TF2
	///////////////////////////////////////////////////////
	//Michele
	TF2* GetTF2dNdOmegaThetaVSThetaPhi()
	{
		if(!GetIsdNdOmega()) GetWarning();
		return fdNdOmegaThetaVsThetaPhi;
	}

	TF2* GetTF2dNdOmegaSigma1ThetaVSThetaPhi()
	{
		if(!GetIsdNdOmega()) GetWarning();
		return fdNdOmegaSigma1ThetaVsThetaPhi;
	}

	TF2* GetTF2dNdOmegaOffVsThetaPhi(Double_t offset=20)
	{
		if(!GetIsdNdOmega()) GetWarning();
		fdNdOmegaOffVsThetaPhi->SetParameter(0,offset);
		return fdNdOmegaOffVsThetaPhi;
	}

	TF2* GetTF2dNdOmegaOffThetaVSThetaPhi(Double_t offset=20)
	{
		if(!GetIsdNdOmega()) GetWarning();
		fdNdOmegaOffThetaVsThetaPhi->SetParameter(0,offset);
		return fdNdOmegaOffThetaVsThetaPhi;
	}

	TF2* GetTF2dNdOmegaSigma1OffThetaVSThetaPhi(Double_t offset=20)
	{
		if(!GetIsdNdOmega()) GetWarning();
		fdNdOmegaSigma1OffThetaVsThetaPhi->SetParameter(0,offset);
		return fdNdOmegaSigma1OffThetaVsThetaPhi;
	}

	///////////////////////////////////////////////////////
	//Double_t
	///////////////////////////////////////////////////////
	Double_t GetJFactorMax() 		{return dJFactorMax;}		// [~GeV,~cm]
	Double_t GetJFactorSigma1Max() 	{return dJFactorSigma1Max;}	// [~GeV,~cm]
	Double_t GetJFactorMin() 		{return dJFactorMin;}		// [~GeV,~cm]
	Double_t GetJFactorSigma1Min() 	{return dJFactorSigma1Min;}	// [~GeV,~cm]
	// Michele
	Double_t GetThetaMax() 			{return dThetaMax;}			// [deg]
	Double_t GetThetaMin() 			{return dThetaMin;}			// [deg]


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
	Bool_t GetIsBonnivard() 					{return bIsBonnivard;}
	Bool_t GetIsGeringer() 						{return bIsGeringer;}
	Bool_t GetIsJFactor()						{return bIsJFactor;}
	Bool_t GetIsJFactorSigma1()					{return bIsJFactorSigma1;}
	// Michele
	Bool_t GetIsdNdOmega()						{return bIsdNdOmega;}
	Bool_t GetIsdNdOmegaSigma1()				{return bIsdNdOmegaSigma1;}
	Bool_t GetIsSphericalCoordinates()			{return bIsSphericalCoordinates;}

protected:

	//Setters********
	void SetIsBonnivard(Bool_t isBonnivard) 			{bIsBonnivard=isBonnivard;}
	void SetIsGeringer(Bool_t isGeringer) 				{bIsGeringer=isGeringer;}
	void SetIsJFactor(Bool_t isJFactor)					{bIsJFactor=isJFactor;}
	void SetIsJFactorSigma1(Bool_t isJFactorSigma1)			{bIsJFactorSigma1=isJFactorSigma1;}
	void SetNumPointsJFactorGraph(Int_t numPoints) 		{iNumPointsJFactorGraph=numPoints;}

	void SetJFactorMax(Double_t jFactorMax) 				{dJFactorMax=jFactorMax;}
	void SetJFactorSigma1Max(Double_t jFactorSigma1Max) 			{dJFactorSigma1Max=jFactorSigma1Max;}
	void SetJFactorMin(Double_t jFactorMin) 				{dJFactorMin=jFactorMin;}
	void SetJFactorSigma1Min(Double_t jFactorSigma1Min) 			{dJFactorSigma1Min=jFactorSigma1Min;}

	Bool_t SetdNdOmegaFromJFactor();

	Bool_t SetJFactorFromReferences(Bool_t verbose=0);

	//Michele
	void SetThetaMax(Double_t thetaMax) 					{dThetaMax=thetaMax;}
	void SetThetaMin(Double_t thetaMin) 					{dThetaMin=thetaMin;}

	void SetIsdNdOmega(Bool_t isdNdOmega)					{bIsdNdOmega=isdNdOmega;}
	void SetIsdNdOmegaSigma1(Bool_t isdNdOmegaSigma1)				{bIsdNdOmegaSigma1=isdNdOmegaSigma1;}



	//OTHERS********
	void CreateFunctionsDM();
	//Michele
	void CreateFunctionsAP();

	void ReadJFactorBonnivard(Bool_t verbose=0);
	void ReadJFactorGeringer(Bool_t verbose=0);

	Double_t TGraphEvaluateJFactorVsTheta(Double_t* x, Double_t* par);
	Double_t TGraphEvaluateJFactorSigma1VsTheta(Double_t* x, Double_t* par);
	//Michele
	Double_t TGraphdNdOmegaVsTheta(Double_t* x, Double_t* par);
	Double_t TGraphdNdOmegaSigma1VsTheta(Double_t* x, Double_t* par);

	Double_t NormdNdOmegaVsTheta(Double_t* x, Double_t* par);
	Double_t NormdNdOmegaSigma1VsTheta(Double_t* x, Double_t* par);

	Double_t IntegratedNdOmegaThetaVsTheta(Double_t* x, Double_t* par);
	Double_t IntegratedNdOmegaSigma1ThetaVsTheta(Double_t* x, Double_t* par);
	Double_t IntegratedNdOmegaOffThetaVsTheta(Double_t* x, Double_t* par);
	Double_t IntegratedNdOmegaSigma1OffThetaVsTheta(Double_t* x, Double_t* par);

	Double_t dNdOmegaVsTheta(Double_t* x, Double_t* par);
	Double_t dNdOmegaSigma1VsTheta(Double_t* x, Double_t* par);
	Double_t dNdOmegaOffVsThetaPhi(Double_t* x, Double_t* par);
	Double_t dNdOmegaSigma1OffVsThetaPhi(Double_t* x, Double_t* par);

	Double_t dNdOmegaThetaVsThetaPhi(Double_t* x, Double_t* par);
	Double_t dNdOmegaSigma1ThetaVsThetaPhi(Double_t* x, Double_t* par);
	Double_t dNdOmegaOffThetaVsThetaPhi(Double_t* x, Double_t* par);
	Double_t dNdOmegaSigma1OffThetaVsThetaPhi(Double_t* x, Double_t* par);

private:

	///////////////////////////////////////////////////////
	//TString
	///////////////////////////////////////////////////////
	TString sAuthor;
	TString sCandidate;
	TString sSource;
	TString sMySourcePath;

	///////////////////////////////////////////////////////
	//Int_t
	///////////////////////////////////////////////////////
	Int_t iNumPointsJFactorGraph;

	///////////////////////////////////////////////////////
	//Double_t
	///////////////////////////////////////////////////////
	Double_t dJFactorMax;
	Double_t dJFactorSigma1Max;
	Double_t dJFactorMin;
	Double_t dJFactorSigma1Min;

	Double_t dDeg2Rad;
	Double_t dBinResolution;

	//Michele
	Double_t dThetaMax;
	Double_t dThetaMin;

	///////////////////////////////////////////////////////
	//TGraph
	///////////////////////////////////////////////////////
	TGraph* gJFactor;
	TGraph* gJFactorSigma1;
	TGraph* gdNdOmega;
	TGraph* gdNdOmegaSigma1;

	///////////////////////////////////////////////////////
	//TF1
	///////////////////////////////////////////////////////
	TF1* fEvaluateJFactorVsTheta;
	TF1* fEvaluateJFactorSigma1VsTheta;
	//Michele
	TF1* fIntegratedNdOmegaThetaVsTheta;
	TF1* fIntegratedNdOmegaSigma1ThetaVsTheta;

	TF1* fIntegratedNdOmegaOffThetaVsTheta;
	TF1* fIntegratedNdOmegaSigma1OffThetaVsTheta;
	TF1* fdNdOmegaVsTheta;
	TF1* fdNdOmegaSigma1VsTheta;
	//To delete
	TF1* fNormdNdOmegaVsTheta;
	TF1* fNormdNdOmegaSigma1VsTheta;

	///////////////////////////////////////////////////////
	//TF2
	///////////////////////////////////////////////////////
	//Michele
	TF2* fdNdOmegaSigma1OffVsThetaPhi;
	TF2* fdNdOmegaOffVsThetaPhi;
	//Michele
	TF2* fdNdOmegaThetaVsThetaPhi;
	TF2* fdNdOmegaSigma1ThetaVsThetaPhi;
	TF2* fdNdOmegaOffThetaVsThetaPhi;
	TF2* fdNdOmegaSigma1OffThetaVsThetaPhi;

	///////////////////////////////////////////////////////
	//Bool_t
	///////////////////////////////////////////////////////
	Bool_t bIsBonnivard;
	Bool_t bIsGeringer;
	Bool_t bIsJFactor;
	Bool_t bIsJFactorSigma1;
	//Michele
	Bool_t bIsdNdOmega;
	Bool_t bIsdNdOmegaSigma1;
	Bool_t bIsSphericalCoordinates;
};

#endif /* JDDarkMatter_H_ */

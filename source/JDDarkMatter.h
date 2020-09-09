/*
 * JDDarkMatter.h
 *
 *  Created on: 03/07/2017
 *  Last revision: 09/09/2020
 *
 *  Authors: 
 *   David Navarro Gironés 	<<david.navarro.girones@gmail.com>>
 *   Joaquim Palacio 		<<jpalacio@ifae.es>>
 *
 *  THIS CLASS IS THE ONE RELATED WITH THE DARK MATTER DATA.
 *  WITH THIS CLASS YOU CAN EVALUATE THE JFACTOR VS THETA, 
 * THE LOS VS THETA AND PHI, THE JFACTOR OBTAINED BY INTEGRATING THE LOS.
 * 
 *  VARIABLES:
 *  	THETA 	[DEG]
 *  	PHI	  	[RAD]
 *  	OFFSET	[DEG]
 *  The macro "macros/exampleJDDarkMatter.cxx" shows how to use this class.
 */

#ifndef JDDarkMatter_H_
#define JDDarkMatter_H_

#include "/Users/mdoro/Soft/ObservationOptimization/source/JDAstroProfile.h"

#include <TF1.h>
#include <TGraph.h>
#include <TF2.h>

using namespace std;

class JDDarkMatter : public JDAstroProfile {
public:

  JDDarkMatter();
  JDDarkMatter(TGraph* jfactor);
  JDDarkMatter(TString txtFile);
  JDDarkMatter(TString author, TString source, TString candidate, TString mySourcePath);
  virtual ~JDDarkMatter();


  //Setters********
  void SetSourcePath(TString sourcePath) {sMySourcePath=sourcePath;}
  void SetSourceName(TString sourceName) {sSource=sourceName;}
  void SetCandidate(TString candidate) 	 {sCandidate=candidate;}
  void SetAuthor(TString author) 	 {sAuthor=author;}

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


  ///////////////////////////////////////////////////////
  //TF2
  ///////////////////////////////////////////////////////
  
  
  ///////////////////////////////////////////////////////
  //Double_t
  ///////////////////////////////////////////////////////
  Double_t GetJFactorMax() 		{return dJFactorMax;}		// [~GeV,~cm]
  Double_t GetJFactorSigma1Max() 	{return dJFactorSigma1Max;}	// [~GeV,~cm]
  Double_t GetJFactorMin() 		{return dJFactorMin;}		// [~GeV,~cm]
  Double_t GetJFactorSigma1Min() 	{return dJFactorSigma1Min;}	// [~GeV,~cm]

  
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
  
protected:

  //Setters********
  void SetIsBonnivard(Bool_t isBonnivard)  	  {bIsBonnivard=isBonnivard;}
  void SetIsGeringer(Bool_t isGeringer) 	  {bIsGeringer=isGeringer;}
  void SetIsJFactor(Bool_t isJFactor)		  {bIsJFactor=isJFactor;}
  void SetIsJFactorSigma1(Bool_t isJFactorSigma1) {bIsJFactorSigma1=isJFactorSigma1;}
  void SetNumPointsJFactorGraph(Int_t numPoints)  {iNumPointsJFactorGraph=numPoints;}
  void SetJFactorMax(Double_t jFactorMax)         {dJFactorMax=jFactorMax;}
  void SetJFactorSigma1Max(Double_t jFactorSigma1Max)	{dJFactorSigma1Max=jFactorSigma1Max;}
  void SetJFactorMin(Double_t jFactorMin) 		{dJFactorMin=jFactorMin;}
  void SetJFactorSigma1Min(Double_t jFactorSigma1Min)	{dJFactorSigma1Min=jFactorSigma1Min;}
  Bool_t SetdNdOmegaFromJFactor();
  Bool_t SetJFactorFromReferences(Bool_t verbose=0);
  
  //OTHERS********
  void CreateFunctionsDM();
  void ReadJFactorBonnivard(Bool_t verbose=0);
  void ReadJFactorGeringer(Bool_t verbose=0);
  
  Double_t TGraphEvaluateJFactorVsTheta(Double_t* x, Double_t* par);
  Double_t TGraphEvaluateJFactorSigma1VsTheta(Double_t* x, Double_t* par);

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
  
  ///////////////////////////////////////////////////////
  //TGraph
  ///////////////////////////////////////////////////////
  TGraph* gJFactor;
  TGraph* gJFactorSigma1;
  
  ///////////////////////////////////////////////////////
  //TF1
  ///////////////////////////////////////////////////////
  TF1* fEvaluateJFactorVsTheta;
  TF1* fEvaluateJFactorSigma1VsTheta;
  
  
  ///////////////////////////////////////////////////////
  //TF2
  ///////////////////////////////////////////////////////
  
  
  ///////////////////////////////////////////////////////
  //Bool_t
  ///////////////////////////////////////////////////////
  Bool_t bIsBonnivard;
  Bool_t bIsGeringer;
  Bool_t bIsJFactor;
  Bool_t bIsJFactorSigma1;
};

#endif /* JDDarkMatter_H_ */

/*
 * Instrument.h
 *
 *  Created on: 03/07/2017
 *  Authors: David Navarro Gironés 	<<david.navarrogir@e-campus.uab.cat>>
 *  		 Joaquim Palacio 		<<jpalacio@ifae.es>>
 *
 *  		 ADD A GENERAL DESCRIPTION ON THE CLASS, THE MAIN FUNCTIONS, THE VARIABLES
 *  		 AND MENTION THE "runExample#.C" THAT SHOWS HOW TO USE IT
 */

#ifndef INSTRUMENT_H_
#define INSTRUMENT_H_

#include <TF1.h>
#include <TF2.h>
#include <TH2.h>



class JDInstrument {
public:
	JDInstrument();
	JDInstrument(Double_t distanceCameraCenterMax, Double_t wobbleDist, TString instrumentName);
	JDInstrument(TGraph* cameraAcceptance,Double_t wobbleDist);
	JDInstrument(TString txtFile, Double_t wobbleDist);
	JDInstrument(TString instrumentName, Double_t wobble, TString instrumentPath);
	virtual ~JDInstrument();

	//Getters********
	///////////////////////////////////////////////////////
	//void
	///////////////////////////////////////////////////////
	void GetListOfInstruments();
	void GetListOfConstructors();
	void GetUnits();
	void GetWarning();

	Bool_t GetIsIdeal()						{return bIsIdeal;}
	Bool_t GetIsMAGIC()						{return bIsMagic;}
	Bool_t GetIsCTA()						{return bIsCTA;}
	Bool_t GetIsCameraAcceptance()			{return bIsCameraAcceptance;}
	Bool_t GetIsSphericalCoordinates()		{return bIsSphericalCoordinates;}

	TString GetInstrumentName()			{return sInstrumentName;}
	TString GetInstrumentPath()			{return sInstrumentPath;}

	Int_t GetNumPointsCameraAcceptanceGraph()	{return iNumPointsCameraAcceptanceGraph;}

	Double_t GetDistCameraCenterMax()	{return dDistCenterCameraMax;}
//	Double_t GetThetaMax()				{return dThetaMax;}
	Double_t GetWobbleDistance()		{return dWobbleDist;}


	TF1* GetTF1EpsilonVsTheta()
	{
		if(!GetIsCameraAcceptance()) GetWarning();
		return fEvaluateEpsilonVsTheta;
	}

	TF1* GetTF1EfficiencyVsTheta(Double_t WobbleDistance)
	{
		if(!GetIsCameraAcceptance()) GetWarning();
		fEvaluateEfficiencyVsTheta->SetParameter(0, WobbleDistance);
		return fEvaluateEfficiencyVsTheta;
	}

	TF2* GetTF2EpsilonVsThetaAndPhi()
	{
		if(!GetIsCameraAcceptance()) GetWarning();
		fEvaluateEpsilonVsThetaAndPhi->SetParameter(0,GetWobbleDistance());
		return fEvaluateEpsilonVsThetaAndPhi;
	}

	TF2* GetTF2EpsilonThetaVsThetaAndPhi()
	{
		if(!GetIsCameraAcceptance()) GetWarning();
		fEvaluateEpsilonThetaVsThetaAndPhi->SetParameter(0,GetWobbleDistance());
		return fEvaluateEpsilonThetaVsThetaAndPhi;
	}

	TF2* GetTF2EpsilonVsXAndY()
	{
		if(!GetIsCameraAcceptance()) GetWarning();
		fEvaluateEpsilonVsXAndY->SetParameter(0,GetWobbleDistance());
		return fEvaluateEpsilonVsXAndY;
	}

	//Setters********

	void SetDistCenterCameraMax(Double_t distDistCenterCamMax)	{dDistCenterCameraMax=distDistCenterCamMax;}
	void SetInstrumentName(TString instrumentName)				{sInstrumentName=instrumentName;}
	void SetInstrumentPath(TString instrumentPath)				{sInstrumentPath=instrumentPath;}

protected:

	//Setters********
	Bool_t SetCameraAcceptanceIdeal(Double_t distanceCenterCameraMax);
	Bool_t SetCameraAcceptanceFromInstrument(Bool_t verbose = 0);
	Bool_t SetCameraAcceptanceFromTGraph(TGraph* cameraAcceptance, Bool_t verbose = 0);
	Bool_t SetCameraAcceptanceFromTxtFile(TString txtFile, Bool_t verbose = 0);

	void SetNumPointsCameraAcceptanceGraph(Int_t numPoints)			{iNumPointsCameraAcceptanceGraph=numPoints;}

	void SetIsIdeal(Bool_t isIdeal)									{bIsIdeal=isIdeal;}
	void SetIsMagic(Bool_t isMagic)									{bIsMagic=isMagic;}
	void SetIsCTA(Bool_t isCTA)										{bIsCTA=isCTA;}
	void SetIsCameraAcceptance(Bool_t isCameraAcceptance)			{bIsCameraAcceptance=isCameraAcceptance;}
	void SetWobbleDist(Double_t wobbleDist)							{dWobbleDist=wobbleDist;}


	//OTHERS********
	void CreateFunctionsInstrument();

	Double_t EvaluateEpsilonVsTheta(Double_t* x, Double_t* par);
	Double_t EvaluateEpsilonVsThetaAndPhi(Double_t* x, Double_t* par);
	Double_t EvaluateEpsilonVsXAndY(Double_t* x, Double_t* par);
	Double_t EvaluateEpsilonThetaVsThetaAndPhi(Double_t* x, Double_t* par);
	Double_t EvaluateEfficiencyVsTheta(Double_t* x, Double_t* par);



private:

	///////////////////////////////////////////////////////
	//TString
	///////////////////////////////////////////////////////

	TString sInstrumentName;
	TString sInstrumentPath;

	///////////////////////////////////////////////////////
	//TString
	///////////////////////////////////////////////////////

	Int_t iNumPointsCameraAcceptanceGraph;

	///////////////////////////////////////////////////////
	//Double_t
	///////////////////////////////////////////////////////

	Double_t dDistCenterCameraMax;
	Double_t dWobbleDist;
//	Double_t dThetaMax;
	Double_t dDeg2Rad;


	///////////////////////////////////////////////////////
	//TGraph
	///////////////////////////////////////////////////////

	TGraph* gCameraAcceptance;

	///////////////////////////////////////////////////////
	//TF1
	///////////////////////////////////////////////////////

	TF1* fEvaluateEpsilonVsTheta;
	TF1* fEvaluateEfficiencyVsTheta;

	///////////////////////////////////////////////////////
	//TF2
	///////////////////////////////////////////////////////

	TF2* fEvaluateEpsilonVsThetaAndPhi;
	TF2* fEvaluateEpsilonVsXAndY;
	TF2* fEvaluateEpsilonThetaVsThetaAndPhi;

	///////////////////////////////////////////////////////
	//Bool_T
	///////////////////////////////////////////////////////
	Bool_t bIsIdeal;
	Bool_t bIsMagic;
	Bool_t bIsCTA;
	Bool_t bIsCameraAcceptance;
	Bool_t bIsSphericalCoordinates;

};


#endif /* INSTRUMENT_H_ */

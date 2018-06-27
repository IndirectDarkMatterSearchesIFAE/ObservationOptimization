/*
 * JDAstroProfile.h
 *
 *  Created on: 09/04/2018
 *
 *  Authors: Joaquim Palacio 		<<jpalacio@ifae.es>>
 *
 */

#ifndef JDAstroProfile_H_
#define JDAstroProfile_H_

#include <TF1.h>
#include <TGraph.h>
#include <TF2.h>

using namespace std;

class JDAstroProfile {
public:

	JDAstroProfile();
	virtual ~JDAstroProfile();

	//Setters********
	void SetIsSphericalCoordinates(Bool_t isSphericalCoordinates)				{bIsSphericalCoordinates=isSphericalCoordinates;}


	//Getters********

	///////////////////////////////////////////////////////
	//void
	///////////////////////////////////////////////////////
	void GetWarning();
	void GetListOfConstructors();
	void GetIsSphericalCoordinates();

	///////////////////////////////////////////////////////
	//TF1
	///////////////////////////////////////////////////////
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
	TF1* GetTF1IntegratedNdOmegaVsTheta()
	{
		if(!GetIsdNdOmega()) GetWarning();
		return fIntegratedNdOmegaThetaVsTheta;
	}

	TF1* GetTF1IntegratedNdOmegaOffVsTheta(Double_t offsetDistance)
	{
		if(!GetIsdNdOmega()) GetWarning();
		fIntegratedNdOmegaOffThetaVsTheta->SetParameter(0,offsetDistance);
		return fIntegratedNdOmegaOffThetaVsTheta;
	}

	TF1* GetTF1IntegratedNdOmegaSigma1VsTheta()
	{
		if(!GetIsdNdOmega()) GetWarning();
		return fIntegratedNdOmegaSigma1ThetaVsTheta;
	}

	///////////////////////////////////////////////////////
	//TF2
	///////////////////////////////////////////////////////
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

	TF2* GetTF2dNdOmegaOffVSThetaPhi(Double_t offset=20)
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
	Double_t GetThetaMax() 			{return dThetaMax;}			// [deg]
	Double_t GetThetaMin() 			{return dThetaMin;}			// [deg]


	///////////////////////////////////////////////////////
	//TString
	///////////////////////////////////////////////////////


	///////////////////////////////////////////////////////
	//Bool_t
	///////////////////////////////////////////////////////
	Bool_t GetIsdNdOmega()						{return bIsdNdOmega;}
	Bool_t GetIsdNdOmegaSigma1()						{return bIsdNdOmegaSigma1;}
	Bool_t GetIsSphericalCoordinates()			{return bIsSphericalCoordinates;}


protected:

	//Setters********
	void SetThetaMax(Double_t thetaMax) 					{dThetaMax=thetaMax;}
	void SetThetaMin(Double_t thetaMin) 					{dThetaMin=thetaMin;}

	void SetIsdNdOmega(Bool_t isdNdOmega)					{bIsdNdOmega=isdNdOmega;}
	void SetIsdNdOmegaSigma1(Bool_t isdNdOmegaSigma1)				{bIsdNdOmegaSigma1=isdNdOmegaSigma1;}

	//OTHERS********
	void CreateFunctionsAP();

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

	///////////////////////////////////////////////////////
	//TGraph
	///////////////////////////////////////////////////////
	TGraph* gdNdOmega;
	TGraph* gdNdOmegaSigma1;

	///////////////////////////////////////////////////////
	//TF1
	///////////////////////////////////////////////////////
	TF1* fIntegratedNdOmegaThetaVsTheta;
	TF1* fIntegratedNdOmegaSigma1ThetaVsTheta;

	TF1* fIntegratedNdOmegaOffThetaVsTheta;
	TF1* fIntegratedNdOmegaSigma1OffThetaVsTheta;
	TF1* fdNdOmegaVsTheta;
	TF1* fdNdOmegaSigma1VsTheta;

	///////////////////////////////////////////////////////
	//TF2
	///////////////////////////////////////////////////////
	TF2* fdNdOmegaSigma1OffVsThetaPhi;
	TF2* fdNdOmegaOffVsThetaPhi;

	///////////////////////////////////////////////////////
	// TO REMOVE
	//TF1
	///////////////////////////////////////////////////////
	TF1* fNormdNdOmegaVsTheta;
	TF1* fNormdNdOmegaSigma1VsTheta;
	///////////////////////////////////////////////////////


private:

	///////////////////////////////////////////////////////
	//TString
	///////////////////////////////////////////////////////


	///////////////////////////////////////////////////////
	//Int_t
	///////////////////////////////////////////////////////


	///////////////////////////////////////////////////////
	//Double_t
	///////////////////////////////////////////////////////
	Double_t dThetaMax;
	Double_t dThetaMin;
	Double_t dDeg2Rad;
	Double_t dBinResolution;

	///////////////////////////////////////////////////////
	//TF2
	///////////////////////////////////////////////////////
	TF2* fdNdOmegaThetaVsThetaPhi;
	TF2* fdNdOmegaSigma1ThetaVsThetaPhi;
	TF2* fdNdOmegaOffThetaVsThetaPhi;
	TF2* fdNdOmegaSigma1OffThetaVsThetaPhi;

	///////////////////////////////////////////////////////
	//Bool_t
	///////////////////////////////////////////////////////
	Bool_t bIsdNdOmega;
	Bool_t bIsdNdOmegaSigma1;
	Bool_t bIsSphericalCoordinates;

};

#endif /* JDAstroProfile_H_ */

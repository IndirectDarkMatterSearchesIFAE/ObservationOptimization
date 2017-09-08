/*
 * Optimization.h
 *
 *  Created on: 03/07/2017
 *  Authors: David Navarro Gironés 	<<david.navarrogir@e-campus.uab.cat>>
 *  		 Joaquim Palacio 		<<jpalacio@ifae.es>>
 *
 *  		 ADD A GENERAL DESCRIPTION ON THE CLASS, THE MAIN FUNCTIONS, THE VARIABLES
 *  		 AND MENTION THE "runExample#.C" THAT SHOWS HOW TO USE IT
 */

#ifndef JDOptimization_H_
#define JDOptimization_H_

#include "JDInstrument.h"
#include "JDDarkMatter.h"


class JDOptimization {
public:
	JDOptimization(TString txtFile, TString instrumentName, Double_t distCameraCenter, Double_t wobble);
	JDOptimization(TString author, TString source, TString candidate, TString mySourcePath, TString instrumentName, Double_t distCameraCenter, Double_t wobble);
	virtual ~JDOptimization();

	void GetListOfQFactors();


	TF1* GetTF1QFactorVsTheta(Int_t type=0, Double_t thetaNorm=1)
	{
		if(type==0)	// J_on/theta
		{
			jdDarkMatter->SetIs_m1(0);
			fEvaluateQ0FactorVsTheta->SetParameter(0, thetaNorm);
			return fEvaluateQ0FactorVsTheta;
		}
		else if(type==1)	// J_on/Sqrt{theta^2+J_off}
		{
			jdDarkMatter->SetIs_m1(0);
			fEvaluateQ1FactorVsTheta->SetParameter(0, thetaNorm);
			return fEvaluateQ1FactorVsTheta;
		}

		else if(type==2)	//J_1sm/theta
		{
			jdDarkMatter->SetIs_m1(1);
			fEvaluateQ2FactorVsTheta->SetParameter(0, thetaNorm);
			return fEvaluateQ2FactorVsTheta;
		}

		else if (type==3)	//J_eff/theta_eff
		{
			jdDarkMatter->SetIs_m1(0);
			fEvaluateQ3FactorVsTheta->SetParameter(0, thetaNorm);
			return fEvaluateQ3FactorVsTheta;
		}

		else if (type==12)	//J_on_1sm/Sqrt{theta^2 + J_off_1sm}
		{
			jdDarkMatter->SetIs_m1(1);
			fEvaluateQ12FactorVsTheta->SetParameter(0, thetaNorm);
			return fEvaluateQ12FactorVsTheta;
		}

		else if (type==13)	//J_on_eff/Sqrt{(theta_eff)^2 + J_off_eff}
		{
			jdDarkMatter->SetIs_m1(0);
			fEvaluateQ13FactorVsTheta->SetParameter(0, thetaNorm);
			return fEvaluateQ13FactorVsTheta;
		}

		else if (type==23)	//J_1sm_eff/theta_eff
		{
			jdDarkMatter->SetIs_m1(1);
			fEvaluateQ23FactorVsTheta->SetParameter(0, thetaNorm);
			return fEvaluateQ23FactorVsTheta;
		}

		else if (type==123)	//J_on_1sm_eff/Sqrt{(theta_eff)^2 + J_off_1sm_eff}
		{
			jdDarkMatter->SetIs_m1(1);
			fEvaluateQ123FactorVsTheta->SetParameter(0, thetaNorm);
			return fEvaluateQ123FactorVsTheta;
		}
		else
		{
			TF1* f1;
			cout << endl;
			cout << "   ***************************************************************" << endl;
			cout << "   ***   WARNING: wrong Qfactor type!!!                        ***" << endl;
			cout << "   ***************************************************************" << endl;
			cout << endl;
			GetListOfQFactors();
			return f1;
		}
	}

//	TH2D* GetTH2DQFactorVsThetaWobble()
//	void* GetOptimalThetaAndWobble(Double_t &theta, Double_t &wobble)


	TF2* GetTF2QFactorVsThetaWobble(Int_t type=0, Double_t thetaNorm=1, Double_t wobbleNorm=0.4)
	{
		if(type==0)	// J_on/theta
		{
			fEvaluateQ0FactorVsThetaWobble->SetParameter(0, thetaNorm);
			fEvaluateQ0FactorVsThetaWobble->SetParameter(1, wobbleNorm);
			return fEvaluateQ0FactorVsThetaWobble;
		}
		if(type==1)	// J_on/theta
		{
			fEvaluateQ1FactorVsThetaWobble->SetParameter(0, thetaNorm);
			fEvaluateQ1FactorVsThetaWobble->SetParameter(1, wobbleNorm);
			return fEvaluateQ1FactorVsThetaWobble;
		}

		else
		{
			TF2* f2;
			cout << endl;
			cout << "   ***************************************************************" << endl;
			cout << "   ***   WARNING: wrong Qfactor type!!!                        ***" << endl;
			cout << "   ***************************************************************" << endl;
			cout << endl;
			GetListOfQFactors();
			return f2;
		}
	}


	// POSAR AQUESTS GETTERS COM ELS GETTERS QUE HI HA ABAIX, EN PRINCIPI LA FUNCIO OPTIMIZATION NOMÉS HAURIA DE DONAR LES Q FACTOR

	/**************************************************/

	//***** JDDarkMatter Getters
	void GetWarning()						{return jdDarkMatter->GetWarning();}

	Bool_t GetIsBonnivard()					{return jdDarkMatter->GetIsBonnivard();}
	Bool_t GetIsGeringer()					{return jdDarkMatter->GetIsGeringer();}
	Bool_t GetIsJFactor()					{return jdDarkMatter->GetIsJFactor();}

	TString GetSourceName()					{return jdDarkMatter->GetSourceName();}
	TString GetCandidate()					{return jdDarkMatter->GetCandidate();}

	Double_t GetThetaMax()					{return jdDarkMatter->GetThetaMax();}
	Double_t GetJFactorMax()				{return jdDarkMatter->GetJFactorMax();}
	Double_t GetJFactorMin()				{return jdDarkMatter->GetJFactorMin();}


//	TF1* GetTF1JFactorFromLOSVsTheta()
//	{
//		return jdDarkMatter->GetTF1JFactorFromLOSVsTheta();
//	}
//
//	TF1* GetTF1JFactorOffFromLOSVsTheta(){return jdDarkMatter->GetTF1JFactorOffFromLOSVsTheta(2*jdInstrument->GetWobbleDistance());}

	//***** JDDarkMatter Setters
	void SetCandidate(TString candidate)		{jdDarkMatter->SetCandidate(candidate);}


	//************************************************

	//***** JDInstrument Getters
	Bool_t GetIsIdeal()							{return jdInstrument->GetIsIdeal();}

	Double_t GetDistCameraCenterMax()			{return jdInstrument->GetDistCameraCenterMax();}
	Double_t GetWobbleDistance()				{return jdInstrument->GetWobbleDistance();}
	//***** JDInstrument Setters



protected:

	// Composition C++ scheme
	JDDarkMatter* jdDarkMatter;
	JDInstrument* jdInstrument;

	void CreateFunctions();

	Double_t EvaluateQ0FactorVsTheta(Double_t* x, Double_t* par);			// J_on/theta
	Double_t EvaluateQ1FactorVsTheta(Double_t* x, Double_t* par);			// J_on/Sqrt{theta^2+J_off}
	Double_t EvaluateQ2FactorVsTheta(Double_t* x, Double_t* par);			// J_1sm/theta
	Double_t EvaluateQ3FactorVsTheta(Double_t* x, Double_t* par);			// J_eff/theta_eff

	Double_t EvaluateQ12FactorVsTheta(Double_t* x, Double_t* par);			// J_eff/theta_eff
	Double_t EvaluateQ13FactorVsTheta(Double_t* x, Double_t* par);			// J_eff/theta_eff
	Double_t EvaluateQ23FactorVsTheta(Double_t* x, Double_t* par);			// J_eff/theta_eff
	Double_t EvaluateQ123FactorVsTheta(Double_t* x, Double_t* par);			// J_eff/theta_eff

	Double_t EvaluateQ0FactorVsThetaWobble(Double_t* x, Double_t* par);		// J_on/theta
	Double_t EvaluateQ1FactorVsThetaWobble(Double_t* x, Double_t* par);		// J_on/Sqrt{theta^2+J_off}
	Double_t EvaluateQ2FactorVsThetaWobble(Double_t* x, Double_t* par);		// J_eff/theta_eff

private:

	TF1* fEvaluateQ0FactorVsTheta;							// J_on/theta
	TF1* fEvaluateQ1FactorVsTheta;							// J_on/Sqrt{theta^2+J_off}
	TF1* fEvaluateQ2FactorVsTheta;							// J_1sm/theta
	TF1* fEvaluateQ3FactorVsTheta;							// J_eff/theta_eff

	TF1* fEvaluateQ12FactorVsTheta;							// J_on_1sm/Sqrt{theta^2 + J_off_1sm}
	TF1* fEvaluateQ13FactorVsTheta;							// J_on_eff/Sqrt{(theta_eff)^2 + J_off_eff}
	TF1* fEvaluateQ23FactorVsTheta;							// J_1sm_eff/theta_eff
	TF1* fEvaluateQ123FactorVsTheta;						// J_on_1sm_eff/Sqrt{(theta_eff)^2 + J_off_1sm_eff}

	TF2* fEvaluateQ0FactorVsThetaWobble;				// J_on/theta
	TF2* fEvaluateQ1FactorVsThetaWobble;				// J_on/Sqrt{theta^2+J_off}
	TF2* fEvaluateQ2FactorVsThetaWobble;				// J_1sm/theta
	TF2* fEvaluateQ3FactorVsThetaWobble;				// J_eff/theta_eff


	Double_t dDeg2Rad;

};

#endif /* 	JDOptimitzation_H_ */

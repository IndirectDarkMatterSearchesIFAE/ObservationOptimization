/*
 * DarkMatter.cc
 *
 *  Created on: 03/07/2017
 *      Author: david
 */

#include "JDOptimization.h"
#include "JDDarkMatter.h"

#include <TGraph.h>
#include <TMath.h>

using namespace std;

JDOptimization::JDOptimization(TString author, TString source, TString candidate, TString instrumentName, Double_t wobble):
		JDDarkMatter( author, source, candidate), JDInstrument( instrumentName, wobble)
{
	    cout << endl;
		cout << endl;
		cout << "Constructor JDOptimization..." << endl;
		cout << endl;
		cout << endl;

		CreateFunctions();
}

JDOptimization::~JDOptimization()
{

		cout << endl;
		cout << endl;
		cout << "Destructor JDOptimization..." << endl;
		cout << endl;
		cout << endl;

}

void JDOptimization::CreateFunctions()
{
		cout<<GetAuthor()<<endl;
		cout<<GetSource()<<endl;
		cout<<GetCandidate()<<endl;
		cout<<GetWobble()<<endl;
		cout<<GetInstrumentName()<<endl;

		//Vull fer el JFactor Effective, que és multiplicar el JFactor amb l'efectivitat

		fJFactorEffectiveVsTheta = new TF1("fJFactorEffectiveVsTheta", this, &JDOptimization::JFactorEffectiveVsTheta, 0., GetThetaMax(), 0, "JDOptimization", "JFactorEffectiveVsTheta");

}

Double_t JDOptimization::JFactorEffectiveVsTheta(Double_t* x, Double_t* par)
{
	return (GetTF1JFactorVsTheta()->Eval(x[0]))*(GetTF1EfficiencyVsTheta()->Eval(x[0]));
}


/*
 * DarkMatter.cc
 *
 *  Created on: 03/07/2017
 *      Author: david
 */

#include "JDOptimization.h"

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

	cout<<GetDcc()<<endl;

}

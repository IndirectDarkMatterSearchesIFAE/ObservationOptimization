/*
 * Optimization.h
 *
 *  Created on: 03/07/2017
 *      Author: david
 */

#ifndef JDOptimization_H_
#define JDOptimization_H_

#include "JDInstrument.h"
#include "JDDarkMatter.h"


class JDOptimization: public JDDarkMatter, public JDInstrument {
public:
	JDOptimization(TString author, TString source, TString candidate, TString instrumentName, Double_t wobble);
	virtual ~JDOptimization();

	TF1* GetJFactorEffectiveVsTheta()
	{
		return fJFactorEffectiveVsTheta;
	}

protected:

	void CreateFunctions();

	Double_t JFactorEffectiveVsTheta(Double_t* x, Double_t* par);

private:

	TF1* fJFactorEffectiveVsTheta;

};

#endif /* 	JDOptimitzation_H_ */

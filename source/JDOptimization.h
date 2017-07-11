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


protected:

	void CreateFunctions();

private:


};

#endif /* 	JDOptimitzation_H_ */

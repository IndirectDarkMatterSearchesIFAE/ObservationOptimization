/*
 * DM.cc
 *
 *  Created on: 03/07/2017
 *      Author: david
 */

#include "JDDarkMatter.h"
#include <TGraph.h>
#include <TMath.h>

using namespace std;

const static double Deg2Rad = TMath::Pi()/180.;
static const Double_t SolarMass2GeV = 1.1154e57;  			// [GeV/SolarM]
static const Double_t kpc2cm        = 3.08568e21; 			// [cm/kpc]

// NOMENCLATURA
//	1) que fa la funcio
//	2) que necessita com a input
//	3) que dona com a output
//	4) definir variables/paràmetres interns
//		x[0], x[1],..., x[n] -> variables
//		par[0], par[1],..., par[n] -> parameters
//


//-----------------------------------------------
//
//	This is the constructor.
//
//	Possible variables are:
// 	sAuthor 	= (TString) name of author
//	sSource 	= (TString) name of dark matter halo
// 	sCandidate 	= (TString) type of signal
JDDarkMatter::JDDarkMatter(TString author, TString source, TString candidate):
		sAuthor(author), sSource(source), sCandidate(candidate), dTheta(NULL),
		gJFactor(NULL), fEvaluateJFactorVsTheta(NULL), fEvaluateQFactorVsTheta(NULL)
//afegir totes les variables variable(NULL)
{
	    cout << endl;
		cout << endl;
		cout << "Constructor DM..." << endl;
		cout << endl;
		cout << endl;

	CreateFunctionsDM();
}

//-----------------------------------------------
//
//	This is the destructor.
JDDarkMatter::~JDDarkMatter()
{
	// delete everything

	if (dTheta)									delete dTheta;
	if (gJFactor)								delete gJFactor;
	if (fEvaluateJFactorVsTheta)				delete fEvaluateJFactorVsTheta;
	if (fEvaluateQFactorVsTheta)				delete fEvaluateQFactorVsTheta;


		cout << endl;
		cout << endl;
		cout << "Destructor DM..." << endl;
		cout << endl;
		cout << endl;

}

//-----------------------------------------------
//	This function creates the important functions of this class. The functions are:
//	SetJFactor() Let us choose which ReadJFactor use depending on the chosen author
//	fEvaluateJFactorVsTheta -> TF1 that evaluates the JFactor vs Theta; JFactor [~GeV,~cm]     theta [deg]
//	fEvaluateQFactorVsTheta -> TF1 that evaluates the QFactor vs Theta; QFactor [~GeV,~cm deg] theta [deg]
void JDDarkMatter::CreateFunctionsDM()
{

	SetJFactor(); // [Units]
	SetLOS();

	fEvaluateJFactorVsTheta = new TF1("fEvaluateJFactorVsTheta",this,&JDDarkMatter::TGraphEvaluateJFactorVsTheta,0.,dTheta,0,"JDDarkMatter","TGraphEvaluateJFactorVsTheta");
	fEvaluateQFactorVsTheta = new TF1("fEvaluateQFactorVsTheta", this, &JDDarkMatter::EvaluateQFactorVsTheta, 0., dTheta, 1, "JDDarkMatter", "EvaluateQFactorVsTheta");

	fEvaluateLOSVsTheta = new TF2("fEvaluateLOSVsTheta", this, &JDDarkMatter::EvaluateLOSVsTheta, 0., dTheta, -TMath::Pi(), TMath::Pi(), 0, "JDDarkMatter", "EvaluateLOSVsTheta");
	fEvaluateLOSPerSinusVsTheta = new TF2("fEvaluateLOSPerSinusVsTheta", this, &JDDarkMatter::EvaluateLOSPerSinusVsTheta, 0., dTheta, -TMath::Pi(), TMath::Pi(), 0, "JDDarkMatter", "EvaluateLOSPerSinusVsTheta");
	fEvaluateJFactorFromLOSVsTheta= new TF1("fEvaluateJFactorFromLOSVsTheta", this, &JDDarkMatter::EvaluateJFactorFromLOSVsTheta, 0., dTheta, 0, "JDDarkMatter", "EvaluateJFactorFromLOSVsTheta");
}

//-----------------------------------------------
//	This function creates the two ReadJFactor functions.
// 	It redirects to other function depending on which author we are taking into account.
void JDDarkMatter::SetJFactor()
{

	if (sAuthor == "Bonnivard")
	{
		ReadJFactorBonnivard();
	}

	else if (sAuthor == "Geringer")
	{
		ReadJFactorGeringer();
	}

	else
	{
		cout<<"ERROR: Author not valid"<<endl;
	}
}

//Bool_t / Int_t
// return 0 if succed
// return -1 else
//-----------------------------------------------
//	This function reads the JFactor data of Bonnivard
//	It fulfills a TGraph with this data
void JDDarkMatter::ReadJFactorBonnivard()
{

	Int_t contador = 0;

	gJFactor = new TGraph();

	Double_t dJ, dJ_m1, dJ_p1, dJ_m2, dJ_p2;
	Double_t exp, exp1;

		if (sCandidate == "Decay")
		{
			const TString myPath = "/home/david/Documents/DarkMatter/"+sAuthor+"/"+sSource+"_Dalphaint_cls_READ.output";
			exp = 1.;
			exp1 = 2.;
		}

		else if(sCandidate =="Annihilation")
		{
			const TString myPath = "/home/david/Documents/DarkMatter/"+sAuthor+"/"+sSource+"_Jalphaint_cls_READ.output";
			exp = 2.;
			exp1 = 5.;
		}

		else
		{
			cout<<"ERROR: Candidate not valid"<<endl;
			cout<<"Possibilities are: DECAY or ANNIHILATION"<<endl;
		}

		/////////////////////////
		//WARNING!!! FA UNA VOLTA DE MÉS!!!
		/////////////////////////
		ifstream file (myPath);
		while(!file.eof())
			{
				file >> dTheta >> dJ >> dJ_m1 >> dJ_p1 >> dJ_m2 >> dJ_p2;

				gJFactor->SetPoint(contador,dTheta,(dJ*(TMath::Power(SolarMass2GeV,exp)/TMath::Power(kpc2cm,exp1))));

				contadorMax=contador;

//				cout<< gJFactor->GetY()[contador]<<endl;
				contador ++;
			}

		cout<<contadorMax<<endl;

		file.close();
}

//-----------------------------------------------
//	This function reads the JFactor data of Geringer
//	It fulfills a TGraph with this data
void JDDarkMatter::ReadJFactorGeringer()
{
	Int_t contador = 0;

	gJFactor = new TGraph();

	TString name;
	Double_t LogJann2m, LogJann1m, LogJann, LogJann1p, LogJann2p;
	Double_t LogJdec2m, LogJdec1m, LogJdec, LogJdec1p, LogJdec2p;
	Double_t a,b,c,d,e,f,g,h,i,j;

	const TString myPath = "/home/david/Documents/DarkMatter/"+sAuthor+"/GeringerSamethTable_"+sSource+".txt";

	if (sCandidate == "Decay")
	{
		ifstream file (myPath);
		while(!file.eof())
			{
				file	>> name >> dTheta
						>> LogJann2m >> LogJann1m >> LogJann >> LogJann1p >> LogJann2p
						>> LogJdec2m >> LogJdec1m >> LogJdec >> LogJdec1p >> LogJdec2p
						>> a >> b >> c >> d >> e >> f >> g >> h >> i >> j;

				gJFactor->SetPoint(contador, dTheta, TMath::Power(10., LogJdec));

				contador ++;
			}
			file.close();
	}

	else if (sCandidate == "Annihilation")
	{
		ifstream file (myPath);
		while(!file.eof())
			{
				file	>> name >> dTheta
						>> LogJann2m >> LogJann1m >> LogJann >> LogJann1p >> LogJann2p
						>> LogJdec2m >> LogJdec1m >> LogJdec >> LogJdec1p >> LogJdec2p
						>> a >> b >> c >> d >> e >> f >> g >> h >> i >> j;

				gJFactor->SetPoint(contador, dTheta, TMath::Power(10., LogJann));

				contador ++;
			}
			file.close();	}

	else
	{
		cout<<"ERROR: Candidate is not valid"<<endl;
		cout<<"Possibilities are: DECAY or ANNIHILATION"<<endl;
	}
}

void JDDarkMatter::SetLOS()
{
	gLOS = new TGraph();

	Double_t* JX = (Double_t*)gJFactor->GetX();
	Double_t* JY = (Double_t*)gJFactor->GetY();


	cout<<"El valor del contador és:"<<contadorMax<<endl;
	for (Int_t t=0;t<contadorMax-1;t++)
		{
			gLOS->SetPoint(t,JX[t+1],(((JY[t+1]-JY[t])/(JX[t+1]-JX[t]))*(1/(2*TMath::Pi()*TMath::Sin(JX[t+1]*Deg2Rad)))));
		}
}


//-----------------------------------------------
// x[0] 	= dTheta
Double_t JDDarkMatter::TGraphEvaluateJFactorVsTheta(Double_t* x, Double_t* par)
{
 return gJFactor->Eval(x[0]);
}

//----------------------------------------------------
// x[0] 	= dTheta
// par[0] 	= point of normalization
Double_t JDDarkMatter::EvaluateQFactorVsTheta(Double_t* x, Double_t* par)
{
 return (gJFactor->Eval(x[0])/x[0])/(gJFactor->Eval(par[0])/par[0]);
}

//----------------------------------------------------
// Line of sight
// x[0]
Double_t JDDarkMatter::EvaluateLOSVsTheta(Double_t* x, Double_t* par)
{
	Double_t dcg = TMath::Power(4*TMath::Power(0,2)+TMath::Power(x[0],2)-2*2*0*x[0]*TMath::Cos(x[1]),0.5);

	return gLOS->Eval(dcg);
}

//----------------------------------------------------
// Line of sight
// x[0]
Double_t JDDarkMatter::EvaluateLOSPerSinusVsTheta(Double_t* x, Double_t* par)
{
	Double_t dcg = TMath::Power(4*TMath::Power(0,2)+TMath::Power(x[0],2)-2*2*0*x[0]*TMath::Cos(x[1]),0.5);

	Double_t thetaRad = x[0]*Deg2Rad;
	return  gLOS->Eval(dcg)*TMath::Sin(thetaRad);

}
//----------------------------------------------------
// Line of sight
// x[0]

Double_t JDDarkMatter::EvaluateJFactorFromLOSVsTheta(Double_t* x, Double_t* par)
{
	return fEvaluateLOSPerSinusVsTheta->Integral(0., x[0], -TMath::Pi(), TMath::Pi(),1e-6);
}



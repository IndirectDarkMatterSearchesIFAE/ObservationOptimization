//this works home
#include "../source/JDDarkMatter.cc"
#include "../source/JDInstrument.cc"
#include "../source/JDOptimization.cc"

//#include </home/david/Work/Software/root_v6.06.08/include/TStyle.h>
#include <TStyle.h>

//this works at IFAE
//#include </home/david/Work/Software/scripts/style.h>

#include <TLegend.h>
#include <TCanvas.h>
#include <TF1.h>


using namespace std;

//-------------------------------------
//  This function shows the JFactor of the author, source and candidate we want.
//	(QUIM) a definition of JFactor should be referenced (probably from Bertone?, miguel-angel?)
//	(QUIM) Declare author, source, candidate
void ShowJFactor() // (QUIM) Show-> Draw?
{

	TString author = "Bonnivard"; 		// (QUIM) Possible authors are "Bonnivard" and "Geringer"
	TString source = "uma2";			// (QUIM) List of possible sources?
	TString candidate = "Annihilation";	// (QUIM) Possible candidates are "Annihilation" and "Decay"

	TString mySourcePath;				// (QUIM) This variable should not be shown in here, should be
										// a private variable of the class (the user should not care)

	if (author == "Bonnivard")
	{
		if (candidate == "Decay")
		{
			// (QUIM) els paths haurien de ser relatius al codi que et descarregues
			mySourcePath = "/home/david/Documents/DarkMatter/"+author+"/"+source+"_Dalphaint_cls_READ.output";
		}

		else if (candidate == "Annihilation")
		{
			// (QUIM) els paths haurien de ser relatius al codi que l'usuari es descarregua
			mySourcePath = "/home/david/Documents/DarkMatter/"+author+"/"+source+"_Jalphaint_cls_READ.output";
		}

		else
		{
			cout<<"ERROR: Candidate is not valid"<<endl;
		}
	}

	else if (author == "Geringer")
	{
		// (QUIM) els paths haurien de ser relatius al codi que l'usuari es descarrega
		mySourcePath = "/home/david/Documents/DarkMatter/"+author+"/GeringerSamethTable_"+source+".txt";
	}

	else
	{
		cout<<"ERROR: Author is not valid"<<endl;
	}

	// (QUIM) some informatino could be given to explain what is each line doing, for example
	// "creating a class element that will read the values of J vs theta for a given author/source/candidate
	JDDarkMatter* 	JFactor = new JDDarkMatter(author, source, candidate, mySourcePath);
	// (QUIM) mySourcePath -> should be a private variable, and not be in the constructor


	// (QUIM) example: "this returns a TF1 with the values of J vs theta"
	TF1* functionJFactor = JFactor->GetTF1JFactorVsTheta();

	// (QUIM) example: "this returns a double ..., only for plotting purpose" [em sembla que el nom de theta no es el millor]
	// (QUIM) variables locals millor sempre en minuscula: Theta -> theta
	Double_t Theta = JFactor->GetTheta();

	functionJFactor->SetLineColor(2);
	functionJFactor->SetLineStyle(1);
	TCanvas* canvas = new TCanvas("canvas","",600,550);
	TH1I* dummy = new TH1I("dummy", author,1,0.01,Theta);
	dummy->SetMaximum(3.e20);
	dummy->SetMinimum(1.e15);
	dummy->SetStats(0);
	dummy->SetXTitle(" #theta ");
	// (QUIM) aquests if else tant facils millor així (més compactes):
	// 	hi hauria d'haver un "GetCandidate()"
	// if(GetCandidate() == "Annihilation") 	dummy->SetYTitle(" J Factor [GeV^2/cm^5]");
	// else if (candidate == "Decay") 	dummy->SetYTitle(" J Factor [GeV/cm^2]");
	if (candidate == "Annihilation")
	{
		dummy->SetYTitle(" J Factor [GeV^2/cm^5]");
	}
	else if (candidate == "Decay")
	{
		dummy->SetYTitle(" J Factor [GeV/cm^2]");
	}
	dummy->GetXaxis()->SetTitleOffset(1.3);
	dummy->GetYaxis()->SetTitleOffset(1.5);
	dummy->DrawCopy();
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionJFactor->Draw("same");

	TLegend* leg=new TLegend(.18,.75,.53,.85);
	leg->AddEntry(functionJFactor, source+"_"+candidate , "l");
	leg->SetFillColor(0);
	leg->SetLineColor(1);
	leg->SetBorderSize(1);
	leg->SetTextSize(0.037);
	leg->Draw();
	gPad->Modified();
	gPad->Update();

//	canvas->Print("/home/david/Documents/DarkMatter/Resultats/JFactor/"+author+"_"+candidate+"_"+source+".png");
}

//-------------------------------------
//  This function shows the LOS of the author, source and candidate we want.
//	(QUIM) a definition of los should be given in relation to JFactor, probably written in here and specifying the units
//	(QUIM) most of the comments of ShowJFactor apply in here
void ShowLOS()
{
	TString author = "Bonnivard";
	TString source ="uma2";
	TString candidate = "Annihilation";
	TString mySourcePath;

	// (QUIM) this piece of code is repeated, in ShowJFactor. As I was saying before, the user does not care at all...
	//------------------------------------------------------------------------------
	if (author == "Bonnivard")
	{
		if (candidate == "Decay")
		{
			mySourcePath = "/home/david/Documents/DarkMatter/"+author+"/"+source+"_Dalphaint_cls_READ.output";
		}

		else if (candidate == "Annihilation")
		{
			mySourcePath = "/home/david/Documents/DarkMatter/"+author+"/"+source+"_Jalphaint_cls_READ.output";
		}

		else
		{
			cout<<"ERROR: Candidate is not valid"<<endl;
		}
	}

	else if (author == "Geringer")
	{
		mySourcePath = "/home/david/Documents/DarkMatter/"+author+"/GeringerSamethTable_"+source+".txt";
	}

	else
	{
		cout<<"ERROR: Author is not valid"<<endl;
	}
	//------------------------------------------------------------------------------

	JDDarkMatter* 	LOS = new JDDarkMatter(author, source, candidate, mySourcePath);
	TF1* functionLOS = LOS->GetTF1LOSVsTheta();
	Double_t Theta = LOS->GetTheta();
	functionLOS->SetLineColor(2);
	functionLOS->SetLineStyle(1);

	TCanvas* canvas1 = new TCanvas("canvas1","",600,550);
	TH1I* dummy1 = new TH1I("dummy1",author ,1,0.01,Theta);
	dummy1->SetMaximum(3.e22);
	dummy1->SetMinimum(1.e17);
	dummy1->SetStats(0);
	dummy1->SetXTitle(" #theta "); //(QUIM) specify UNITS!!!
	if (candidate == "Annihilation")
		{
		dummy1->SetYTitle(" LOS ");

		}
	else if (candidate == "Decay")
		{
		dummy1->SetYTitle(" LOS");
		}
	dummy1->GetXaxis()->SetTitleOffset(1.3);
	dummy1->GetYaxis()->SetTitleOffset(1.5);
	dummy1->DrawCopy();
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionLOS->Draw("same");

	TLegend* leg1=new TLegend(.18,.75,.53,.85);
	leg1->AddEntry(functionLOS,source+"_"+candidate , "l");
	leg1->SetFillColor(0);
	leg1->SetLineColor(1);
	leg1->SetBorderSize(1);
	leg1->SetTextSize(0.037);
	leg1->Draw();
	gPad->Modified();
	gPad->Update();

//	canvas1->Print("/home/david/Documents/DarkMatter/Resultats/LOS/"+author+"_"+candidate+"_"+source+".png");

}

//-------------------------------------
// (QUIM): Rephrase... We need to define epsilon with an equation, and then explain how we obtain
// 	epsilon for each instrument
// (QUIM): Define instrumentName, wobble, myInstrumentPath
//
// This function shows the epsilon of the telescope, which expresses in a scale
// from 0 to 1 how good are the measures that it takes depending on the point of the camera
//
void ShowEpsilon()
{

	TString instrumentName= "MagicPointLikeEfficiency";	//(QUIM) The user name should be MAGICPointLike, Efficiency doesn't have a meaning for users
	Double_t wobble=1.;	// (QUIM) UNITS!
	TString sWobble = Form("%.1f",wobble);
	// (QUIM) Aixó hauria de ser un path relatiu a gitHup
	TString myInstrumentPath = "/home/david/Documents/DarkMatter/Epsilon/"+instrumentName+".txt";

	// (QUIM) "E"psilon majuscula...
	JDInstrument* Epsilon = new JDInstrument(instrumentName, wobble, myInstrumentPath);
	TF2* functionEpsilon = Epsilon->GetTF2EpsilonVsThetaAndPhi();
	Double_t Dcc = Epsilon->GetDcc();

	TCanvas* canvas2 = new TCanvas("canvas2","",600,550);
	TH2I* dummy2 = new TH2I("dummy2", "",1,0.,Dcc, 1, -TMath::Pi(), TMath::Pi());
	dummy2->SetMaximum(1.5);
	dummy2->SetMinimum(0.);
	dummy2->SetStats(0);
	dummy2->SetXTitle(" dcc ");
	dummy2->SetYTitle(" Epsilon");
	dummy2->GetXaxis()->SetTitleOffset(1.3);
	dummy2->GetYaxis()->SetTitleOffset(1.5);
	dummy2->DrawCopy();
//	gPad->SetLogx();
//	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionEpsilon->Draw("colz");

	TLegend* leg2=new TLegend(.18,.75,.66,.85);
	leg2->AddEntry(functionEpsilon, instrumentName , "l");
	leg2->SetFillColor(0);
	leg2->SetLineColor(1);
	leg2->SetBorderSize(1);
	leg2->SetTextSize(0.037);
	leg2->Draw();
	gPad->Modified();
	gPad->Update();

//	canvas2->Print("/home/david/Documents/DarkMatter/Resultats/Epsilon/"+instrumentName+"_Wobble:"+sWobble+".png");

}

//-------------------------------------
//This function shows the efficiency of the telescope, which is the integral of the epsilon multiplied by theta
//
void ShowEfficiency()
{
	TString instrumentName= "MagicPointLikeEfficiency";
	Double_t wobble=1.;
	TString sWobble = Form("%.1f",wobble);
	TString myInstrumentPath = "/home/david/Documents/DarkMatter/Epsilon/"+instrumentName+".txt";

	JDInstrument* Efficiency = new JDInstrument(instrumentName, wobble, myInstrumentPath);
	TF1* functionEfficiency = Efficiency->GetTF1EfficiencyVsTheta();
	Double_t TethaMax = Efficiency->GetThetaMax();

	functionEfficiency->SetLineColor(2);
	functionEfficiency->SetLineStyle(1);

	TCanvas* canvas3 = new TCanvas("canvas3","",600,550);
	TH1I* dummy3 = new TH1I("dummy3","",1,0.,TethaMax);
	dummy3->SetMaximum(1.5);
	dummy3->SetMinimum(0.);
	dummy3->SetStats(0);
	dummy3->SetXTitle(" #theta ");
	dummy3->SetYTitle(" Efficiency ");
	dummy3->GetXaxis()->SetTitleOffset(1.3);
	dummy3->GetYaxis()->SetTitleOffset(1.5);
	dummy3->DrawCopy();
//	gPad->SetLogx();
//	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionEfficiency->Draw("same");

	TLegend* leg3=new TLegend(.18,.75,.66,.85);
	leg3->AddEntry(functionEfficiency, instrumentName, "l");
	leg3->SetFillColor(0);
	leg3->SetLineColor(1);
	leg3->SetBorderSize(1);
	leg3->SetTextSize(0.037);
	leg3->Draw();
	gPad->Modified();
	gPad->Update();

//	canvas3->Print("/home/david/Documents/DarkMatter/Resultats/Eficiència/"+instrumentName+"_Wobble:"+sWobble+".png");
}

//-------------------------------------
//  This function shows the QFactor of the author, source and candidate we want.
//
void ShowQFactor()
{
	TString author = "Bonnivard";
	TString source ="uma2";
	TString candidate = "Annihilation";
	TString mySourcePath;
	TString instrumentName= "MagicPointLikeEfficiency";
	Double_t wobble=1.;
	TString myInstrumentPath= "/home/david/Documents/DarkMatter/Epsilon/"+instrumentName+".txt";

	TString sWobble = Form("%.1f",wobble);
	Double_t normalizationPoint = 0.1;

	if (author == "Bonnivard")
	{
		if (candidate == "Decay")
		{
			mySourcePath = "/home/david/Documents/DarkMatter/"+author+"/"+source+"_Dalphaint_cls_READ.output";
		}

		else if (candidate == "Annihilation")
		{
			mySourcePath = "/home/david/Documents/DarkMatter/"+author+"/"+source+"_Jalphaint_cls_READ.output";
		}

		else
		{
			cout<<"ERROR: Candidate is not valid"<<endl;
		}
	}

	else if (author == "Geringer")
	{
		mySourcePath = "/home/david/Documents/DarkMatter/"+author+"/GeringerSamethTable_"+source+".txt";
	}

	else
	{
		cout<<"ERROR: Author is not valid"<<endl;
	}

	JDOptimization* QFactor = new JDOptimization(author, source, candidate, mySourcePath, instrumentName, wobble, myInstrumentPath);
	// (QUIM) cambiar "normalizationPoint" -> "normalizationFactor"; així podem comparar differents QFactors.
	// normalizationFactor hauria de ser una variable de la classe JDOptimization, amb valor per defecte 1. Hi hauria d'aver un
	// Setter (per canviar-li el valor si es vol) i un Getter
	TF1* functionQFactor= QFactor->GetTF1QFactorVsTheta(normalizationPoint);
	Double_t TethaMax = QFactor->GetThetaMax();

	functionQFactor->SetLineColor(2);
	functionQFactor->SetLineStyle(1);

	TCanvas* canvas4 = new TCanvas("canvas4","",600,550);
	TH1I* dummy4 = new TH1I("dummy4",author ,1,0.01,TethaMax);
	dummy4->SetMaximum(3.);
	dummy4->SetMinimum(0.);
	dummy4->SetStats(0);
	dummy4->SetXTitle(" #theta ");
	if (candidate == "Annihilation")
		{
			dummy4->SetYTitle(" Q Factor [GeV^2/cm^5 #theta]");
		}
	else if (candidate == "Decay")
		{
			dummy4->SetYTitle(" Q Factor [GeV/cm^2 #theta]");
		}
	dummy4->GetXaxis()->SetTitleOffset(1.3);
	dummy4->GetYaxis()->SetTitleOffset(1.5);
	dummy4->DrawCopy();
	gPad->SetLogx();
//	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionQFactor->Draw("same");

	TLegend* leg4=new TLegend(.18,.75,.53,.85);
	leg4->AddEntry(functionQFactor,source+"_"+candidate , "l");
	leg4->SetFillColor(0);
	leg4->SetLineColor(1);
	leg4->SetBorderSize(1);
	leg4->SetTextSize(0.037);
	leg4->Draw();
	gPad->Modified();
	gPad->Update();

//	canvas4->Print("/home/david/Documents/DarkMatter/Resultats/QFactor/"+author+"_"+candidate+"_"+source+".png");

}

//-------------------------------------
//  This function shows the JFactor calculated from the LOS in the "signal zone" of the author, source and candidate we want.
//
void ShowJFactorFromLOSOn()
{
	TString author = "Bonnivard";
	TString source ="uma2";
	TString candidate = "Annihilation";
	TString mySourcePath;
	TString instrumentName= "MagicPointLikeEfficiency";
	Double_t wobble=1.;
	TString myInstrumentPath= "/home/david/Documents/DarkMatter/Epsilon/"+instrumentName+".txt";

	TString sWobble = Form("%.1f",wobble);

	if (author == "Bonnivard")
	{
		if (candidate == "Decay")
		{
			mySourcePath = "/home/david/Documents/DarkMatter/"+author+"/"+source+"_Dalphaint_cls_READ.output";
		}

		else if (candidate == "Annihilation")
		{
			mySourcePath = "/home/david/Documents/DarkMatter/"+author+"/"+source+"_Jalphaint_cls_READ.output";
		}

		else
		{
			cout<<"ERROR: Candidate is not valid"<<endl;
		}
	}

	else if (author == "Geringer")
	{
		mySourcePath = "/home/david/Documents/DarkMatter/"+author+"/GeringerSamethTable_"+source+".txt";
	}

	else
	{
		cout<<"ERROR: Author is not valid"<<endl;
	}

	JDOptimization* JFactorFromLOSOn = new JDOptimization(author, source, candidate, mySourcePath, instrumentName, wobble, myInstrumentPath);
	TF1* functionJFactorFromLOSOn = JFactorFromLOSOn->GetTF1JFactorFromLOS_OnVsTheta();
	Double_t TethaMax = JFactorFromLOSOn->GetThetaMax();

	functionJFactorFromLOSOn->SetLineColor(2);
	functionJFactorFromLOSOn->SetLineStyle(1);

	TCanvas* canvas5 = new TCanvas("canvas5","",600,550);
	TH1I* dummy5 = new TH1I("dummy5", author,1,0.01,TethaMax);
	dummy5->SetMaximum(3.e20);
	dummy5->SetMinimum(1.e15);
	dummy5->SetStats(0);
	dummy5->SetXTitle(" #theta ");
	if (candidate == "Annihilation")
		{
			dummy5->SetYTitle(" J Factor On [GeV^2/cm^5]");
		}
	else if (candidate == "Decay")
		{
			dummy5->SetYTitle(" J Factor On [GeV/cm^2]");
		}
	dummy5->GetXaxis()->SetTitleOffset(1.3);
	dummy5->GetYaxis()->SetTitleOffset(1.5);
	dummy5->DrawCopy();
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionJFactorFromLOSOn->Draw("same");

	TLegend* leg5=new TLegend(.18,.75,.66,.85);
	leg5->AddEntry(functionJFactorFromLOSOn, source+"_"+candidate, "l");
	leg5->SetFillColor(0);
	leg5->SetLineColor(1);
	leg5->SetBorderSize(1);
	leg5->SetTextSize(0.037);
	leg5->Draw();
	gPad->Modified();
	gPad->Update();

//	canvas5->Print("/home/david/Documents/DarkMatter/Resultats/JFactorFromLOS_On/"+author+"_"+candidate+"_"+source+".png");

}

//-------------------------------------
//  This function shows the JFactor calculated from the LOS in the "background zone" of the author, source and candidate we want.
//
void ShowJFactorFromLOSOff()
{
	TString author = "Bonnivard";
	TString source ="uma2";
	TString candidate = "Annihilation";
	TString mySourcePath;
	TString instrumentName= "MagicPointLikeEfficiency";
	Double_t wobble=1.;
	TString myInstrumentPath= "/home/david/Documents/DarkMatter/Epsilon/"+instrumentName+".txt";

	TString sWobble = Form("%.1f",wobble);

	if (author == "Bonnivard")
	{
		if (candidate == "Decay")
		{
			mySourcePath = "/home/david/Documents/DarkMatter/"+author+"/"+source+"_Dalphaint_cls_READ.output";
		}

		else if (candidate == "Annihilation")
		{
			mySourcePath = "/home/david/Documents/DarkMatter/"+author+"/"+source+"_Jalphaint_cls_READ.output";
		}

		else
		{
			cout<<"ERROR: Candidate is not valid"<<endl;
		}
	}

	else if (author == "Geringer")
	{
		mySourcePath = "/home/david/Documents/DarkMatter/"+author+"/GeringerSamethTable_"+source+".txt";
	}

	else
	{
		cout<<"ERROR: Author is not valid"<<endl;
	}

	JDOptimization* JFactorFromLOSOff = new JDOptimization(author, source, candidate, mySourcePath, instrumentName, wobble, myInstrumentPath);
	TF1* functionJFactorFromLOSOff = JFactorFromLOSOff->GetTF1JFactorFromLOS_OffVsTheta();
	Double_t TethaMax = JFactorFromLOSOff->GetThetaMax();

	functionJFactorFromLOSOff->SetLineColor(2);
	functionJFactorFromLOSOff->SetLineStyle(1);

	TCanvas* canvas6 = new TCanvas("canvas6","",600,550);
	TH1I* dummy6 = new TH1I("dummy6","",1,0.01,TethaMax);
	dummy6->SetMaximum(3.e20);
	dummy6->SetMinimum(1.e15);
	dummy6->SetStats(0);
	dummy6->SetXTitle(" #theta ");
	if (candidate == "Annihilation")
		{
			dummy6->SetYTitle(" J Factor Off [GeV^2/cm^5]");
		}
	else if (candidate == "Decay")
		{
			dummy6->SetYTitle(" J Factor Off [GeV/cm^2]");
		}
	dummy6->GetXaxis()->SetTitleOffset(1.3);
	dummy6->GetYaxis()->SetTitleOffset(1.5);
	dummy6->DrawCopy();
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionJFactorFromLOSOff->Draw("same");

	TLegend* leg6=new TLegend(.18,.75,.66,.85);
	leg6->AddEntry(functionJFactorFromLOSOff, source+"_"+candidate, "l");
	leg6->SetFillColor(0);
	leg6->SetLineColor(1);
	leg6->SetBorderSize(1);
	leg6->SetTextSize(0.037);
	leg6->Draw();
	gPad->Modified();
	gPad->Update();

//	canvas6->Print("/home/david/Documents/DarkMatter/Resultats/JFactorFromLOS_Off/"+author+"_"+candidate+"_"+source+".png");

}

//-------------------------------------
//  This function shows the total JFactor calculated from the LOS of the author, source and candidate we want.
//
void ShowJFactorFromLOSTotal()
{
	TString author = "Bonnivard";
	TString source ="uma2";
	TString candidate = "Annihilation";
	TString mySourcePath;
	TString instrumentName= "MagicPointLikeEfficiency";
	Double_t wobble=1.;
	TString myInstrumentPath= "/home/david/Documents/DarkMatter/Epsilon/"+instrumentName+".txt";

	TString sWobble = Form("%.1f",wobble);

	if (author == "Bonnivard")
	{
		if (candidate == "Decay")
		{
			mySourcePath = "/home/david/Documents/DarkMatter/"+author+"/"+source+"_Dalphaint_cls_READ.output";
		}

		else if (candidate == "Annihilation")
		{
			mySourcePath = "/home/david/Documents/DarkMatter/"+author+"/"+source+"_Jalphaint_cls_READ.output";
		}

		else
		{
			cout<<"ERROR: Candidate is not valid"<<endl;
		}
	}

	else if (author == "Geringer")
	{
		mySourcePath = "/home/david/Documents/DarkMatter/"+author+"/GeringerSamethTable_"+source+".txt";
	}

	else
	{
		cout<<"ERROR: Author is not valid"<<endl;
	}

	JDOptimization* JFactorFromLOSTotal = new JDOptimization(author, source, candidate, mySourcePath, instrumentName, wobble, myInstrumentPath);
	TF1* functionJFactorFromLOSTotal = JFactorFromLOSTotal->GetTF1JFactorFromLOS_TotalVsTheta();
	Double_t TethaMax = JFactorFromLOSTotal->GetThetaMax();

	functionJFactorFromLOSTotal->SetLineColor(2);
	functionJFactorFromLOSTotal->SetLineStyle(1);

	TCanvas* canvas7 = new TCanvas("canvas7","",600,550);
	TH1I* dummy7 = new TH1I("dummy7","",1,0.01,TethaMax);
	dummy7->SetMaximum(3.e20);
	dummy7->SetMinimum(1.e15);
	dummy7->SetStats(0);
	dummy7->SetXTitle(" #theta ");
	if (candidate == "Annihilation")
		{
			dummy7->SetYTitle(" J Factor Total [GeV^2/cm^5]");
		}
	else if (candidate == "Decay")
		{
			dummy7->SetYTitle(" J Factor Total [GeV/cm^2]");
		}
	dummy7->GetXaxis()->SetTitleOffset(1.3);
	dummy7->GetYaxis()->SetTitleOffset(1.5);
	dummy7->DrawCopy();
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionJFactorFromLOSTotal->Draw("same");

	TLegend* leg7=new TLegend(.18,.75,.66,.85);
	leg7->AddEntry(functionJFactorFromLOSTotal, source+"_"+candidate, "l");
	leg7->SetFillColor(0);
	leg7->SetLineColor(1);
	leg7->SetBorderSize(1);
	leg7->SetTextSize(0.037);
	leg7->Draw();
	gPad->Modified();
	gPad->Update();

//	canvas7->Print("/home/david/Documents/DarkMatter/Resultats/JFactorFromLOS_Total/"+author+"_"+candidate+"_"+source+".png");
}

//-------------------------------------
//  This function shows the total QFactor calculated from the LOS of the author, source and candidate we want.
//
void ShowQFactorFromLOSTotal()
{
	TString author = "Bonnivard";
	TString source ="uma2";
	TString candidate = "Annihilation";
	TString mySourcePath;
	TString instrumentName= "MagicPointLikeEfficiency";
	Double_t wobble=1.;
	TString myInstrumentPath= "/home/david/Documents/DarkMatter/Epsilon/"+instrumentName+".txt";

	TString sWobble = Form("%.1f",wobble);
	Double_t normalizationPoint = 0.1;


	if (author == "Bonnivard")
	{
		if (candidate == "Decay")
		{
			mySourcePath = "/home/david/Documents/DarkMatter/"+author+"/"+source+"_Dalphaint_cls_READ.output";
		}

		else if (candidate == "Annihilation")
		{
			mySourcePath = "/home/david/Documents/DarkMatter/"+author+"/"+source+"_Jalphaint_cls_READ.output";
		}

		else
		{
			cout<<"ERROR: Candidate is not valid"<<endl;
		}
	}

	else if (author == "Geringer")
	{
		mySourcePath = "/home/david/Documents/DarkMatter/"+author+"/GeringerSamethTable_"+source+".txt";
	}

	else
	{
		cout<<"ERROR: Author is not valid"<<endl;
	}

	JDOptimization* QFactorFromLOSTotal = new JDOptimization(author, source, candidate, mySourcePath, instrumentName, wobble, myInstrumentPath);
	TF1* functionQFactorFromLOSTotal = QFactorFromLOSTotal->GetTF1QFactorFromLOS_TotalVsTheta(normalizationPoint);
	Double_t TethaMax = QFactorFromLOSTotal->GetThetaMax();

	functionQFactorFromLOSTotal->SetLineColor(2);
	functionQFactorFromLOSTotal->SetLineStyle(1);

	TCanvas* canvas8 = new TCanvas("canvas8","",600,550);
	TH1I* dummy8 = new TH1I("dummy8", author,1,0.01,TethaMax);
	dummy8->SetMaximum(3.);
	dummy8->SetMinimum(0.);
	dummy8->SetStats(0);
	dummy8->SetXTitle(" #theta ");
	if (candidate == "Annihilation")
		{
			dummy8->SetYTitle(" Q Factor Total [GeV^2/cm^5 #theta]");
		}
	else if (candidate == "Decay")
		{
			dummy8->SetYTitle(" Q Factor Total [GeV/cm^2 #theta]");
		}
	dummy8->GetXaxis()->SetTitleOffset(1.3);
	dummy8->GetYaxis()->SetTitleOffset(1.5);
	dummy8->DrawCopy();
	gPad->SetLogx();
//	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionQFactorFromLOSTotal->Draw("same");

	TLegend* leg8=new TLegend(.18,.75,.66,.85);
	leg8->AddEntry(functionQFactorFromLOSTotal, source+"_"+candidate, "l");
	leg8->SetFillColor(0);
	leg8->SetLineColor(1);
	leg8->SetBorderSize(1);
	leg8->SetTextSize(0.037);
	leg8->Draw();
	gPad->Modified();
	gPad->Update();

//	canvas8->Print("/home/david/Documents/DarkMatter/Resultats/QFactorFromLOS_Total/"+author+"_"+candidate+"_"+source+".png");
}

//-------------------------------------
//  This function shows the total JFactor multiplied by the efficiency of the camera of the author, source, candidate and instrument we want.
//
void ShowJFactorEffective()
{
	TString author = "Bonnivard";
	TString source ="uma2";
	TString candidate = "Annihilation";
	TString mySourcePath;
	TString instrumentName= "MagicPointLikeEfficiency";
	Double_t wobble=1.;
	TString myInstrumentPath= "/home/david/Documents/DarkMatter/Epsilon/"+instrumentName+".txt";

	TString sWobble = Form("%.1f",wobble);

	if (author == "Bonnivard")
	{
		if (candidate == "Decay")
		{
			mySourcePath = "/home/david/Documents/DarkMatter/"+author+"/"+source+"_Dalphaint_cls_READ.output";
		}

		else if (candidate == "Annihilation")
		{
			mySourcePath = "/home/david/Documents/DarkMatter/"+author+"/"+source+"_Jalphaint_cls_READ.output";
		}

		else
		{
			cout<<"ERROR: Candidate is not valid"<<endl;
		}
	}

	else if (author == "Geringer")
	{
		mySourcePath = "/home/david/Documents/DarkMatter/"+author+"/GeringerSamethTable_"+source+".txt";
	}

	else
	{
		cout<<"ERROR: Author is not valid"<<endl;
	}

	JDOptimization* JFactorEffective = new JDOptimization(author, source, candidate, mySourcePath, instrumentName, wobble, myInstrumentPath);
	TF1* functionJFactorEffective = JFactorEffective->GetTF1JFactorEffectiveVsTheta();
	Double_t TethaMax = JFactorEffective->GetThetaMax();

	functionJFactorEffective->SetLineColor(2);
	functionJFactorEffective->SetLineStyle(1);

	TCanvas* canvas9 = new TCanvas("canvas9","",600,550);
	TH1I* dummy9 = new TH1I("dummy9", author+"_"+instrumentName,1,0.01,TethaMax);
	dummy9->SetMaximum(3.e20);
	dummy9->SetMinimum(1.e15);
	dummy9->SetStats(0);
	dummy9->SetXTitle(" #theta ");
	if (candidate == "Annihilation")
		{
			dummy9->SetYTitle(" J Factor Effective [GeV^2/cm^5]");
		}
	else if (candidate == "Decay")
		{
			dummy9->SetYTitle(" J Factor Effective [GeV/cm^2]");
		}
	dummy9->GetXaxis()->SetTitleOffset(1.3);
	dummy9->GetYaxis()->SetTitleOffset(1.5);
	dummy9->DrawCopy();
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionJFactorEffective->Draw("same");

	TLegend* leg9=new TLegend(.18,.75,.66,.85);
	leg9->AddEntry(functionJFactorEffective, source+"_"+candidate, "l");
	leg9->SetFillColor(0);
	leg9->SetLineColor(1);
	leg9->SetBorderSize(1);
	leg9->SetTextSize(0.037);
	leg9->Draw();
	gPad->Modified();
	gPad->Update();

//	canvas9->Print("/home/david/Documents/DarkMatter/Resultats/JFactorEffective/"+author+"_"+candidate+"_"+source+instrumentName+"_Wobble:"+sWobble+".png");
}

//-------------------------------------
//  This function shows the total QFactor multiplied by the efficiency of the camera of the author, source, candidate and instrument we want.
//
void ShowQFactorEffective()
{
	TString author = "Bonnivard";
	TString source ="uma2";
	TString candidate = "Annihilation";
	TString mySourcePath;
	TString instrumentName= "MagicPointLikeEfficiency";
	Double_t wobble=1.;
	TString myInstrumentPath= "/home/david/Documents/DarkMatter/Epsilon/"+instrumentName+".txt";

	TString sWobble = Form("%.1f",wobble);
	Double_t normalizationPoint = 0.1;

	if (author == "Bonnivard")
	{
		if (candidate == "Decay")
		{
			mySourcePath = "/home/david/Documents/DarkMatter/"+author+"/"+source+"_Dalphaint_cls_READ.output";
		}

		else if (candidate == "Annihilation")
		{
			mySourcePath = "/home/david/Documents/DarkMatter/"+author+"/"+source+"_Jalphaint_cls_READ.output";
		}

		else
		{
			cout<<"ERROR: Candidate is not valid"<<endl;
		}
	}

	else if (author == "Geringer")
	{
		mySourcePath = "/home/david/Documents/DarkMatter/"+author+"/GeringerSamethTable_"+source+".txt";
	}

	else
	{
		cout<<"ERROR: Author is not valid"<<endl;
	}

	JDOptimization* QFactorEffective = new JDOptimization(author, source, candidate, mySourcePath, instrumentName, wobble, myInstrumentPath);
	TF1* functionQFactorEffective = QFactorEffective->GetTF1QFactorEffectiveVsTheta(normalizationPoint);
	Double_t TethaMax = QFactorEffective->GetThetaMax();

	functionQFactorEffective->SetLineColor(2);
	functionQFactorEffective->SetLineStyle(1);

	TCanvas* canvas10 = new TCanvas("canvas10","",600,550);
	TH1I* dummy10 = new TH1I("dummy10", author+"_"+instrumentName,1,0.01,TethaMax);
	dummy10->SetMaximum(3.);
	dummy10->SetMinimum(0.);
	dummy10->SetStats(0);
	dummy10->SetXTitle(" #theta ");
	if (candidate == "Annihilation")
		{
			dummy10->SetYTitle(" Q Factor Effective [GeV^2/cm^5 #theta]");
		}
	else if (candidate == "Decay")
		{
			dummy10->SetYTitle(" Q Factor Effective [GeV/cm^2 #theta]");
		}
	dummy10->GetXaxis()->SetTitleOffset(1.3);
	dummy10->GetYaxis()->SetTitleOffset(1.5);
	dummy10->DrawCopy();
	gPad->SetLogx();
//	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionQFactorEffective->Draw("same");

	TLegend* leg10=new TLegend(.18,.75,.66,.85);
	leg10->AddEntry(functionQFactorEffective, source+"_"+candidate, "l");
	leg10->SetFillColor(0);
	leg10->SetLineColor(1);
	leg10->SetBorderSize(1);
	leg10->SetTextSize(0.037);
	leg10->Draw();
	gPad->Modified();
	gPad->Update();

//	canvas10->Print("/home/david/Documents/DarkMatter/Resultats/QFactorEffective/"+author+"_"+candidate+"_"+source+instrumentName+"_Wobble:"+sWobble+".png");
}

void RunExample1()
{
///////////////////////////////////////////////////////////////
//  Dark Matter Class
//////////////////////////////////////////////////////////////
	ShowJFactor();  		// (QUIM)-> checked!
	ShowLOS();				// (QUIM)-> checked!

///////////////////////////////////////////////////////////////
//  Instrument Class
////////////////////////////////////////////////////////////////
	ShowEpsilon();			// (QUIM)-> checked!
	ShowEfficiency();		// (QUIM)-> checked!

///////////////////////////////////////////////////////////////
//  Combined Class
////////////////////////////////////////////////////////////////
	ShowQFactor();
	ShowJFactorFromLOSOn();
	ShowJFactorFromLOSOff();
	ShowJFactorFromLOSTotal();
	ShowQFactorFromLOSTotal();
	ShowJFactorEffective();
	ShowQFactorEffective();
}

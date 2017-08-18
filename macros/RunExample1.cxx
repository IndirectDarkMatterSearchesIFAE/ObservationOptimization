//this works home
#include "../source/JDDarkMatter.cc"
#include "../source/JDInstrument.cc"
#include "../source/JDOptimization.cc"

#include </home/david/Work/Software/root_v6.06.08/include/TStyle.h>

//this works at IFAE
//#include </home/david/Work/Software/scripts/style.h>

#include <TLegend.h>
#include <TCanvas.h>
#include <TF1.h>


using namespace std;

//-------------------------------------
//  This function shows the JFactor of the author, source and candidate we want.
//
void ShowJFactor()
{

	TString author = "Bonnivard";
	TString source = "uma2";
	TString candidate = "Annihilation";
	TString mySourcePath;

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

	JDDarkMatter* 	JFactor = new JDDarkMatter(author, source, candidate, mySourcePath);
	TF1* functionJFactor = JFactor->GetTF1JFactorVsTheta();
	Double_t Theta = JFactor->GetTheta();

	cout<<"Theta:0.01"<<endl;
	cout<<"JFactor:"<<functionJFactor->Eval(0.01)<<endl;
	cout<<"Theta:0.1"<<endl;
	cout<<"JFactor:"<<functionJFactor->Eval(0.1)<<endl;
	cout<<"Theta:0.5"<<endl;
	cout<<"JFactor:"<<functionJFactor->Eval(0.5)<<endl;
	cout<<"Theta:1."<<endl;
	cout<<"JFactor:"<<functionJFactor->Eval(1.)<<endl;
	cout<<"Theta:5."<<endl;
	cout<<"JFactor:"<<functionJFactor->Eval(5.)<<endl;
	cout<<"Theta:10."<<endl;
	cout<<"JFactor:"<<functionJFactor->Eval(10.)<<endl;

	functionJFactor->SetLineColor(2);
	functionJFactor->SetLineStyle(1);
	TCanvas* canvas = new TCanvas("canvas","",600,550);
	TH1I* dummy = new TH1I("dummy", author,1,0.01,Theta);
	dummy->SetMaximum(3.e20);
	dummy->SetMinimum(1.e15);
	dummy->SetStats(0);
	dummy->SetXTitle(" #theta ");
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
//
void ShowLOS()
{
	TString author = "Bonnivard";
	TString source ="uma2";
	TString candidate = "Annihilation";
	TString mySourcePath;

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

	JDDarkMatter* 	LOS = new JDDarkMatter(author, source, candidate, mySourcePath);
	TF1* functionLOS = LOS->GetTF1LOSVsTheta();
	Double_t Theta = LOS->GetTheta();
	functionLOS->SetLineColor(2);
	functionLOS->SetLineStyle(1);

	cout<<functionLOS->Eval(0.2)<<endl;

	TCanvas* canvas2 = new TCanvas("canvas2","",600,550);
	TH1I* dummy2 = new TH1I("dummy2",author ,1,0.01,Theta);
	dummy2->SetMaximum(3.e22);
	dummy2->SetMinimum(1.e17);
	dummy2->SetStats(0);
	dummy2->SetXTitle(" #theta ");
	if (candidate == "Annihilation")
		{
		dummy2->SetYTitle(" LOS ");

		}
		else if (candidate == "Decay")
		{
			dummy2->SetYTitle(" LOS");
		}
	dummy2->GetXaxis()->SetTitleOffset(1.3);
	dummy2->GetYaxis()->SetTitleOffset(1.5);
	dummy2->DrawCopy();
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionLOS->Draw("same");

	TLegend* leg2=new TLegend(.18,.75,.53,.85);
	leg2->AddEntry(functionLOS,source+"_"+candidate , "l");
	leg2->SetFillColor(0);
	leg2->SetLineColor(1);
	leg2->SetBorderSize(1);
	leg2->SetTextSize(0.037);
	leg2->Draw();
	gPad->Modified();
	gPad->Update();

//	canvas2->Print("/home/david/Documents/DarkMatter/Resultats/LOS/"+author+"_"+candidate+"_"+source+".png");

}

//-------------------------------------
// This function shows the epsilon of the telescope, which expresses in a scale from 0 to 1 how good are the measures that it takes depending on the point of the camera
//
void ShowEpsilon()
{

	TString instrumentName= "MagicIdealEfficiency";
	Double_t wobble=0.;
	TString sWobble = Form("%.1f",wobble);
	TString myInstrumentPath = "/home/david/Documents/DarkMatter/Epsilon/"+instrumentName+".txt";

	JDInstrument* Epsilon = new JDInstrument(instrumentName, wobble, myInstrumentPath);
	TF2* functionEpsilon = Epsilon->GetTF2EpsilonVsThetaAndPhi();
	Double_t Dcc = Epsilon->GetDcc();

	TCanvas* canvas4 = new TCanvas("canvas4","",600,550);
	TH2I* dummy4 = new TH2I("dummy4", "",1,0.,Dcc, 1, -TMath::Pi(), TMath::Pi());
	dummy4->SetMaximum(1.5);
	dummy4->SetMinimum(0.);
	dummy4->SetStats(0);
	dummy4->SetXTitle(" dcc ");
	dummy4->SetYTitle(" Epsilon");
	dummy4->GetXaxis()->SetTitleOffset(1.3);
	dummy4->GetYaxis()->SetTitleOffset(1.5);
	dummy4->DrawCopy();
//	gPad->SetLogx();
//	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionEpsilon->Draw("colz");

	TLegend* leg4=new TLegend(.18,.75,.66,.85);
	leg4->AddEntry(functionEpsilon, instrumentName , "l");
	leg4->SetFillColor(0);
	leg4->SetLineColor(1);
	leg4->SetBorderSize(1);
	leg4->SetTextSize(0.037);
	leg4->Draw();
	gPad->Modified();
	gPad->Update();

//	canvas4->Print("/home/david/Documents/DarkMatter/Resultats/Epsilon/"+instrumentName+"_Wobble:"+sWobble+".png");

}

//-------------------------------------
//This function shows the efficiency of the telescope, which is the integral of the epsilon multiplied by theta
//
void ShowEfficiency()
{
	TString instrumentName= "CrabNebula,Post-upgrade";
	Double_t wobble=0.;
	TString sWobble = Form("%.1f",wobble);
	TString myInstrumentPath = "/home/david/Documents/DarkMatter/Epsilon/"+instrumentName+".txt";

	JDInstrument* Efficiency = new JDInstrument(instrumentName, wobble, myInstrumentPath);
	TF1* functionEfficiency = Efficiency->GetTF1EfficiencyVsTheta();
	Double_t TethaMax = Efficiency->GetThetaMax();

	// We declare here the name of the ...
	functionEfficiency->SetLineColor(2);
	functionEfficiency->SetLineStyle(1);
	TCanvas* canvas5 = new TCanvas("canvas5","",600,550);
	TH1I* dummy5 = new TH1I("dummy5","",1,0.,TethaMax);
	dummy5->SetMaximum(1.5);
	dummy5->SetMinimum(0.);
	dummy5->SetStats(0);
	dummy5->SetXTitle(" #theta ");
	dummy5->SetYTitle(" Efficiency ");
	dummy5->GetXaxis()->SetTitleOffset(1.3);
	dummy5->GetYaxis()->SetTitleOffset(1.5);
	dummy5->DrawCopy();
//	gPad->SetLogx();
//	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionEfficiency->Draw("same");

	TLegend* leg5=new TLegend(.18,.75,.66,.85);
	leg5->AddEntry(functionEfficiency, instrumentName, "l");
	leg5->SetFillColor(0);
	leg5->SetLineColor(1);
	leg5->SetBorderSize(1);
	leg5->SetTextSize(0.037);
	leg5->Draw();
	gPad->Modified();
	gPad->Update();

//	canvas5->Print("/home/david/Documents/DarkMatter/Resultats/Eficiència/"+instrumentName+"_Wobble:"+sWobble+".png");
}

//-------------------------------------
//  This function shows the QFactor of the author, source and candidate we want.
//
void ShowQFactor()
{
	TString author = "Bonnivard";
	TString source ="uma2";
	TString candidate = "Annihilation";
	Double_t normalizationPoint = 0.1;
	TString mySourcePath;
	TString instrumentName= "MagicPointLikeEfficiency";
	Double_t wobble=1.;
	TString sWobble = Form("%.1f",wobble);
	TString myInstrumentPath= "/home/david/Documents/DarkMatter/Epsilon/"+instrumentName+".txt";

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
//	QFactor->~JDDarkMatter();
	TF1* functionQFactor= QFactor->GetTF1QFactorVsTheta(normalizationPoint);
	Double_t TethaMax = QFactor->GetThetaMax();

	cout<<"Q Factor:"<<endl;
	cout<<functionQFactor->Eval(0.2)<<endl;

	functionQFactor->SetLineColor(2);
	functionQFactor->SetLineStyle(1);
	TCanvas* canvas1 = new TCanvas("canvas1","",600,550);
	TH1I* dummy1 = new TH1I("dummy1",author ,1,0.01,TethaMax);
	dummy1->SetMaximum(3.);
	dummy1->SetMinimum(0.);
	dummy1->SetStats(0);
	dummy1->SetXTitle(" #theta ");
	if (candidate == "Annihilation")
		{
			dummy1->SetYTitle(" Q Factor [GeV^2/cm^5 #theta]");

		}
		else if (candidate == "Decay")
		{
			dummy1->SetYTitle(" Q Factor [GeV/cm^2 #theta]");
		}
	dummy1->GetXaxis()->SetTitleOffset(1.3);
	dummy1->GetYaxis()->SetTitleOffset(1.5);
	dummy1->DrawCopy();
	gPad->SetLogx();
//	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionQFactor->Draw("same");

	TLegend* leg1=new TLegend(.18,.75,.53,.85);
	leg1->AddEntry(functionQFactor,source+"_"+candidate , "l");
	leg1->SetFillColor(0);
	leg1->SetLineColor(1);
	leg1->SetBorderSize(1);
	leg1->SetTextSize(0.037);
	leg1->Draw();
	gPad->Modified();
	gPad->Update();

//	canvas1->Print("/home/david/Documents/DarkMatter/Resultats/QFactor/"+author+"_"+candidate+"_"+source+".png");

}

void ShowJFactorFromLOSOn()
{
	TString author = "Bonnivard";
	TString source ="uma2";
	TString candidate = "Annihilation";
	TString instrumentName= "MagicPointLikeEfficiency";
	Double_t wobble=0.;
	TString sWobble = Form("%.1f",wobble);
	TString mySourcePath;
	TString myInstrumentPath= "/home/david/Documents/DarkMatter/Epsilon/"+instrumentName+".txt";

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
	TF1* functionJFactorFromLOSOn = JFactorFromLOSOn->GetTF1JFactorFromLOS_On();
	Double_t TethaMax = JFactorFromLOSOn->GetThetaMax();

	cout<<"Theta:0.01"<<endl;
	cout<<"JFactorFromLOS:"<<functionJFactorFromLOSOn->Eval(0.01)<<endl;
	cout<<"Theta:0.1"<<endl;
	cout<<"JFactorFromLOS:"<<functionJFactorFromLOSOn->Eval(0.1)<<endl;
	cout<<"Theta:0.5"<<endl;
	cout<<"JFactorFromLOS:"<<functionJFactorFromLOSOn->Eval(0.5)<<endl;
	cout<<"Theta:1."<<endl;
	cout<<"JFactorFromLOS:"<<functionJFactorFromLOSOn->Eval(1.)<<endl;
	cout<<"Theta:5."<<endl;
	cout<<"JFactorFromLOS:"<<functionJFactorFromLOSOn->Eval(5.)<<endl;
	cout<<"Theta:10."<<endl;
	cout<<"JFactorFromLOS:"<<functionJFactorFromLOSOn->Eval(10.)<<endl;

	functionJFactorFromLOSOn->SetLineColor(2);
	functionJFactorFromLOSOn->SetLineStyle(1);
	TCanvas* canvas7 = new TCanvas("canvas7","",600,550);
	TH1I* dummy7 = new TH1I("dummy7", author,1,0.01,TethaMax);
	dummy7->SetMaximum(3.e20);
	dummy7->SetMinimum(1.e15);
	dummy7->SetStats(0);
	dummy7->SetXTitle(" #theta ");
	dummy7->SetYTitle(" Efficiency ");
	dummy7->GetXaxis()->SetTitleOffset(1.3);
	dummy7->GetYaxis()->SetTitleOffset(1.5);
	dummy7->DrawCopy();
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionJFactorFromLOSOn->Draw("same");

	TLegend* leg7=new TLegend(.18,.75,.66,.85);
	leg7->AddEntry(functionJFactorFromLOSOn, source+"_"+candidate, "l");
	leg7->SetFillColor(0);
	leg7->SetLineColor(1);
	leg7->SetBorderSize(1);
	leg7->SetTextSize(0.037);
	leg7->Draw();
	gPad->Modified();
	gPad->Update();
}

void ShowJFactorFromLOSOff()
{
	TString author = "Bonnivard";
	TString source ="uma2";
	TString candidate = "Decay";
	TString instrumentName= "MagicPointLikeEfficiency";
	Double_t wobble=1.;
	TString sWobble = Form("%.1f",wobble);
	TString mySourcePath;
	TString myInstrumentPath= "/home/david/Documents/DarkMatter/Epsilon/"+instrumentName+".txt";

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
	TF1* functionJFactorFromLOSOff = JFactorFromLOSOff->GetTF1JFactorFromLOS_Off();
	Double_t TethaMax = JFactorFromLOSOff->GetThetaMax();

	functionJFactorFromLOSOff->SetLineColor(2);
	functionJFactorFromLOSOff->SetLineStyle(1);
	TCanvas* canvas8 = new TCanvas("canvas8","",600,550);
	TH1I* dummy8 = new TH1I("dummy8","",1,0.01,TethaMax);
	dummy8->SetMaximum(3.e20);
	dummy8->SetMinimum(1.e15);
	dummy8->SetStats(0);
	dummy8->SetXTitle(" #theta ");
	dummy8->SetYTitle(" Efficiency ");
	dummy8->GetXaxis()->SetTitleOffset(1.3);
	dummy8->GetYaxis()->SetTitleOffset(1.5);
	dummy8->DrawCopy();
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionJFactorFromLOSOff->Draw("same");

	TLegend* leg8=new TLegend(.18,.75,.66,.85);
	leg8->AddEntry(functionJFactorFromLOSOff, instrumentName, "l");
	leg8->SetFillColor(0);
	leg8->SetLineColor(1);
	leg8->SetBorderSize(1);
	leg8->SetTextSize(0.037);
	leg8->Draw();
	gPad->Modified();
	gPad->Update();
}

void ShowJFactorFromLOSTotal()
{
	TString author = "Bonnivard";
	TString source ="uma2";
	TString candidate = "Decay";
	TString instrumentName= "MagicPointLikeEfficiency";
	Double_t wobble=1.;
	TString mySourcePath;
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
	TF1* functionJFactorFromLOSTotal = JFactorFromLOSTotal->GetTF1JFactorFromLOS_Total();
	Double_t TethaMax = JFactorFromLOSTotal->GetThetaMax();

	functionJFactorFromLOSTotal->SetLineColor(2);
	functionJFactorFromLOSTotal->SetLineStyle(1);
	TCanvas* canvas9 = new TCanvas("canvas9","",600,550);
	TH1I* dummy9 = new TH1I("dummy9","",1,0.01,TethaMax);
	dummy9->SetMaximum(3.e20);
	dummy9->SetMinimum(1.e15);
	dummy9->SetStats(0);
	dummy9->SetXTitle(" #theta ");
	dummy9->SetYTitle(" Efficiency ");
	dummy9->GetXaxis()->SetTitleOffset(1.3);
	dummy9->GetYaxis()->SetTitleOffset(1.5);
	dummy9->DrawCopy();
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionJFactorFromLOSTotal->Draw("same");

	TLegend* leg9=new TLegend(.18,.75,.66,.85);
	leg9->AddEntry(functionJFactorFromLOSTotal, instrumentName, "l");
	leg9->SetFillColor(0);
	leg9->SetLineColor(1);
	leg9->SetBorderSize(1);
	leg9->SetTextSize(0.037);
	leg9->Draw();
	gPad->Modified();
	gPad->Update();
}

//-------------------------------------
//
void ShowJFactorEffective()
{
	TString author = "Bonnivard";
	TString source ="uma2";
	TString candidate = "Decay";
	TString instrumentName= "MagicPointLikeEfficiency";
	Double_t wobble=0.;
	TString sWobble = Form("%.1f",wobble);
	TString mySourcePath;
	TString myInstrumentPath= "/home/david/Documents/DarkMatter/Epsilon/"+instrumentName+".txt";

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

	JDOptimization* JFactorEfficiency = new JDOptimization(author, source, candidate, mySourcePath, instrumentName, wobble, myInstrumentPath);
	TF1* functionJFactorEfficiency = JFactorEfficiency->GetTF1JFactorEffectiveVsTheta();
	Double_t TethaMax = JFactorEfficiency->GetThetaMax();

	functionJFactorEfficiency->SetLineColor(2);
	functionJFactorEfficiency->SetLineStyle(1);
	TCanvas* canvas6 = new TCanvas("canvas6","",600,550);
	TH1I* dummy6 = new TH1I("dummy6","",1,0.01,TethaMax);
	dummy6->SetMaximum(3.e20);
	dummy6->SetMinimum(1.e15);
	dummy6->SetStats(0);
	dummy6->SetXTitle(" #theta ");
	dummy6->SetYTitle(" Efficiency ");
	dummy6->GetXaxis()->SetTitleOffset(1.3);
	dummy6->GetYaxis()->SetTitleOffset(1.5);
	dummy6->DrawCopy();
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionJFactorEfficiency->Draw("same");

	TLegend* leg6=new TLegend(.18,.75,.66,.85);
	leg6->AddEntry(functionJFactorEfficiency, instrumentName, "l");
	leg6->SetFillColor(0);
	leg6->SetLineColor(1);
	leg6->SetBorderSize(1);
	leg6->SetTextSize(0.037);
	leg6->Draw();
	gPad->Modified();
	gPad->Update();

//	canvas6->Print("/home/david/Documents/DarkMatter/Resultats/JFactorEfficiency/"+author+"_"+candidate+"_"+source+instrumentName+"_Wobble:"+sWobble+".png");
}

void RunExample1()
{
///////////////////////////////////////////////////////////////
//  Dark Matter Class
//////////////////////////////////////////////////////////////
//	ShowJFactor();
//	ShowLOS();

///////////////////////////////////////////////////////////////
//  Instrument Class
////////////////////////////////////////////////////////////////
//	ShowEpsilon();
//	ShowEfficiency();

///////////////////////////////////////////////////////////////
//  Combined Class
////////////////////////////////////////////////////////////////
	ShowQFactor();
//	ShowJFactorFromLOSOn();
//	ShowJFactorFromLOSOff();
//	ShowJFactorFromLOSTotal();
//	ShowJFactorEffective();
}

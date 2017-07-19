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
//
void ShowJFactor()
{

	TString author = "Bonnivard";
	TString source ="uma2";
	TString candidate = "Decay";

	JDDarkMatter* 	JFactor = new JDDarkMatter(author, source, candidate);
	TF1* functionJFactor = JFactor->GetTF1JFactorVsTheta();
	Double_t Theta = JFactor->GetTheta();

	cout<<Theta<<endl;

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
//
void ShowQFactor()
{

	TString author = "Bonnivard";
	TString source ="uma2";
	TString candidate = "Annihilation";
	Double_t normalizationPoint = 0.1;

	JDDarkMatter* QFactor = new JDDarkMatter(author, source, candidate);
//	QFactor->~JDDarkMatter();
	TF1* functionQFactor= QFactor->GetTF1QFactorVsTheta(normalizationPoint);
	Double_t Theta = QFactor->GetTheta();

	functionQFactor->SetLineColor(2);
	functionQFactor->SetLineStyle(1);
	TCanvas* canvas1 = new TCanvas("canvas1","",600,550);
	TH1I* dummy1 = new TH1I("dummy1",author ,1,0.01,Theta);
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

//-------------------------------------
//
void ShowLOS()
{
	TString author = "Bonnivard";
	TString source ="uma2";
	TString candidate = "Decay";

	JDDarkMatter* 	LOS = new JDDarkMatter(author, source, candidate);
	TF1* functionLOS = LOS->GetTF1LOSVsTheta();
	Double_t Theta = LOS->GetTheta();
	functionLOS->SetLineColor(2);
	functionLOS->SetLineStyle(1);

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

	canvas2->Print("/home/david/Documents/DarkMatter/Resultats/LOS/"+author+"_"+candidate+"_"+source+".png");

}

//-------------------------------------
//
void ShowJFactorFromLOS()
{
	TString author = "Bonnivard";
	TString source ="uma2";
	TString candidate = "Decay";

	JDDarkMatter* 	JFactorFromLOS = new JDDarkMatter(author, source, candidate);
	TF1* functionJFactorFromLOS = JFactorFromLOS->GetTF1JFactorFromLOSVsTheta();
	Double_t Theta = JFactorFromLOS->GetTheta();

	functionJFactorFromLOS->SetLineColor(2);
	functionJFactorFromLOS->SetLineStyle(1);
	TCanvas* canvas3 = new TCanvas("canvas3","",600,550);
	TH1I* dummy3 = new TH1I("dummy3", author,1,0.01,Theta);
	dummy3->SetMaximum(3.e20);
	dummy3->SetMinimum(1.e15);
	dummy3->SetStats(0);
	dummy3->SetXTitle(" #theta ");
	if (candidate == "Annihilation")
	{
		dummy3->SetYTitle(" J Factor From LOS [GeV^2/cm^5]");
	}
	else if (candidate == "Decay")
	{
		dummy3->SetYTitle(" J Factor From LOS [GeV/cm^2]");
	}
	dummy3->GetXaxis()->SetTitleOffset(1.3);
	dummy3->GetYaxis()->SetTitleOffset(1.5);
	dummy3->DrawCopy();
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionJFactorFromLOS->Draw("same");

	TLegend* leg3=new TLegend(.18,.75,.53,.85);
	leg3->AddEntry(functionJFactorFromLOS, source+"_"+candidate , "l");
	leg3->SetFillColor(0);
	leg3->SetLineColor(1);
	leg3->SetBorderSize(1);
	leg3->SetTextSize(0.037);
	leg3->Draw();
	gPad->Modified();
	gPad->Update();

	canvas3->Print("/home/david/Documents/DarkMatter/Resultats/JFactorFromLOS/"+author+"_"+candidate+"_"+source+".png");

}

//-------------------------------------
//
void ShowEpsilon()
{

	TString instrumentName= "MagicIdealEfficiency";
	Double_t wobble=0.;
	TString sWobble = Form("%.1f",wobble);

	JDInstrument* Epsilon = new JDInstrument(instrumentName, wobble);
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

	canvas4->Print("/home/david/Documents/DarkMatter/Resultats/Epsilon/"+instrumentName+"_Wobble:"+sWobble+".png");

}

//-------------------------------------
//
void ShowEfficiency()
{
	// We declare here the name of the ...
	TString instrumentName= "MagicIdealEfficiency";
	Double_t wobble=0.;
	TString sWobble = Form("%.1f",wobble);

	// We declare here the name of the ...
	JDInstrument* Efficiency = new JDInstrument(instrumentName, wobble);
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

	canvas5->Print("/home/david/Documents/DarkMatter/Resultats/Eficiència/"+instrumentName+"_Wobble:"+sWobble+".png");


	//EXAMPLE ONLY
	//canvas5->Print("../plots/"+InstrumentName+"_Wobble:"+sWobble+".png");

}



//-------------------------------------
//
void ShowJFactorEffective()
{
	TString author = "Bonnivard";
	TString source ="uma2";
	TString candidate = "Decay";
	TString instrumentName= "CrabNebula,Post-upgrade";
	Double_t wobble=0.;

	TString sWobble = Form("%.1f",wobble);


	JDOptimization* JFactorEfficiencyVsTheta = new JDOptimization(author, source, candidate, instrumentName, wobble);
	TF1* functionJFactorEfficiencyVsTheta = JFactorEfficiencyVsTheta->GetJFactorEffectiveVsTheta();
	Double_t TethaMax = JFactorEfficiencyVsTheta->GetThetaMax();

	functionJFactorEfficiencyVsTheta->SetLineColor(2);
	functionJFactorEfficiencyVsTheta->SetLineStyle(1);
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
	functionJFactorEfficiencyVsTheta->Draw("same");

	TLegend* leg6=new TLegend(.18,.75,.66,.85);
	leg6->AddEntry(functionJFactorEfficiencyVsTheta, instrumentName, "l");
	leg6->SetFillColor(0);
	leg6->SetLineColor(1);
	leg6->SetBorderSize(1);
	leg6->SetTextSize(0.037);
	leg6->Draw();
	gPad->Modified();
	gPad->Update();

	canvas6->Print("/home/david/Documents/DarkMatter/Resultats/JFactorEfficiency/"+author+"_"+candidate+"_"+source+instrumentName+"_Wobble:"+sWobble+".png");
}
void RunExample1()
{

///////////////////////////////////////////////////////////////
//  DM Class
//////////////////////////////////////////////////////////////
//	ShowJFactor();
//	ShowQFactor();
//	ShowLOS();
//	ShowJFactorFromLOS();

///////////////////////////////////////////////////////////////
//  Instrument Class
////////////////////////////////////////////////////////////////
//	ShowEpsilon();
//	ShowEfficiency();

///////////////////////////////////////////////////////////////
//  Combined Class
////////////////////////////////////////////////////////////////
	ShowJFactorEffective();
}

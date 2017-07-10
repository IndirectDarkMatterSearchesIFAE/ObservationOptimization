//this works home
#include "../source/JDDarkMatter.cc"
#include "../source/Instrument.cc"
#include "../source/Optimitzation.cc"

//this works at IFAE
//#include <../source/DM.cc>
//#include <../source/Instrument.cc>
//#include <../source/Optimitzation.cc>


//this works home
//#include </home/david/Work/Software/root_v6.06.08/include/TStyle.h>

//this works at IFAE
#include </home/david/Work/Software/scripts/style.h>

#include <TLegend.h>
#include <TCanvas.h>
#include <TF1.h>

using namespace std;


void ShowJFactor()
{

	TString author = "Bonnivard";
	TString source ="uma2";
	TString candidate = "Decay";

	JDDarkMatter* 	JFactor = new JDDarkMatter(author, source, candidate);
	TF1* functionJFactor = JFactor->GetTF1JFactorVsTheta();
	Double_t Theta = JFactor->GetTheta();

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

	TLegend* leg=new TLegend(.18,.75,.53,.85);
	leg->AddEntry(functionQFactor,source+"_"+candidate , "l");
	leg->SetFillColor(0);
	leg->SetLineColor(1);
	leg->SetBorderSize(1);
	leg->SetTextSize(0.037);
	leg->Draw();
	gPad->Modified();
	gPad->Update();

//	canvas1->Print("/home/david/Documents/DarkMatter/Resultats/QFactor/"+author+"_"+candidate+"_"+source+".png");

}

void ShowEpsilon()
{

	TString instrumentName= "MagicIdealEfficiency";
	Double_t wobble=0.;
	TString sWobble = Form("%.1f",wobble);

	JDInstrument* Epsilon = new JDInstrument(instrumentName, wobble);
	TF2* functionEpsilon = Epsilon->GetEpsilon();
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
//
void ShowEfficiency()
{
	// We declare here the name of the ...
	TString InstrumentName= "MagicIdealEfficiency";
	Double_t Wobble=0.;
	TString sWobble = Form("%.1f",Wobble);

	// We declare here the name of the ...
	JDInstrument* Efficiency = new JDInstrument(InstrumentName, Wobble);
	TF1* functionEfficiency = Efficiency->GetEfficiency();
	Double_t TethaMax = Efficiency->GetThetaMax();

	// We declare here the name of the ...
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
	leg3->AddEntry(functionEfficiency, InstrumentName, "l");
	leg3->SetFillColor(0);
	leg3->SetLineColor(1);
	leg3->SetBorderSize(1);
	leg3->SetTextSize(0.037);
	leg3->Draw();
	gPad->Modified();
	gPad->Update();

//	canvas3->Print("/home/david/Documents/DarkMatter/Resultats/Eficiència/"+InstrumentName+"_Wobble:"+sWobble+".png");
	//EXAMPLE ONLY
	//canvas3->Print("../plots/"+InstrumentName+"_Wobble:"+sWobble+".png");

}

void RunExample1()
{

///////////////////////////////////////////////////////////////
//  DM Class
//////////////////////////////////////////////////////////////
	ShowJFactor();
//	ShowQFactor();

///////////////////////////////////////////////////////////////
//  Instrument Class
////////////////////////////////////////////////////////////////
//	ShowEpsilon();
//	ShowEfficiency();

///////////////////////////////////////////////////////////////
//  Combined Class
////////////////////////////////////////////////////////////////
}

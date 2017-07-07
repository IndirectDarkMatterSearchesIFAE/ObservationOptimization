//this works home
#include "../source/DM.cc"
#include "../source/Instrument.cc"
#include "../source/Optimitzation.cc"

//this works at IFAE
//#include <../source/DM.cc>
//#include <../source/Instrument.cc>
//#include <../source/Optimitzation.cc>


//this works home
#include </home/david/Work/Software/root_v6.06.08/include/TStyle.h>

//this works at IFAE
//#include </home/david/Work/Software/scripts/style.h>

#include <TLegend.h>
#include <TCanvas.h>
#include <TF1.h>

using namespace std;


void ShowJFactor()
{

	TString Candidate ="uma2";
	TString Source = "Bonnivard";
	TString Form = "Annihilation";

	DM* 	JFactor = new DM(Candidate, Source, Form);
	TF1* functionJFactor = JFactor->GetJFactor();
	Double_t Theta = JFactor->GetTheta();

	functionJFactor->SetLineColor(2);
	functionJFactor->SetLineStyle(1);
	TCanvas* canvas = new TCanvas("canvas","",600,550);
	TH1I* dummy = new TH1I("dummy", Source,1,0.01,Theta);
	dummy->SetMaximum(3.e20);
	dummy->SetMinimum(1.e15);
	dummy->SetStats(0);
	dummy->SetXTitle(" #theta ");
	if (Form == "Annihilation")
	{
		cout<<"Annihilation"<<endl;
		dummy->SetYTitle(" J Factor [GeV^2/cm^5]");

	}
	else if (Form == "Decay")
	{
		cout<<"Decay"<<endl;
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
	leg->AddEntry(functionJFactor, Candidate+"_"+Form , "l");
	leg->SetFillColor(0);
	leg->SetLineColor(1);
	leg->SetBorderSize(1);
	leg->SetTextSize(0.037);
	leg->Draw();
	gPad->Modified();
	gPad->Update();

//	canvas->Print("/home/david/Documents/DarkMatter/Resultats/JFactor/"+Source+"_"+Form+"_"+Candidate+".png");

}

void ShowQFactor()
{

	TString Candidate ="uma2";
	TString Source = "Geringer";
	TString Form = "Decay";

	DM* QFactor = new DM(Candidate, Source, Form);
	TF1* functionQFactor= QFactor->GetQFactor();
	Double_t Theta = QFactor->GetTheta();

	functionQFactor->SetLineColor(2);
	functionQFactor->SetLineStyle(1);
	TCanvas* canvas1 = new TCanvas("canvas1","",600,550);
	TH1I* dummy1 = new TH1I("dummy1",Source ,1,0.01,Theta);
	dummy1->SetMaximum(3.);
	dummy1->SetMinimum(0.);
	dummy1->SetStats(0);
	dummy1->SetXTitle(" #theta ");
	if (Form == "Annihilation")
		{
			dummy1->SetYTitle(" Q Factor [GeV^2/cm^5 #theta]");

		}
		else if (Form == "Decay")
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
	leg->AddEntry(functionQFactor,Candidate+"_"+Form , "l");
	leg->SetFillColor(0);
	leg->SetLineColor(1);
	leg->SetBorderSize(1);
	leg->SetTextSize(0.037);
	leg->Draw();
	gPad->Modified();
	gPad->Update();

	canvas1->Print("/home/david/Documents/DarkMatter/Resultats/QFactor/"+Source+"_"+Form+"_"+Candidate+".png");

}

void ShowEpsilon()
{

	TString InstrumentName= "CrabNebula,Post-upgrade";
	Double_t Wobble=0.;
	TString sWobble = Form("%.1f",Wobble);

	Instrument* Epsilon = new Instrument(InstrumentName, Wobble);
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
	leg2->AddEntry(functionEpsilon, InstrumentName , "l");
	leg2->SetFillColor(0);
	leg2->SetLineColor(1);
	leg2->SetBorderSize(1);
	leg2->SetTextSize(0.037);
	leg2->Draw();
	gPad->Modified();
	gPad->Update();

	canvas2->Print("/home/david/Documents/DarkMatter/Resultats/Epsilon/"+InstrumentName+"_Wobble:"+sWobble+".png");

}

void ShowEfficiency()
{

	TString InstrumentName= "CrabNebula,Post-upgrade";
	Double_t Wobble=0.;
	TString sWobble = Form("%.1f",Wobble);

	Instrument* Efficiency = new Instrument(InstrumentName, Wobble);
	TF1* functionEfficiency = Efficiency->GetEfficiency();
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
	leg3->AddEntry(functionEfficiency, InstrumentName, "l");
	leg3->SetFillColor(0);
	leg3->SetLineColor(1);
	leg3->SetBorderSize(1);
	leg3->SetTextSize(0.037);
	leg3->Draw();
	gPad->Modified();
	gPad->Update();

	canvas3->Print("/home/david/Documents/DarkMatter/Resultats/Eficiència/"+InstrumentName+"_Wobble:"+sWobble+".png");

}

void RunExample1()
{

///////////////////////////////////////////////////////////////
//DM Class
//////////////////////////////////////////////////////////////
//	ShowJFactor();
//	ShowQFactor();

///////////////////////////////////////////////////////////////
//Instrument Class
////////////////////////////////////////////////////////////////
	ShowEpsilon();
	ShowEfficiency();
}

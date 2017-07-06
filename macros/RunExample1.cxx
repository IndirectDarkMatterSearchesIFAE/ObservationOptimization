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
	TString Form = "Decay";

	DM* 	JFactor = new DM(Candidate, Source, Form);
	JFactor->SetCandidate(Candidate);
	JFactor->SetSource(Source);
	JFactor->SetForm(Form);
	TF1* functionJFactor = JFactor->GetJFactor();

//	TString Candidate = JFactor->GetCandidate();
//	TString Source = JFactor->GetSource();
//	TString Form = JFactor->GetForm();

	functionJFactor->SetLineColor(2);
	functionJFactor->SetLineStyle(1);
	TCanvas* canvas = new TCanvas("canvas","",600,550);
	TH1I* dummy = new TH1I("dummy", Source,1,0.01,2.0);
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

	canvas->Print("/home/david/Documents/DarkMatter/Resultats/JFactor/"+Source+"_"+Form+"_"+Candidate+".png");

}

void ShowQFactor()
{

	DM* QFactor = new DM();
	TF1* functionQFactor= QFactor->GetQFactor();

	functionQFactor->SetLineColor(2);
	functionQFactor->SetLineStyle(1);
	TCanvas* canvas1 = new TCanvas("canvas1","",600,550);
	TH1I* dummy1 = new TH1I("dummy1","UMa2",1,0.01,2.0);
	dummy1->SetMaximum(10.);
	dummy1->SetMinimum(0.);
	dummy1->SetStats(0);
	dummy1->SetXTitle(" #theta ");
	dummy1->SetYTitle(" Q Factor [GeV^2/cm^5 #theta]");
	dummy1->GetXaxis()->SetTitleOffset(1.3);
	dummy1->GetYaxis()->SetTitleOffset(1.5);
	dummy1->DrawCopy();
	gPad->SetLogx();
//	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionQFactor->Draw("same");

	TLegend* leg=new TLegend(.18,.75,.38,.90);
	leg->AddEntry(functionQFactor, "Q Factor", "l");
	leg->SetFillColor(0);
	leg->SetLineColor(1);
	leg->SetBorderSize(1);
	leg->SetTextSize(0.037);
	leg->Draw();
	gPad->Modified();
	gPad->Update();
}

void ShowEpsilon()
{
	Instrument* Epsilon = new Instrument();
	TF1* functionEpsilon = Epsilon->GetEpsilon();

	functionEpsilon->SetLineColor(2);
	functionEpsilon->SetLineStyle(1);
	TCanvas* canvas3 = new TCanvas("canvas3","",600,550);
	TH1I* dummy3 = new TH1I("dummy3","UMa2",1,0.01,2.);
	dummy3->SetMaximum(1.5);
	dummy3->SetMinimum(0.);
	dummy3->SetStats(0);
	dummy3->SetXTitle(" dcc ");
	dummy3->SetYTitle(" Epsilon");
	dummy3->GetXaxis()->SetTitleOffset(1.3);
	dummy3->GetYaxis()->SetTitleOffset(1.5);
	dummy3->DrawCopy();
	gPad->SetLogx();
//	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionEpsilon->Draw("same");

	TLegend* leg3=new TLegend(.18,.75,.38,.90);
	leg3->AddEntry(functionEpsilon, "Epsilon", "l");
	leg3->SetFillColor(0);
	leg3->SetLineColor(1);
	leg3->SetBorderSize(1);
	leg3->SetTextSize(0.037);
	leg3->Draw();
	gPad->Modified();
	gPad->Update();
}

void ShowEfficiency()
{

//	Instrument* Efficiency = new Instrument();
//	TF1* functionEfficiency = Efficiency->GetEfficiency();
//
//	functionEfficiency->SetLineColor(2);
//	functionEfficiency->SetLineStyle(1);
//	TCanvas* canvas2 = new TCanvas("canvas2","",600,550);
//	TH1I* dummy2 = new TH1I("dummy2","UMa2",1,0.01,2.);
//	dummy2->SetMaximum(1.5);
//	dummy2->SetMinimum(0.);
//	dummy2->SetStats(0);
//	dummy2->SetXTitle(" #theta ");
//	dummy2->SetYTitle(" LOS [GeV^2/cm^5 #theta]");
//	dummy2->GetXaxis()->SetTitleOffset(1.3);
//	dummy2->GetYaxis()->SetTitleOffset(1.5);
//	dummy2->DrawCopy();
//	gPad->SetLogx();
////	gPad->SetLogy();
//	gPad->SetGridy();
//	gPad->SetGridx();
//	functionEfficiency->Draw("same");
//
//	TLegend* leg2=new TLegend(.18,.75,.38,.90);
//	leg2->AddEntry(functionEfficiency, "LOS", "l");
//	leg2->SetFillColor(0);
//	leg2->SetLineColor(1);
//	leg2->SetBorderSize(1);
//	leg2->SetTextSize(0.037);
//	leg2->Draw();
//	gPad->Modified();
//	gPad->Update();
}

void RunExample1()
{
	ShowJFactor();
//	ShowQFactor();
//	ShowEpsilon();
//	ShowEfficiency();
}

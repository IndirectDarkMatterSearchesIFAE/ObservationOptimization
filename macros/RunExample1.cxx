#include "../source/DM.cc"
#include "../source/Instrument.cc"
#include "../source/Optimitzation.cc"


#include </home/david/Work/Software/root_v6.06.08/include/TStyle.h>

#include <TLegend.h>
#include <TCanvas.h>
#include <TF1.h>

using namespace std;


void ShowJFactor()
{
	DM* 	JFactor = new DM();
	TF1* functionJFactor = JFactor->GetJFactor();

		functionJFactor->SetLineColor(2);
		functionJFactor->SetLineStyle(1);
		TCanvas* canvas = new TCanvas("canvas","",600,550);
		TH1I* dummy = new TH1I("dummy","UMa2",1,0.01,2.0);
		dummy->SetMaximum(3.e22);
		dummy->SetMinimum(1.e15);
		dummy->SetStats(0);
		dummy->SetXTitle(" #theta ");
		dummy->SetYTitle(" J Factor [GeV^2/cm^5]");
		dummy->GetXaxis()->SetTitleOffset(1.3);
		dummy->GetYaxis()->SetTitleOffset(1.5);
		dummy->DrawCopy();
		gPad->SetLogx();
		gPad->SetLogy();
		gPad->SetGridy();
		gPad->SetGridx();
		functionJFactor->Draw("same");

		TLegend* leg=new TLegend(.18,.75,.38,.90);
		leg->AddEntry(functionJFactor, "J Factor", "l");
		leg->SetFillColor(0);
		leg->SetLineColor(1);
		leg->SetBorderSize(1);
		leg->SetTextSize(0.037);
		leg->Draw();
		gPad->Modified();
		gPad->Update();
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


void RunExample1()
{
	ShowJFactor();
	ShowQFactor();
}

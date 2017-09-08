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
//  This function shows the QFactor (JFactor/Theta) of the Dark Matter data and the Instrument data from references
//
void PlotQ0Factor()	// J/theta
{

	TString author = "Bonnivard";
	TString source = "uma2";
	TString candidate = "Annihilation";
	TString mySourcePath = "/home/david/Work/eclipse/workspace/TFG/ObservationOptimization";
	TString instrumentName= "IDEAL";
	Double_t distanceCameraCenterMax=5;	// [deg]
	Double_t wobbleDist=1.;	// [deg]
	Double_t normalizationPoint=8.8136e+19;

	JDOptimization* QFactor = new JDOptimization(author, source, candidate, mySourcePath, instrumentName, distanceCameraCenterMax, wobbleDist);
	TF1* functionQFactorVsTheta= QFactor->GetTF1QFactorVsTheta(0,0.4);
//	TF1* functionJFactorVsTheta= QFactor->GetJFactorVsTheta();

	Double_t thetaMax = QFactor->GetThetaMax();
	Double_t distCameraCenterMax = QFactor->GetDistCameraCenterMax();

	functionQFactorVsTheta->SetLineColor(2);
	functionQFactorVsTheta->SetLineStyle(1);

	// This is for plotting purposes only
	TString isBonnivard = (QFactor->GetIsBonnivard()? "arxiv:1504.02048" : "");
	TString isGeringer = (QFactor->GetIsGeringer()? "arxiv:1408.0002" : "");
	TString isIDEAL = (QFactor->GetIsIdeal()? "IDEAL" : "");

//
	TLegend* leg=new TLegend(.18,.15,.88,.25);
//	leg->AddEntry(functionJFactorVsTheta, QFactor->GetSourceName()+" - "+isIDEAL+" - "+isBonnivard+isGeringer, "l");
	leg->SetFillColor(0);
	leg->SetLineColor(1);
	leg->SetBorderSize(1);
	leg->SetTextSize(0.037);


	TCanvas* canvas = new TCanvas("canvas","",600,550);
	TH1I* dummy = new TH1I("dummy", "" ,1,1e-2,thetaMax);
	dummy->SetMaximum(QFactor->GetJFactorMax()*1.1);
	dummy->SetMinimum(QFactor->GetJFactorMin()*0.9);
	dummy->SetStats(0);
	dummy->SetXTitle(" #theta [deg]");
	if(QFactor->GetCandidate()=="Annihilation"){dummy->SetYTitle(" JFactor_{Ann} [GeV^2/cm^{5}]");}
	else if(QFactor->GetCandidate()=="Decay"){	dummy->SetYTitle(" JFactor_{Dec} [GeV/cm^{3}]");}
	dummy->GetXaxis()->SetTitleOffset(1.3);
	dummy->GetYaxis()->SetTitleOffset(1.5);
	dummy->DrawCopy();
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
//	functionJFactorVsTheta->Draw("same");
	leg->Draw();
	gPad->Modified();
	gPad->Update();

	TCanvas* canvas2 = new TCanvas("canvas2","",600,550);
	TH1I* dummy1 = new TH1I("dummy1","" ,1,1e-2,thetaMax);
	dummy1->SetMaximum(3.);
	dummy1->SetMinimum(0.);
	dummy1->SetStats(0);
	dummy1->SetXTitle(" #theta [deg]");
	if(QFactor->GetCandidate()=="Annihilation"){	dummy->SetYTitle(" QFactor_{Ann} ");}
	else if(QFactor->GetCandidate()=="Decay"){	dummy->SetYTitle(" QFactor_{Dec} ");}
	dummy1->GetXaxis()->SetTitleOffset(1.3);
	dummy1->GetYaxis()->SetTitleOffset(1.5);
	dummy1->DrawCopy();
	gPad->SetLogx();
//	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionQFactorVsTheta->Draw("same");
	leg->Draw();
	gPad->Modified();
	gPad->Update();

}

//-------------------------------------
//  This function shows the QFactor (JFactor_On/Sqrt(Theta²+JFactor_Off) of the Dark Matter data and the Instrument data from references
//
void PlotQ1Factor() // J_on/Sqrt{theta^2 + J_off}
{

	TString author = "Bonnivard";
	TString source = "uma2";
	TString candidate = "Annihilation";
	TString mySourcePath = "/home/david/Work/eclipse/workspace/TFG/ObservationOptimization";
	TString instrumentName= "IDEAL";
	Double_t distanceCameraCenterMax=5;	// [deg]
	Double_t wobbleDist=1.;	// [deg]
	Double_t normalizationPoint=8.8136e+19;

	JDOptimization* QFactor = new JDOptimization(author, source, candidate, mySourcePath, instrumentName, distanceCameraCenterMax, wobbleDist);
	TF1* functionQFactorVsTheta = QFactor->GetTF1QFactorVsTheta(1,0.4);
//	TF2* functionQFactorVsThetaWobble= QFactor->GetTF2QFactorVsThetaWobble(1,0.1,0.4);


	Double_t thetaMax = QFactor->GetThetaMax();
	Double_t distCameraCenterMax = QFactor->GetDistCameraCenterMax();


	// This is for plotting purposes only
	TString isBonnivard = (QFactor->GetIsBonnivard()? "arxiv:1504.02048" : "");
	TString isGeringer = (QFactor->GetIsGeringer()? "arxiv:1408.0002" : "");
	TString isIDEAL = (QFactor->GetIsIdeal()? "IDEAL" : "");


	TLegend* leg=new TLegend(.18,.15,.88,.25);
	leg->AddEntry(functionQFactorVsTheta, "JFactor Segue", "l");
	leg->AddEntry(functionQFactorVsTheta, Form("JFactor_{off} Segue (%2.1f [deg])",QFactor->GetWobbleDistance()), "l");

	leg->SetFillColor(0);
	leg->SetLineColor(1);
	leg->SetBorderSize(1);
	leg->SetTextSize(0.037);

	TCanvas* canvas = new TCanvas("canvas","",600,550);
	TH1I* dummy = new TH1I("dummy", "" ,1,1e-2,thetaMax);
	dummy->SetMaximum(QFactor->GetJFactorMax()*1.1);
	dummy->SetMinimum(QFactor->GetJFactorMax()*0.001);
	dummy->SetStats(0);
	dummy->SetXTitle(" #theta [deg]");
	if(QFactor->GetCandidate()=="Annihilation"){dummy->SetYTitle(" JFactor_{Ann} [GeV^2/cm^{5}]");}
	else if(QFactor->GetCandidate()=="Decay"){	dummy->SetYTitle(" JFactor_{Dec} [GeV/cm^{3}]");}
	dummy->GetXaxis()->SetTitleOffset(1.3);
	dummy->GetYaxis()->SetTitleOffset(1.5);
	dummy->DrawCopy();
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionQFactorVsTheta->Draw("same");
	gPad->Modified();
	gPad->Update();
	functionQFactorVsTheta->SetLineColor(1);
	functionQFactorVsTheta->Draw("same");
	leg->Draw();
	gPad->Modified();
	gPad->Update();

//	TLegend* leg1=new TLegend(.18,.65,.88,.85);
////	leg1->AddEntry(functionQFactorVsThetaWobble, "J_{on}/#theta", "l");
////	leg1->AddEntry(functionQFactor1VsTheta, "J_{on}/Sqrt{#theta^{2}+J_{off}}", "l");
//
//	leg1->SetFillColor(0);
//	leg1->SetLineColor(1);
//	leg1->SetBorderSize(1);
//	leg1->SetTextSize(0.037);
//
//	TCanvas* canvas1 = new TCanvas("canvas1","",600,550);
//	TH1I* dummy1 = new TH1I("dummy1", "" ,1,1e-2,thetaMax);
//	dummy1->SetMaximum(3.);
//	dummy1->SetMinimum(0.);
//	dummy1->SetStats(0);
//	dummy1->SetXTitle(" #theta [deg]");
//	if(QFactor->GetCandidate()=="Annihilation"){	dummy->SetYTitle(" QFactor_{Ann} ");}
//	else if(QFactor->GetCandidate()=="Decay"){	dummy->SetYTitle(" QFactor_{Dec} ");}
//	dummy1->GetXaxis()->SetTitleOffset(1.3);
//	dummy1->GetYaxis()->SetTitleOffset(1.5);
//	dummy1->DrawCopy();
//	gPad->SetLogx();
////	gPad->SetLogy();
//	gPad->SetGridy();
//	gPad->SetGridx();
//	functionQFactorVsThetaWobble->Draw("same");
//	functionQFactorVsThetaWobble->Draw("same");
//	leg1->Draw();
//	gPad->Modified();
//	gPad->Update();
//
//	TCanvas* canvas2 = new TCanvas("canvas2","",600,550);
//	TH2I* dummy2 = new TH2I("dummy2","" ,1,1e-2,thetaMax,1,0.,distCameraCenterMax);
//	dummy2->SetStats(0);
//	dummy2->SetXTitle(" #theta [deg]");
//	dummy2->SetYTitle(" wooble_{dist} [deg] ");
//	dummy2->GetXaxis()->SetTitleOffset(1.3);
//	dummy2->GetYaxis()->SetTitleOffset(1.3);
//	dummy2->DrawCopy();
//	gPad->SetLogx();
////	gPad->SetLogy();
//	gPad->SetLogz();
//	gPad->SetGridy();
//	gPad->SetGridx();
//	functionQFactorVsThetaWobble->Draw("colz same");
////	cout <<	functionQFactorVsThetaWobble->Eval(0.1,0.1) << endl;
//	gPad->Modified();
//	gPad->Update();

}


//void PlotQ1Factor1() // J_on/Sqrt{theta^2 + J_off}
//{
//
//	TString instrumentName= "IDEAL";
//	TString txtFile = "/home/jpalacio/Work/eclipse/workspace/pic/DarkMatter/ObservationOptimization/references/JFactor/JFactorSegue.txt";
//	Double_t distanceCameraCenterMax=5;	// [deg]
//	Double_t wobbleDist=.4;	// [deg]
//
//	JDOptimization* QFactor = new JDOptimization(txtFile, instrumentName, distanceCameraCenterMax, wobbleDist);
//	TF2* functionQFactorVsThetaWobble= QFactor->GetTF2QFactorVsThetaWobble(1,0.1,0.4);
//	TF1* functionQFactor0VsTheta= QFactor->GetTF1QFactor0VsTheta();
//	TF1* functionQFactor1VsTheta= QFactor->GetTF1QFactor1VsTheta();
//	TF1* functionJFactorVsTheta= QFactor->GetTF1JFactorFromLOSVsTheta();
//	TF1* functionJFactorOffVsTheta= QFactor->GetTF1JFactorOffFromLOSVsTheta();
//	QFactor->SetCandidate("Annihilation");
//	Double_t thetaMax = QFactor->GetThetaMax();
//	Double_t distCameraCenterMax = QFactor->GetDistCameraCenterMax();
//
//	functionQFactor1VsTheta->SetLineColor(1);
//
//	// This is for plotting purposes only
//	TString isBonnivard = (QFactor->GetIsBonnivard()? "arxiv:1504.02048" : "");
//	TString isGeringer = (QFactor->GetIsGeringer()? "arxiv:1408.0002" : "");
//	TString isIDEAL = (QFactor->GetIsIdeal()? "IDEAL" : "");
//
//
//	TLegend* leg=new TLegend(.18,.15,.88,.25);
//	leg->AddEntry(functionJFactorVsTheta, "JFactor Segue", "l");
//	leg->AddEntry(functionJFactorOffVsTheta, Form("JFactor_{off} Segue (%2.1f [deg])",QFactor->GetWobbleDistance()), "l");
//
//	leg->SetFillColor(0);
//	leg->SetLineColor(1);
//	leg->SetBorderSize(1);
//	leg->SetTextSize(0.037);
//
//	TCanvas* canvas = new TCanvas("canvas","",600,550);
//	TH1I* dummy = new TH1I("dummy", "" ,1,1e-2,thetaMax);
//	dummy->SetMaximum(QFactor->GetJFactorMax()*1.1);
//	dummy->SetMinimum(QFactor->GetJFactorMax()*0.001);
//	dummy->SetStats(0);
//	dummy->SetXTitle(" #theta [deg]");
//	if(QFactor->GetCandidate()=="Annihilation"){dummy->SetYTitle(" JFactor_{Ann} [GeV^2/cm^{5}]");}
//	else if(QFactor->GetCandidate()=="Decay"){	dummy->SetYTitle(" JFactor_{Dec} [GeV/cm^{3}]");}
//	dummy->GetXaxis()->SetTitleOffset(1.3);
//	dummy->GetYaxis()->SetTitleOffset(1.5);
//	dummy->DrawCopy();
//	gPad->SetLogx();
//	gPad->SetLogy();
//	gPad->SetGridy();
//	gPad->SetGridx();
//	functionJFactorOffVsTheta->Draw("same");
//	gPad->Modified();
//	gPad->Update();
//	functionJFactorVsTheta->SetLineColor(1);
//	functionJFactorVsTheta->Draw("same");
//	leg->Draw();
//	gPad->Modified();
//	gPad->Update();
//
//	TLegend* leg1=new TLegend(.18,.65,.88,.85);
//	leg1->AddEntry(functionQFactor0VsTheta, "J_{on}/#theta", "l");
//	leg1->AddEntry(functionQFactor1VsTheta, "J_{on}/Sqrt{#theta^{2}+J_{off}}", "l");
//
//	leg1->SetFillColor(0);
//	leg1->SetLineColor(1);
//	leg1->SetBorderSize(1);
//	leg1->SetTextSize(0.037);
//
//	TCanvas* canvas1 = new TCanvas("canvas1","",600,550);
//	TH1I* dummy1 = new TH1I("dummy1", "" ,1,1e-2,thetaMax);
//	dummy1->SetMaximum(3.);
//	dummy1->SetMinimum(0.);
//	dummy1->SetStats(0);
//	dummy1->SetXTitle(" #theta [deg]");
//	if(QFactor->GetCandidate()=="Annihilation"){	dummy->SetYTitle(" QFactor_{Ann} ");}
//	else if(QFactor->GetCandidate()=="Decay"){	dummy->SetYTitle(" QFactor_{Dec} ");}
//	dummy1->GetXaxis()->SetTitleOffset(1.3);
//	dummy1->GetYaxis()->SetTitleOffset(1.5);
//	dummy1->DrawCopy();
//	gPad->SetLogx();
////	gPad->SetLogy();
//	gPad->SetGridy();
//	gPad->SetGridx();
//	functionQFactor0VsTheta->Draw("same");
//	functionQFactor1VsTheta->Draw("same");
//	leg1->Draw();
//	gPad->Modified();
//	gPad->Update();
//
//	TCanvas* canvas2 = new TCanvas("canvas2","",600,550);
//	TH2I* dummy2 = new TH2I("dummy2","" ,1,1e-2,thetaMax,1,0.,distCameraCenterMax);
//	dummy2->SetStats(0);
//	dummy2->SetXTitle(" #theta [deg]");
//	dummy2->SetYTitle(" wooble_{dist} [deg] ");
//	dummy2->GetXaxis()->SetTitleOffset(1.3);
//	dummy2->GetYaxis()->SetTitleOffset(1.3);
//	dummy2->DrawCopy();
//	gPad->SetLogx();
////	gPad->SetLogy();
//	gPad->SetLogz();
//	gPad->SetGridy();
//	gPad->SetGridx();
//	functionQFactorVsThetaWobble->Draw("colz same");
////	cout <<	functionQFactorVsThetaWobble->Eval(0.1,0.1) << endl;
//	gPad->Modified();
//	gPad->Update();
//
//}

void PlotQ2Factor() //J_1sm/theta
{

	TString author = "Bonnivard";
	TString source = "uma2";
	TString candidate = "Annihilation";
	TString mySourcePath = "/home/david/Work/eclipse/workspace/TFG/ObservationOptimization";
	TString instrumentName= "IDEAL";
	Double_t distanceCameraCenterMax=5;	// [deg]
	Double_t wobbleDist=1.;	// [deg]
	Double_t normalizationPoint=8.8136e+19;

	JDOptimization* QFactor = new JDOptimization(author, source, candidate, mySourcePath, instrumentName, distanceCameraCenterMax, wobbleDist);
	TF1* functionQFactorVsTheta= QFactor->GetTF1QFactorVsTheta(123,0.4);

	Double_t thetaMax = QFactor->GetThetaMax();
	Double_t distCameraCenterMax = QFactor->GetDistCameraCenterMax();

	functionQFactorVsTheta->SetLineColor(2);
	functionQFactorVsTheta->SetLineStyle(1);

	// This is for plotting purposes only
	TString isBonnivard = (QFactor->GetIsBonnivard()? "arxiv:1504.02048" : "");
	TString isGeringer = (QFactor->GetIsGeringer()? "arxiv:1408.0002" : "");
	TString isIDEAL = (QFactor->GetIsIdeal()? "IDEAL" : "");

	TLegend* leg=new TLegend(.18,.15,.88,.25);
//	leg->AddEntry(functionJFactorVsTheta, QFactor->GetSourceName()+" - "+isIDEAL+" - "+isBonnivard+isGeringer, "l");
	leg->SetFillColor(0);
	leg->SetLineColor(1);
	leg->SetBorderSize(1);
	leg->SetTextSize(0.037);

	TCanvas* canvas = new TCanvas("canvas","",600,550);
	TH1I* dummy = new TH1I("dummy","" ,1,1e-2,thetaMax);
	dummy->SetMaximum(3.);
	dummy->SetMinimum(0.);
	dummy->SetStats(0);
	dummy->SetXTitle(" #theta [deg]");
	if(QFactor->GetCandidate()=="Annihilation"){	dummy->SetYTitle(" QFactor_{Ann} ");}
	else if(QFactor->GetCandidate()=="Decay"){	dummy->SetYTitle(" QFactor_{Dec} ");}
	dummy->GetXaxis()->SetTitleOffset(1.3);
	dummy->GetYaxis()->SetTitleOffset(1.5);
	dummy->DrawCopy();
	gPad->SetLogx();
//	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionQFactorVsTheta->Draw("same");
	leg->Draw();
	gPad->Modified();
	gPad->Update();
}

void PlotQ3Factor()
{
	TString author = "Bonnivard";
	TString source = "uma2";
	TString candidate = "Annihilation";
	TString mySourcePath = "/home/david/Work/eclipse/workspace/TFG/ObservationOptimization";
	TString instrumentName= "IDEAL";
	Double_t distanceCameraCenterMax=5;	// [deg]
	Double_t wobbleDist=1.;	// [deg]
	Double_t normalizationPoint=8.8136e+19;

	JDOptimization* QFactor = new JDOptimization(author, source, candidate, mySourcePath, instrumentName, distanceCameraCenterMax, wobbleDist);
	TF1* functionQFactorVsTheta= QFactor->GetTF1QFactorVsTheta(3,0.4);

	Double_t thetaMax = QFactor->GetThetaMax();
	Double_t distCameraCenterMax = QFactor->GetDistCameraCenterMax();

	functionQFactorVsTheta->SetLineColor(2);
	functionQFactorVsTheta->SetLineStyle(1);

	// This is for plotting purposes only
	TString isBonnivard = (QFactor->GetIsBonnivard()? "arxiv:1504.02048" : "");
	TString isGeringer = (QFactor->GetIsGeringer()? "arxiv:1408.0002" : "");
	TString isIDEAL = (QFactor->GetIsIdeal()? "IDEAL" : "");

	TLegend* leg=new TLegend(.18,.15,.88,.25);
//	leg->AddEntry(functionJFactorVsTheta, QFactor->GetSourceName()+" - "+isIDEAL+" - "+isBonnivard+isGeringer, "l");
	leg->SetFillColor(0);
	leg->SetLineColor(1);
	leg->SetBorderSize(1);
	leg->SetTextSize(0.037);

	TCanvas* canvas1 = new TCanvas("canvas1","",600,550);
	TH1I* dummy = new TH1I("dummy","" ,1,1e-2,thetaMax);
	dummy->SetMaximum(3.);
	dummy->SetMinimum(0.);
	dummy->SetStats(0);
	dummy->SetXTitle(" #theta [deg]");
	if(QFactor->GetCandidate()=="Annihilation"){	dummy->SetYTitle(" QFactor_{Ann} ");}
	else if(QFactor->GetCandidate()=="Decay"){	dummy->SetYTitle(" QFactor_{Dec} ");}
	dummy->GetXaxis()->SetTitleOffset(1.3);
	dummy->GetYaxis()->SetTitleOffset(1.5);
	dummy->DrawCopy();
	gPad->SetLogx();
//	gPad->SetLogy();
	gPad->SetGridy();
	gPad->SetGridx();
	functionQFactorVsTheta->Draw("same");
	leg->Draw();
	gPad->Modified();
	gPad->Update();
}
void exampleJDOptimization()
{
///////////////////////////////////////////////////////////////
//  Combined Class
///////////////////////////////////////////////////////////////

//	PlotQ0Factor();		// 	J/theta
//	PlotQ1Factor();		//	J_on/Sqrt{theta^2+J_off}
	PlotQ2Factor();		//  J_1sm/theta
//	PlotQ3Factor();		//  J_eff/theta_eff


}

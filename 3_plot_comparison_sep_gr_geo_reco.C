#include <iostream>
#include <fstream>
#include <TFile.h>
#include <TTree.h>
#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <TLine.h>

void sep_gr_geo_reco()
{
  TFile *file_standalone[25];
  TTree *tree_standalone[25];

  TString path_standalone("/w/halld-scshelf2101/halld3/home/ihossain/Mcp_test_reconstruction_5_4/reco_files/");
  TString in_standalone[25];

  TString path_standalone_4_4("/w/halld-scshelf2101/halld3/home/ihossain/Mcp_test_reconstruction/reco_files/");
  TString in_standalone_4_4[25];

    
  Int_t theta_value[25];
  Double_t theta[25];
  Double_t theta_err[25] = {0};

  Double_t sep_gr_standaloneRoman[25] = {3.730761, 3.596965, 3.308513, 3.266788, 3.30443, 3.362408, 3.210066, 3.193811, 3.053817, 3.001219, 2.787778, 3.46087, 4.170896, 2.825622, 2.63589, 2.835406, 2.772732, 2.940102, 3.034555, 3.0925, 3.101411, 3.202123, 3.199491, 3.440447, 3.658279};

  Double_t sep_gr_err_standaloneRoman[25] = {0.2030417, 0.2030106, 0.2025419, 0.2026328, 0.2024949, 0.2025293, 0.2025467, 0.2024625, 0.202323, 0.202408, 0.202169, 0.2028398, 0.2033696, 0.2021105, 0.2019752, 0.2021825, 0.2020778, 0.2023317, 0.2023261, 0.2024186, 0.2023646, 0.2025706, 0.2024614, 0.2027412, 0.2030658};



  for(int i=0; i < 25; i++)
    {
      theta_value[i] = 30 + 5*i;
      theta[i] = 30 + 5*i;
    }

  Double_t sep_gr_standalone[25];
  Double_t sep_gr_err_standalone[25];
  Double_t sep_gr, sep_gr_err;

  Double_t sep_gr_standalone_4_4[25];
  Double_t sep_gr_err_standalone_4_4[25];

  TGraphErrors *gr1[3]; //imran
  
  TMultiGraph *mg = new TMultiGraph(); //imran
  TLine *line = new TLine(25, 3, 150, 3);

  for(int j=0; j < 25; j++)
    {      
      in_standalone[j] = Form(path_standalone + "reco_%1.0d_deg.root", theta_value[j]);  
      file_standalone[j] = TFile::Open(in_standalone[j]);
      tree_standalone[j] = (TTree*) file_standalone[j]->Get("reco");

      tree_standalone[j]->SetBranchAddress("sep_gr",&sep_gr);               
      tree_standalone[j]->SetBranchAddress("sep_gr_err",&sep_gr_err);
      tree_standalone[j]->GetEntry(0);

      sep_gr_standalone[j] = sep_gr;
      sep_gr_err_standalone[j] = sep_gr_err;


      in_standalone_4_4[j] = Form(path_standalone_4_4 + "reco_%1.0d_deg.root", theta_value[j]);
      file_standalone[j] = TFile::Open(in_standalone_4_4[j]);
      tree_standalone[j] = (TTree*) file_standalone[j]->Get("reco");

      tree_standalone[j]->SetBranchAddress("sep_gr",&sep_gr);
      tree_standalone[j]->SetBranchAddress("sep_gr_err",&sep_gr_err);
      tree_standalone[j]->GetEntry(0);

      sep_gr_standalone_4_4[j] = sep_gr;
      sep_gr_err_standalone_4_4[j] = sep_gr_err;
      
      //imran code
      gr1[0] = new TGraphErrors(25, theta, sep_gr_standalone, theta_err, sep_gr_err_standalone);
      gr1[1] = new TGraphErrors(25, theta, sep_gr_standaloneRoman, theta_err, sep_gr_err_standaloneRoman);
      gr1[2] = new TGraphErrors(25, theta, sep_gr_standalone_4_4, theta_err, sep_gr_err_standalone_4_4);
    }

  //TGraphErrors* gr1 = new TGraphErrors(25, theta, sep_gr_standalone, theta_err, sep_gr_err_standalone);
  //TGraphErrors* gr2 = new TGraphErrors(25, theta, sep_gr_standaloneRoman, theta_err, sep_gr_err_standaloneRoman);
  //TGraphErrors* gr3 = new TGraphErrors(25, theta, sep_gr_standalone_4_4, theta_err, sep_gr_err_standalone_4_4);

  //gr1->SetTitle("Separation Power at 6 GeV/c - geometric reco. default vs Mcp Rearrange");
  //gr1->GetXaxis()->SetTitle("Polar angle (deg)");  
  //gr1->GetYaxis()->SetTitle("Separation Power (s.d.)");
  //gr1->GetYaxis()->SetRangeUser(0,10);
  //gr1->GetXaxis()->SetRangeUser(20,160);
  //gr1->GetXaxis()->SetTitleSize(0.05);
  //gr1->GetYaxis()->SetTitleSize(0.05);  
  //gr1->GetXaxis()->SetTitleOffset(0.8);
  //gr1->GetYaxis()->SetTitleOffset(0.8);

  gr1[0]->SetMarkerStyle(kFullDotLarge);
  gr1[0]->SetMarkerSize(1.4);
  gr1[0]->SetMarkerColor(1);
  gr1[0]->SetLineColor(1);
  mg->Add(gr1[0]);

  
  //gr1->SetMarkerStyle(kFullDotLarge);
  //gr1->SetMarkerSize(1.4);
  //gr1->SetMarkerColor(1);
  //gr1->SetLineColor(1);
  //gr1->Draw("APL");

  gr1[1]->SetMarkerStyle(kFullDotLarge);
  gr1[1]->SetMarkerSize(1);
  gr1[1]->SetMarkerColor(2);
  gr1[1]->SetLineColor(2);
  mg->Add(gr1[1]);

  //gr3->GetYaxis()->SetRangeUser(0,10);
  //gr3->GetXaxis()->SetRangeUser(20,160);
  //gr3->GetXaxis()->SetTitleSize(0.05);
  //gr3->GetYaxis()->SetTitleSize(0.05);
  //gr3->GetXaxis()->SetTitleOffset(0.8);
  //gr3->GetYaxis()->SetTitleOffset(0.8);

  gr1[2]->SetMarkerStyle(kFullDotLarge);
  gr1[2]->SetMarkerSize(1.4);
  gr1[2]->SetMarkerColor(3);
  gr1[2]->SetLineColor(3);
  mg->Add(gr1[2]);

  mg->SetTitle("Separation Power at 6 GeV/c - geometric reco. for Mcp 6x4 vs 5x4 vs 4x4");                                             
  mg->GetXaxis()->SetTitle("Polar angle (deg)");                  
  mg->GetYaxis()->SetTitle("Separation Power (s.d.)");            
  mg->GetYaxis()->SetRangeUser(0,10);                             
  mg->GetXaxis()->SetRangeUser(20,160);                           
  mg->GetXaxis()->SetTitleSize(0.05);                             
  mg->GetYaxis()->SetTitleSize(0.05);                             
  mg->GetXaxis()->SetTitleOffset(0.8);                            
  mg->GetYaxis()->SetTitleOffset(0.8);
  
  mg->Draw("APL");
  
  line->SetLineColor(kBlack);
  line->SetLineWidth(2);
  line->Draw("SAME");

  gPad->SetGrid();

}


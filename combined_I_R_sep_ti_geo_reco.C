#include <iostream>
#include <fstream>

void combined_I_R_sep_ti_geo_reco()
{
  TFile *file_standalone[25];
  TTree *tree_standalone[25];

  TString path_standalone("/w/halld-scshelf2101/halld3/home/ihossain/t_reconstruction_check/reco_files/");
  TString in_standalone[25];
    
  Int_t theta_value[25];
  Double_t theta[25];
  Double_t theta_err[25] = {0};

  Double_t sep_ti_standaloneRoman[25] = {4.122793, 4.241292, 3.836061, 3.801448, 3.762329, 3.738932, 3.544073, 3.439946, 3.361644, 3.326905, 3.146923, 4.493265, 5.248694, 4.267332, 3.536726, 3.615854, 3.690439, 3.772235, 3.896683, 4.022663, 3.986315, 4.11946, 4.25407, 4.453654, 4.897438};

  Double_t sep_ti_err_standaloneRoman[25] = {0.2037139, 0.2038312, 0.2031278, 0.2032166, 0.2031149, 0.2030702, 0.2030146, 0.2028507, 0.2027265, 0.2027006, 0.2025352, 0.2039005, 0.2048793, 0.2037618, 0.2028461, 0.2030545, 0.2030875, 0.2031511, 0.203252, 0.2034111, 0.20352, 0.2035431, 0.2037563, 0.2042077, 0.2045701};

  for(int i=0; i < 25; i++)
    {
      theta_value[i] = 30 + 5*i;
      theta[i] = 30 + 5*i;
    }

  Double_t sep_ti_standalone[25];
  Double_t sep_ti_err_standalone[25];
  Double_t sep_ti, sep_ti_err;

  for(int j=0; j < 25; j++)
    {      
      in_standalone[j] = Form(path_standalone + "reco_%1.0d_deg.root", theta_value[j]);  
      file_standalone[j] = TFile::Open(in_standalone[j]);
      tree_standalone[j] = (TTree*) file_standalone[j]->Get("reco");

      tree_standalone[j]->SetBranchAddress("sep_ti",&sep_ti);               
      tree_standalone[j]->SetBranchAddress("sep_ti_err",&sep_ti_err);
      tree_standalone[j]->GetEntry(0);

      sep_ti_standalone[j] = sep_ti;
      sep_ti_err_standalone[j] = sep_ti_err;
      
    }

  TGraphErrors* gr1 = new TGraphErrors(25, theta, sep_ti_standalone, theta_err, sep_ti_err_standalone);
  TGraphErrors* gr2 = new TGraphErrors(25, theta, sep_ti_standaloneRoman, theta_err, sep_ti_err_standaloneRoman);


  gr1->SetTitle("Separation Power at 6 GeV/c - time imaging reco.");
  gr1->GetXaxis()->SetTitle("Polar angle (deg)");  
  gr1->GetYaxis()->SetTitle("Separation Power (s.d.)");
  gr1->GetYaxis()->SetRangeUser(0,10);
  gr1->GetXaxis()->SetRangeUser(20,160);
  gr1->GetXaxis()->SetTitleSize(0.05);
  gr1->GetYaxis()->SetTitleSize(0.05);  
  gr1->GetXaxis()->SetTitleOffset(0.8);
  gr1->GetYaxis()->SetTitleOffset(0.8);
  
  gr1->SetMarkerStyle(kFullCircle);
  gr1->SetMarkerSize(1.4);
  gr1->SetMarkerColor(1);
  gr1->SetLineColor(1);
  gr1->Draw("APL");

  gr2->SetMarkerStyle(kFullCircle);
  gr2->SetMarkerSize(1.4);
  gr2->SetMarkerColor(1);
  gr2->SetLineColor(2);
  gr2->Draw("same");


  gPad->SetGrid();

}


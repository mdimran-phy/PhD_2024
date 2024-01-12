#include <iostream>
#include <fstream>

void combined_I_R_nph_ti_geo_reco()
{
  TFile *file_standalone[25];
  TTree *tree_standalone[25];

  TString path_standalone("/w/halld-scshelf2101/halld3/home/ihossain/t_reconstruction_check/reco_files/");
  TString in_standalone[25];
    
  Int_t theta_value[25];
  Double_t theta[25];
  Double_t theta_err[25] = {0};

  Double_t nph_ti_standaloneRoman[25] = {111.4964, 100.0659, 78.1644, 74.19845, 61.71498, 58.33431, 50.09018, 45.36478, 40.05502, 36.63679, 30.77543, 44.30096, 54.89887, 47.26138, 40.13871, 46.90694, 50.3276, 56.78034, 62.87067, 72.55601, 77.10775, 91.87683, 97.11863, 123.5043, 138.6409};

  Double_t nph_ti_err_standaloneRoman[25] = {5.002309, 5.00202, 5.00162, 5.001501, 5.001238, 5.001201, 5.001004, 5.000936, 5.000823, 5.000766, 5.000641, 5.000898, 5.001061, 5.000939, 5.000803, 5.000963, 5.001001, 5.001192, 5.001293, 5.001424, 5.00159, 5.00188, 5.001988, 5.002537, 5.002785};

  for(int i=0; i < 25; i++)
    {
      theta_value[i] = 30 + 5*i;
      theta[i] = 30 + 5*i;
    }

  Double_t nph_ti_standalone[25];
  Double_t nph_ti_err_standalone[25];
  Double_t nph_ti[5], nph_ti_err[5];

  for(int j=0; j < 25; j++)
    {      
      in_standalone[j] = Form(path_standalone + "reco_%1.0d_deg.root", theta_value[j]);  
      file_standalone[j] = TFile::Open(in_standalone[j]);
      tree_standalone[j] = (TTree*) file_standalone[j]->Get("reco");

      tree_standalone[j]->SetBranchAddress("nph_ti",&nph_ti);               
      tree_standalone[j]->SetBranchAddress("nph_ti_err",&nph_ti_err);
      tree_standalone[j]->GetEntry(0);
      nph_ti_standalone[j] = nph_ti[2];
      nph_ti_err_standalone[j] = nph_ti_err[2];
      
    }

  TGraphErrors* gr1 = new TGraphErrors(25, theta, nph_ti_standalone, theta_err, nph_ti_err_standalone);
  TGraphErrors* gr2 = new TGraphErrors(25, theta, nph_ti_standaloneRoman, theta_err, nph_ti_err_standaloneRoman);
 

  gr1->SetTitle("Photon yield at 6 GeV/c - time imaging reco.");
  gr1->GetXaxis()->SetTitle("Polar angle (deg)");  
  gr1->GetYaxis()->SetTitle("Photon yield");
  gr1->GetYaxis()->SetRangeUser(0,200);
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
  gr2->SetMarkerColor(2);
  gr2->SetLineColor(2);
  gr2->Draw("same");


  gPad->SetGrid();

}


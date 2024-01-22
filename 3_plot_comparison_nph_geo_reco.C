#include <iostream>
#include <fstream>

void nph_geo_reco()
{
  TFile *file_standalone[25];
  TTree *tree_standalone[25];

  TString path_standalone_5_4("/w/halld-scshelf2101/halld3/home/ihossain/Mcp_test_reconstruction_5_4/reco_files/");
  TString path_standalone_4_4("/w/halld-scshelf2101/halld3/home/ihossain/Mcp_test_reconstruction/reco_files/");

  TString in_standalone_5_4[25];
  TString in_standalone_4_4[25];
  // TString tree_standalone_5_4[25];
  //TString file_standalone_5_4[25];
  

    
  Int_t theta_value[25];
  Double_t theta[25];
  Double_t theta_err[25] = {0};

  Double_t nph_gr_standalone_6_4[25] = {101.7201, 91.12185, 70.00105, 65.96993, 54.52047, 52.06829, 44.42717, 40.32332, 35.76618, 32.90585, 27.20358, 35.74753, 49.17241, 40.31743, 36.28313, 42.68954, 45.54804, 51.35406, 56.69788, 65.9413, 69.576, 83.3358, 88.11396, 114.4614, 129.4032};

  Double_t nph_gr_err_standalone_6_4[25] = {5.002063, 5.001833, 5.001432, 5.001313, 5.001072, 5.00109, 5.000898, 5.000791, 5.00072, 5.000676, 5.000587, 5.000717, 5.000976, 5.000817, 5.000718, 5.000851, 5.000894, 5.001074, 5.001179, 5.001314, 5.00144, 5.001681, 5.001766, 5.002348, 5.002622};

  for(int i=0; i < 25; i++)
    {
      theta_value[i] = 30 + 5*i;
      theta[i] = 30 + 5*i;
    }

  Double_t nph_gr_standalone_5_4[25];
  Double_t nph_gr_err_standalone_5_4[25];
  Double_t nph_gr[5], nph_gr_err[5];

  Double_t nph_gr_standalone_4_4[25];
  Double_t nph_gr_err_standalone_4_4[25];

  for(int j=0; j < 25; j++)
    {      
      in_standalone_5_4[j] = Form(path_standalone_5_4 + "reco_%1.0d_deg.root", theta_value[j]);  
      file_standalone[j] = TFile::Open(in_standalone_5_4[j]);
      tree_standalone[j] = (TTree*) file_standalone[j]->Get("reco");

      tree_standalone[j]->SetBranchAddress("nph_gr",&nph_gr);               
      tree_standalone[j]->SetBranchAddress("nph_gr_err",&nph_gr_err);
      tree_standalone[j]->GetEntry(0);

      nph_gr_standalone_5_4[j] = nph_gr[2];
      nph_gr_err_standalone_5_4[j] = nph_gr_err[2];

      in_standalone_4_4[j] = Form(path_standalone_4_4 + "reco_%1.0d_deg.root", theta_value[j]);
      file_standalone[j] = TFile::Open(in_standalone_4_4[j]);
      tree_standalone[j] = (TTree*) file_standalone[j]->Get("reco");

      tree_standalone[j]->SetBranchAddress("nph_gr",&nph_gr);
      tree_standalone[j]->SetBranchAddress("nph_gr_err",&nph_gr_err);
      tree_standalone[j]->GetEntry(0);

      nph_gr_standalone_4_4[j] = nph_gr[2];
      nph_gr_err_standalone_4_4[j] = nph_gr_err[2];
     
      
    }

  TGraphErrors* gr1 = new TGraphErrors(25, theta, nph_gr_standalone_5_4, theta_err, nph_gr_err_standalone_5_4);
  TGraphErrors* gr2 = new TGraphErrors(25, theta, nph_gr_standalone_6_4, theta_err, nph_gr_err_standalone_6_4);
  TGraphErrors* gr3 = new TGraphErrors(25, theta, nph_gr_standalone_4_4, theta_err, nph_gr_err_standalone_4_4);



  gr1->SetTitle("Photon yield at 6 GeV/c - geometric reco 6x4 vs 5x4 vs 4x4");
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
  gr2->SetMarkerSize(1);
  gr2->SetMarkerColor(2);
  gr2->SetLineColor(2);
  gr2->Draw("same");

  gr2->GetYaxis()->SetRangeUser(0,200);
  gr2->GetXaxis()->SetRangeUser(20,160);
  gr2->GetXaxis()->SetTitleSize(0.05);
  gr2->GetYaxis()->SetTitleSize(0.05);
  gr2->GetXaxis()->SetTitleOffset(0.8);
  gr2->GetYaxis()->SetTitleOffset(0.8);

  gr3->SetMarkerStyle(kFullCircle);
  gr3->SetMarkerSize(1);
  gr3->SetMarkerColor(3);
  gr3->SetLineColor(3);
  gr3->Draw("same");

  gPad->SetGrid();

}


#include <iostream>

#include <fstream>

void spr_geo_reco()
{
  TFile *file_standalone[25];
  TTree *tree_standalone[25];

  TString path_standalone("/w/halld-scshelf2101/halld3/home/ihossain/reconstruction/reco_files/");
  TString in_standalone[25];
    
  Int_t theta_value[25];
  Double_t theta[25];
  Double_t theta_err[25] = {0};

  for(int i=0; i < 25; i++)
    {
      theta_value[i] = 30 + 5*i;
      theta[i] = 30 + 5*i;
    }

  Double_t spr_gr_standalone[25];
  Double_t spr_gr_err_standalone[25];
  Double_t spr[5], spr_err[5];

  for(int j=0; j < 25; j++)
    {      
      in_standalone[j] = Form(path_standalone + "reco_%1.0d_deg.root", theta_value[j]);  
      file_standalone[j] = TFile::Open(in_standalone[j]);
      tree_standalone[j] = (TTree*) file_standalone[j]->Get("reco");

      tree_standalone[j]->SetBranchAddress("spr", &spr);               
      tree_standalone[j]->GetEntry(0);

      //      spr_gr_standalone[j] = spr_gr[2];
      //spr_gr_err_standalone[j] = spr_gr_err[2];
    
      spr_gr_standalone[j] = spr[2];
      spr_gr_err_standalone[j] = 0.;
  
    }

  TGraphErrors* gr1 = new TGraphErrors(25, theta, spr_gr_standalone, theta_err, spr_gr_err_standalone);

  gr1->SetTitle("SPR at 6 GeV/c - Geometric reco.");
  gr1->GetXaxis()->SetTitle("Polar angle (deg)");  
  gr1->GetYaxis()->SetTitle("Single Photon Resolution (mrad)");
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

  gPad->SetGrid();

}


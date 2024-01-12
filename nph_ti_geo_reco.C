#include <iostream>
#include <fstream>

void nph_ti_geo_reco()
{
  TFile *file_standalone[25];
  TTree *tree_standalone[25];

  TString path_standalone("/w/halld-scshelf2101/halld3/home/ihossain/t_reconstruction/reco_files/");
  TString in_standalone[25];
    
  Int_t theta_value[25];
  Double_t theta[25];
  Double_t theta_err[25] = {0};

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

  TGraphErrors* gr1 = new TGraphErrors(25, theta, nph_ti_standalone, theta_err,\
				       nph_ti_err_standalone);
 

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

  // gr2->SetMarkerStyle(kFullCircle);
  //gr2->SetMarkerSize(1.4);
  //gr2->SetMarkerColor(2);
  //gr2->SetLineColor(2);
  //gr2->Draw("same");


  gPad->SetGrid();

}


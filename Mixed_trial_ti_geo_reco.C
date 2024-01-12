#include <iostream>
#include <fstream>

void Mixed_trial_ti_geo_reco()
{
  TFile *file_standaloneti[25];
  TTree *tree_standaloneti[25];

  TFile *file_standalonegr[25];
  TTree *tree_standalonegr[25];

  TString path_standaloneti("/w/halld-scshelf2101/halld3/home/ihossain/t_reconstruction/reco_files/");
  TString path_standalonegr("/w/halld-scshelf2101/halld3/home/ihossain/reconstruction/reco_files/");

  TString in_standaloneti[25];
  TString in_standalonegr[25];
    
  Int_t theta_value[25];
  Double_t theta[25];
  Double_t theta_err[25] = {0};

  for(int i=0; i < 25; i++)
    {
      theta_value[i] = 30 + 5*i;
      theta[i] = 30 + 5*i;
    }

  Double_t sep_ti_standaloneti[25];
  Double_t sep_ti_err_standaloneti[25];
  Double_t sep_ti, sep_ti_err;

  Double_t sep_gr_standalonegr[25];
  Double_t sep_gr_err_standalonegr[25];
  Double_t sep_gr, sep_gr_err;

  for(int j=0; j < 25; j++)
    {      
      in_standaloneti[j] = Form(path_standaloneti + "reco_%1.0d_deg.root", theta_value[j]);  
      file_standaloneti[j] = TFile::Open(in_standaloneti[j]);
      tree_standaloneti[j] = (TTree*) file_standaloneti[j]->Get("reco");

      in_standalonegr[j] = Form(path_standalonegr + "reco_%1.0d_deg.root", theta_value[j]);
      file_standalonegr[j] = TFile::Open(in_standalonegr[j]);
      tree_standalonegr[j] = (TTree*) file_standalonegr[j]->Get("reco");

      tree_standaloneti[j]->SetBranchAddress("sep_ti",&sep_ti);               
      tree_standaloneti[j]->SetBranchAddress("sep_ti_err",&sep_ti_err);
      tree_standaloneti[j]->GetEntry(0);

      tree_standalonegr[j]->SetBranchAddress("sep_gr",&sep_gr);
      tree_standalonegr[j]->SetBranchAddress("sep_gr_err",&sep_gr_err);
      tree_standalonegr[j]->GetEntry(0);

      sep_ti_standaloneti[j] = sep_ti;
      sep_ti_err_standaloneti[j] = sep_ti_err;
      

      sep_gr_standalonegr[j] = sep_gr;
      sep_gr_err_standalonegr[j] = sep_gr_err;
    }

  TGraphErrors* gr1 = new TGraphErrors(25, theta, sep_ti_standaloneti, theta_err, sep_ti_err_standaloneti);

  TGraphErrors* gr2 = new TGraphErrors(25, theta, sep_gr_standalonegr, theta_err, sep_gr_err_standalonegr);

  gr1->SetTitle("Separation Power at 6 GeV/c - ti ve geo reco.");
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
  gr2->SetMarkerColor(2);
  gr2->SetLineColor(1);
  gr2->Draw("Same");


  gPad->SetGrid();

}


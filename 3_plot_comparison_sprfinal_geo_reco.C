#include <iostream>
#include <fstream>
#include <TFile.h>
#include <TTree.h>
#include <TGraphErrors.h>
#include <TMultiGraph.h>

void sprfinal_geo_reco()
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

  Double_t spr_gr_standaloneRoman[25][5];  
  Double_t spr_gr_standalone[25][5];  
  Double_t spr_gr_standalone_4_4[25][5];  

  Double_t spr_standalone[25] = {4.334744, 4.37254, 4.290221, 4.359178, 4.171189, 4.073144, 3.822108, 3.630362, 3.538566, 3.257534,
                                 3.090831, 3.256002, 3.6583, 4.312116, 4.255668, 4.619131, 4.711809, 4.948341, 4.990961, 5.085305,
                                 5.151838, 5.09418, 5.202971, 5.347549, 5.422529};

  TGraphErrors *gr1_standalone = new TGraphErrors();
  TGraphErrors *gr1_standaloneRoman = new TGraphErrors();
  TGraphErrors *gr1_standalone_4_4 = new TGraphErrors();

  TMultiGraph *mg = new TMultiGraph();       

  for (int i = 0; i < 25; i++)
    {
      theta_value[i] = 30 + 5 * i;
      theta[i] = 30 + 5 * i;

      in_standalone[i] = Form((path_standalone + "reco_%1.0d_deg.root").Data(), theta_value[i]);
      file_standalone[i] = TFile::Open(in_standalone[i]);
      tree_standalone[i] = (TTree *)file_standalone[i]->Get("reco");

      // Declare arrays to hold spr values
      Double_t spr[5];

      // Set branch addresses
      tree_standalone[i]->SetBranchAddress("spr", spr);
      tree_standalone[i]->GetEntry(0);

      // Copy values to the arrays for the new file
      for (int j = 0; j < 5; j++)
	{
	  spr_gr_standalone[i][j] = spr[j];
	}
    }

  for (int i = 0; i < 25; i++)
    {
      in_standalone_4_4[i] = Form((path_standalone_4_4 + "reco_%1.0d_deg.root").Data(), theta_value[i]);
      file_standalone[i] = TFile::Open(in_standalone_4_4[i]);
      tree_standalone[i] = (TTree *)file_standalone[i]->Get("reco");

      // Declare arrays to hold spr values
      Double_t spr[5];

      // Set branch addresses
      tree_standalone[i]->SetBranchAddress("spr", spr);
      tree_standalone[i]->GetEntry(0);

      // Copy values to the arrays for the new file
      for (int j = 0; j < 5; j++)
	{
	  spr_gr_standalone_4_4[i][j] = spr[j];
	}
    }

  Double_t spr_standalone_err[25] = {0.3027795, 0.3389978, 0.3827913, 0.3283814, 0.3610204, 0.3236413, 0.3410821, 0.3677715, 0.3847127, 0.3649256, 0.3081383, 0.312083, 0.3479199, 0.3777878, 0.3994845, 0.3541551, 0.3255905, 0.3638946, 0.357591, 0.3258721, 0.3191593, 0.3445663, 0.3149267, 0.3996724, 0.3121762};

  for (int k = 0; k < 25; k++)
    {
      // Add points to the standalone graph
      gr1_standalone->SetPoint(k, theta[k], spr_gr_standalone[k][2]);
      gr1_standalone->SetPointError(k, theta_err[k], spr_standalone_err[k]);

      // Add points to the standaloneRoman graph
      gr1_standaloneRoman->SetPoint(k, theta[k], spr_standalone[k]);
      gr1_standaloneRoman->SetPointError(k, theta_err[k], spr_standalone_err[k]);

      // Add points to the standalone_4_4 graph
      gr1_standalone_4_4->SetPoint(k, theta[k], spr_gr_standalone_4_4[k][2]);
      gr1_standalone_4_4->SetPointError(k, theta_err[k], spr_standalone_err[k]);
    }

  gr1_standalone->SetMarkerStyle(kFullDotLarge);
  gr1_standalone->SetMarkerSize(1);
  gr1_standalone->SetMarkerColor(1);
  gr1_standalone->SetLineColor(1);

  gr1_standaloneRoman->SetMarkerStyle(kFullDotLarge);
  gr1_standaloneRoman->SetMarkerSize(1);
  gr1_standaloneRoman->SetMarkerColor(2);
  gr1_standaloneRoman->SetLineColor(2);

  gr1_standalone_4_4->SetMarkerStyle(kFullDotLarge);
  gr1_standalone_4_4->SetMarkerSize(1);
  gr1_standalone_4_4->SetMarkerColor(3);
  gr1_standalone_4_4->SetLineColor(3);

  mg->Add(gr1_standalone);
  mg->Add(gr1_standaloneRoman);
  mg->Add(gr1_standalone_4_4);

  mg->SetTitle("Single Photon Resolution at 6 GeV/c - geometric reco. for Mcp 6x4 vs 5x4 vs 4x4");
  mg->GetXaxis()->SetTitle("Polar angle (deg)");
  mg->GetYaxis()->SetTitle("Single Photon Resolution (mrad)");
  mg->GetYaxis()->SetRangeUser(0, 10);
  mg->GetXaxis()->SetRangeUser(20, 160);
  mg->GetXaxis()->SetTitleSize(0.05);
  mg->GetYaxis()->SetTitleSize(0.05);
  mg->GetXaxis()->SetTitleOffset(0.8);
  mg->GetYaxis()->SetTitleOffset(0.8);

  mg->Draw("APL");

  gPad->SetGrid();
}

#include <iostream>
#include <fstream>

void combined_I_R_sep_gr_geo_reco()
{
  TFile *file_standalone[25];
  TTree *tree_standalone[25];

  TString path_standalone("/w/halld-scshelf2101/halld3/home/ihossain/reconstruction/reco_files/");
  TString in_standalone[25];
    
  Int_t theta_value[25];
  Double_t theta[25];
  Double_t theta_err[25] = {0};
  
  Double_t sep_standaloneRoman[25] = {3.730761, 3.596965, 3.308513, 3.266788, 3.30443, 3.362408, 3.210066, 3.193811, 3.053817, 3.001219, 2.787778, 3.46087, 4.170896, 2.825622, 2.63589, 2.835406, 2.772732, 2.940102, 3.034555, 3.0925, 3.101411, 3.202123, 3.199491, 3.440447, 3.658279};

  Double_t sep_standalone_errRoman[25] = {0.2030417, 0.2030106, 0.2025419, 0.2026328, 0.2024949, 0.2025293, 0.2025467, 0.2024625, 0.202323, 0.202408, 0.202169, 0.2028398, 0.2033696, 0.2021105, 0.2019752, 0.2021825, 0.2020778, 0.2023317, 0.2023261, 0.2024186, 0.2023646, 0.2025706, 0.2024614, 0.2027412, 0.2030658};
  


  
for(int i=0; i < 25; i++)
    {
      theta_value[i] = 30 + 5*i;
      theta[i] = 30 + 5*i;
    }

  Double_t sep_gr_standalone[25];
  Double_t sep_gr_err_standalone[25];
  Double_t sep_gr, sep_gr_err;

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
      
    }

  TGraphErrors* gr1 = new TGraphErrors(25, theta, sep_gr_standalone, theta_err, sep_gr_err_standalone);
  TGraphErrors* gr2 = new TGraphErrors(25, theta, sep_standaloneRoman, theta_err, sep_standalone_errRoman);

  gr1->SetTitle("Separation Power at 6 GeV/c - geometric reco.");
  gr1->GetXaxis()->SetTitle("Polar angle (deg)");  
  gr1->GetYaxis()->SetTitle("Separation Power (s.d.)");
  gr1->GetYaxis()->SetRangeUser(0,10);
  gr1->GetXaxis()->SetRangeUser(20,160);
  gr1->GetXaxis()->SetTitleSize(0.05);
  gr1->GetYaxis()->SetTitleSize(0.05);  
  gr1->GetXaxis()->SetTitleOffset(0.8);
  gr1->GetYaxis()->SetTitleOffset(0.8);
  
  gr1->SetMarkerStyle(kFullCircle);
  gr1->SetMarkerSize(2);
  gr1->SetMarkerColor(1);
  gr1->SetLineColor(1);
  gr1->Draw("APL");

  gr2->SetMarkerStyle(kFullCircle);
  gr2->SetMarkerSize(1.1);
  gr2->SetMarkerColor(2);
  gr2->SetLineColor(2);
  gr2->Draw("same");


  gPad->SetGrid();

}


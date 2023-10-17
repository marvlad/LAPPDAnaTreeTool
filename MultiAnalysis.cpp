//#define MultiAnalysis_cxx

#include "MultiAnalysis.h"

using namespace std;

MultiAnalysis::MultiAnalysis(const std::string& inputString) : mypath(inputString){
   TTree *tree;
   fChain = 0;
   //if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(mypath.c_str());
      //if (!f || !f->IsOpen()) {
         f = new TFile(mypath.c_str());
      //}
      f->GetObject("ffmytree",tree);
   //}
   Init(tree);
}

void MultiAnalysis::Print(){
	cout << " {+} "<< mypath << endl;
}

MultiAnalysis::~MultiAnalysis()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t MultiAnalysis::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t MultiAnalysis::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void MultiAnalysis::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("StripRMSAmp", StripRMSAmp, &b_StripRMSAmp);
   fChain->SetBranchAddress("StripStandDevAmp", StripStandDevAmp, &b_StripStandDevAmp);
   fChain->SetBranchAddress("StripVarianceAmp", StripVarianceAmp, &b_StripVarianceAmp);
   Notify();
}

Bool_t MultiAnalysis::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void MultiAnalysis::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t MultiAnalysis::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}

void MultiAnalysis::Loop(){
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      cout<<StripVarianceAmp[0]<<endl;
      // if (Cut(ientry) < 0) continue;
    }
}

TH1D* MultiAnalysis::Histogram(const std::string& namehist, int channel){
   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;

   // Define a histogram
   //std::string hname = "Histogram";
   std::string hname = namehist;
   int hbins = 100;
   double in_bin = 0.0;
   double end_bin = 20.0;
   TH1D* h = new TH1D(hname.c_str(),hname.c_str(),hbins, in_bin, end_bin);

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      //h->Fill(StripVarianceAmp[0]);
      h->Fill(StripRMSAmp[channel]);
      //cout<<StripRMSAmp[channel]<<endl;
      //cout<<StripVarianceAmp[0]<<endl;
      // if (Cut(ientry) < 0) continue;
   }
   return h; 
   delete h; 
}

TH2D* MultiAnalysis::Histogram2D(const std::string& namehist){
   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;

   // Define a histogram
   //std::string hname = "Histogram";
   std::string hname = namehist;
   int hbins = 2000;
   double in_bin = 0.0;
   double end_bin = 200.0;
   TH2D* h = new TH2D(hname.c_str(),hname.c_str(),hbins, in_bin, end_bin, 60, 0., 60.);
   for (int channel=0; channel < 60; channel++){
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
	   Long64_t ientry = LoadTree(jentry);
	   if (ientry < 0) break;
	   nb = fChain->GetEntry(jentry);   nbytes += nb;
	   //h->Fill(StripVarianceAmp[0]);
	   h->Fill(StripRMSAmp[channel], channel);
	   //cout<<StripRMSAmp[channel]<<endl;
	   //cout<<StripVarianceAmp[0]<<endl;
	   // if (Cut(ientry) < 0) continue;
	}
   }
   return h; 
   delete h; 
}

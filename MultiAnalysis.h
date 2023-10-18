#define _GLIBCXX_USE_CXX11_ABI 0

#ifndef MULTIANALYSIS_H
#define MULTIANALYSIS_H

#include <TROOT.h>
#include <string>
#include <iostream> 
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <vector>
#include <TColor.h>
#include <TRandom.h>

class MultiAnalysis {
private: 
   std::string mypath;
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   Double_t        StripRMSAmp[60];   //[Nchannels]
   Double_t        StripStandDevAmp[60];   //[Nchannels]
   Double_t        StripVarianceAmp[60];   //[Nchannels]

   TBranch        *b_StripRMSAmp;   //!
   TBranch        *b_StripStandDevAmp;   //!
   TBranch        *b_StripVarianceAmp;   //!

   MultiAnalysis(const std::string& inputString);
   void Print();
   virtual ~MultiAnalysis();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   TH1D *Histogram(const std::string& namehist, int channel);
   TH2D *Histogram2D(const std::string& namehist);
};

#endif

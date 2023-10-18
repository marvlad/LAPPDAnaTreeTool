#include "MultiAnalysis.h"
#include "aux_common.h"

using namespace std;

const std::string mainpath = "/Users/marvinascenciososa/Desktop/pnfs_mrvn/ANNIE/Lab_6/LAPPD39/LAPPD39_output/new_branch/";
const std::string rootfile = "/Analysis.root";

std::string get_path(std::string conf){
 std::string fullpath = mainpath+conf+rootfile;
 return fullpath;
}

void showme_elements(TH1D*h)
{
  cout <<"SHOW ME THE VALUES OF THE HISTOGRAM" <<endl;
  int numBins = h->GetNbinsX();
  cout << "The histogram bins are: " << numBins << endl;
  for (int bin = 1; bin <= numBins; ++bin) {
    double binContent = h->GetBinContent(bin);
    std::cout << "Bin " << bin << ": Content = " << binContent << std::endl;
  }
}

void plot(std::vector<TH1D*> hists, std::vector<std::string> names){
 //std::vector<int>::iterator it;
 TCanvas *c = new TCanvas("C","C",1000,700);
 c->cd();
 TLegend *legend = new TLegend(0.4, 0.7,.9, 0.9);
 for(int i=0; i< hists.size(); i++){
   legend->AddEntry(hists[i], names[i].c_str());
   hists[i]->SetStats(0);
   hists[i]->SetLineWidth(2);
   hists[i]->GetXaxis()->SetTitle("RMS");
   hists[i]->GetYaxis()->SetTitle("Entries");
   hists[i]->SetLineColor(randomColor(i));
   //showme_elements(hists[i]);
   hists[i]->Draw(i==0 ? "hist" : "hist same");
 }
 legend->Draw("same");

 cout<<"[+] Done making the plot ... :)" << endl;
 c->SaveAs("test.png");	
}

void GetHistogram(std::vector<TH1D*> &all_vects, std::vector<std::string> names, int channel){
 TH1D *h;
 MultiAnalysis *LAPPD;
 std::string fullname;

 for(int i=0; i < names.size();i++){
    fullname = get_path(names[i]);
    cout << "The files you are reading is: " << fullname << endl;
    LAPPD = new MultiAnalysis(fullname.c_str());
    h = (TH1D*)LAPPD->Histogram(names[i], channel)->Clone(names[i].c_str());
    all_vects.push_back((TH1D*)h->Clone());
 }
 cout<<"[+] Done making vector histograms ... :)"<<endl;
}

void GetHistogram2D(std::vector<TH2D*> &all_vects, std::vector<std::string> names){
 TH2D *h;
 MultiAnalysis *LAPPD;
 std::string fullname;
 TFile fout("./RMS_channels.root","RECREATE");
 //fout.cd();
 for(int i=0; i < names.size();i++){
    fullname = get_path(names[i]);
    cout << "The files you are reading is: " << fullname << endl;
    LAPPD = new MultiAnalysis(fullname.c_str());
    h = (TH2D*)LAPPD->Histogram2D(names[i])->Clone(names[i].c_str());
    all_vects.push_back((TH2D*)h->Clone());
    TH2D* to_clone = (TH2D*)h->Clone();
    fout.cd();
    to_clone->Write();
    delete to_clone;
 }
 cout<<"[+] Done making vector 2D histograms ... :)"<<endl;
}

//Main code
int main(){

 std::vector<std::string> namedir = {"No_TriggBoard", "ACDCsTriggCable", "TrigCableNOGround","TrigCableNOGround_Connected_to_Spare_PickUpSMA", "No_white_trigg_grounds", "One_red_ground_other_grounds_isolated","No_TriggBoard_after_rg"};

 // MultiAnalyusis objects
 int channel = 5;
 std::vector<TH1D*> HistVect;
 std::vector<TH2D*> HistVect2D;
 GetHistogram(HistVect, namedir, channel);
 //GetHistogram2D(HistVect2D, namedir);
 plot(HistVect, namedir);
 
 return 0;
}

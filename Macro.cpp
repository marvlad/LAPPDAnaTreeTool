#include "MultiAnalysis.h"
#include "aux_common.h"

using namespace std;

std::string get_path(std::string conf, std::string mainpath, std::string rootfile){
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
   hists[i]->SetLineWidth(3);
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

void GetHistogram(std::vector<TH1D*> &all_vects, std::vector<std::string> names, int channel, std::string mainpath, std::string rootfile){
 TH1D *h;
 MultiAnalysis *LAPPD;
 std::string fullname;

 for(int i=0; i < names.size();i++){
    fullname = get_path(names[i],mainpath,rootfile);
    cout << "The files you are reading is: " << fullname << endl;
    LAPPD = new MultiAnalysis(fullname.c_str());
    h = (TH1D*)LAPPD->Histogram(names[i], channel)->Clone(names[i].c_str());
    all_vects.push_back((TH1D*)h->Clone());
 }
 cout<<"[+] Done making vector histograms ... :)"<<endl;
}

void GetHistogram2D(std::vector<TH2D*> &all_vects, std::vector<std::string> names, std::string mainpath, std::string rootfile){
 TH2D *h;
 MultiAnalysis *LAPPD;
 std::string fullname;
 TFile fout("./RMS_channels.root","RECREATE");
 //fout.cd();
 for(int i=0; i < names.size();i++){
    fullname = get_path(names[i],mainpath,rootfile);
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
int main(int argc, char *argv[]) {
  //******************************************************************
  //       CHECK THE ARGUMENTS (CONFIGFILE) 
  //******************************************************************
  // Check for the correct number of command-line arguments
  if (argc != 2) {
      std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
      return 1;
  }

  // Get the configuration file name from the command-line argument
  std::string configFileName = argv[1];

  // Define a map to store the configuration key-value pairs
  std::map<std::string, std::string> configMap;

  // Open the configuration file
  std::ifstream configFile(configFileName);

  if (!configFile) {
      std::cerr << "Failed to open the configuration file: " << configFileName << std::endl;
      return 1;
  }

  // Read the configuration file and populate the map
  std::string line;
  while (std::getline(configFile, line)) {
      // Split each line into key and value
      size_t equalsPos = line.find('=');
      if (equalsPos != std::string::npos) {
          std::string key = line.substr(0, equalsPos);
          std::string value = line.substr(equalsPos + 1);
          configMap[key] = value;
      }
  }

  // Close the configuration file
  configFile.close();

  // -------------------------------------------------------
  // Getting varibles from the configfile
  // -------------------------------------------------------
  std::string mainpath;
  std::string rootfile;
  std::vector<std::string> namedir;
    
  std::vector<std::string> stringVector;
  std::stringstream ss(configMap["namefile"]);
  std::string item;

  // Example: Accessing specific configuration values
  if (configMap.find("namefile") != configMap.end()) {
    std::cout << "namefile: " << configMap["namefile"] << std::endl;
	  while (std::getline(ss, item, ',')) {
          stringVector.push_back(item);
    }
    namedir = stringVector;
  }

  if (configMap.find("mainpath") != configMap.end()) {
      std::cout << "mainpath: " << configMap["mainpath"] << std::endl;
      mainpath = configMap["mainpath"];
  }

  if (configMap.find("rootfile") != configMap.end()) {
      std::cout << "rootfile: " << configMap["rootfile"] << std::endl;
      rootfile = configMap["rootfile"];
  }

  // MultiAnalyusis objects
  int channel = 5;
  std::vector<TH1D*> HistVect;
  std::vector<TH2D*> HistVect2D;
  GetHistogram(HistVect, namedir, channel, mainpath, rootfile);
  //GetHistogram2D(HistVect2D, namedir, mainpath, rootfile);
  plot(HistVect, namedir);
 
 return 0;
}

#ifndef AUX_COMMON_H
#define AUX_COMMON_H

#include "MultiAnalysis.h"

//----------------------------------------------------------------------------------------
// Color function (for people who are color blind)
//----------------------------------------------------------------------------------------
// To add color to the histograms
int randomColor(int seed){
	//Color taken from https://davidmathlogic.com/colorblind/#%23FEFE62-%23D35FB7
        std::string CB_colors[23] = {"#000000", "#0072B2", "#DC3220", "#009E73", "#F0E442",
                                     "#785EF0", "#56B4E9", "#E66100", "#CC79A7", "#994F00",
                                     "#006CD1", "#E66100", "#5D3A9B", "#FEFE62", "#D35FB7",
                                     "#1A85FF", "#2E4052", "#6FDE6E", "#848FA2", "#929084",
                                     "#235FA4", "#BDD9BF", "#004949"};
	TColor color;
	static Int_t c_index;
	TRandom *RN = new TRandom(seed);
	int val = seed;//RN->Integer(23);
	c_index = color.GetColor(CB_colors[val].c_str());
	return c_index;
}
#endif

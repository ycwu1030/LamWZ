#ifndef UTILITIES_H
#define UTILITIES_H

#include "plotheader.h"
#include "LamWZPreAna.h"
#include "TStyle.h"
#include "TH2F.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

void GetHistID(int cat,int &SorB,int &ID);
void createdir(const char *dir);
void SetPlotStyle();
void Analysis(int i);
void LoadingRootFile(TChain *ch,std::string InputDir);

#endif
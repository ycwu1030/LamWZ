#ifndef CutList_H
#define CutList_H
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define DVAR_MAX 10
#define IVAR_MAX 10
#define MAXPOINTS 8


class CutList
{
public:
    CutList();
    ~CutList(){};

    int NVARS;
    int NVARS_D;
    int NVARS_I;
    int NCuts;

    int *VARS_I[IVAR_MAX];
    double *VARS_D[DVAR_MAX];
    std::string VARSNAME_I[IVAR_MAX];
    std::string VARSNAME_D[DVAR_MAX];

    int LowerPoints_D[DVAR_MAX];
    int LowerCurrentPoint_D[DVAR_MAX];
    double LowerBounds_D[DVAR_MAX][MAXPOINTS];
    int UpperPoints_D[DVAR_MAX];
    int UpperCurrentPoint_D[DVAR_MAX];
    double UpperBounds_D[DVAR_MAX][MAXPOINTS];

    int LowerPoints_I[IVAR_MAX];
    int LowerCurrentPoint_I[IVAR_MAX];
    int LowerBounds_I[IVAR_MAX][MAXPOINTS];
    int UpperPoints_I[IVAR_MAX];
    int UpperCurrentPoint_I[IVAR_MAX];
    int UpperBounds_I[IVAR_MAX][MAXPOINTS];

    void addCut(double *variable, std::string name, double start_low, double end_low, int steps_low, double start_up, double end_up, int steps_up);
    void addCut(int *variable, std::string name, int start_low, int end_low, int steps_low, int start_up, int end_up, int steps_up);
    bool Apply(int i);// Apply i-th cut.
    std::string CutInfo(int i);
    
};


#endif
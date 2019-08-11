#ifndef CutList_H
#define CutList_H
#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define DVAR_MAX 10
#define IVAR_MAX 10
#define STEPS 8


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

    int LowerSteps_D[DVAR_MAX];
    int LowerCurrent_D[DVAR_MAX];
    double LowerBounds_D[DVAR_MAX][STEPS];
    int UpperSteps_D[DVAR_MAX];
    int UpperCurrent_D[DVAR_MAX];
    double UpperBounds_D[DVAR_MAX][STEPS];

    int LowerSteps_I[IVAR_MAX];
    int LowerCurrent_I[IVAR_MAX];
    int LowerBounds_I[IVAR_MAX][STEPS];
    int UpperSteps_I[IVAR_MAX];
    int UpperCurrent_I[IVAR_MAX];
    int UpperBounds_I[IVAR_MAX][STEPS];

    void addCut(double *variable, double start_low, double end_low, int steps_low, double start_up, double end_up, int steps_up);
    void addCut(int *variable, int start_low, int end_low, int steps_low, int start_up, int end_up, int steps_up);
    bool Apply(int i);// Apply i-th cut.
    
};


#endif
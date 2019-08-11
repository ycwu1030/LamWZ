#include "CutList.h"


CutList::CutList()
{
    NVARS_D = 0;
    NVARS_I = 0;
    NCuts = 0;
}
void CutList::addCut(double *variable, double start_low, double end_low, int steps_low, double start_up, double end_up, int steps_up)
{
    VARS_D[NVARS_D] = variable;
    if (start_low<0)
    {
        LowerSteps_D[NVARS_D] = 0;
    }
    else
    {
        if (steps_low + 1 > STEPS) {steps_low = STEPS-1; std::cout<<"Exceed the UP limit of cut steps, change to default MAX value."<<std::endl;}
        LowerSteps_D[NVARS_D] = steps_low + 1;
        for (int j = 0; j < steps_low+1; ++j)
        {
            if (steps_low == 0)
            {
                LowerBounds_D[NVARS_D][j] = start_low;
            }
            else
            {
                LowerBounds_D[NVARS_D][j] = start_low + j*(end_low-start_low)/(1.0*steps_low);
            }
        }
        if (NCuts==0)
        {
            NCuts = steps_low+1;
        }
        else
        {
            NCuts *= steps_low+1;
        }
        LowerCurrent_D[NVARS_D] = 0;
    }

    if (start_up<0)
    {
        UpperSteps_D[NVARS_D] = 0;
    }
    else
    {
        if (steps_up + 1 > STEPS) {steps_up = STEPS-1; std::cout<<"Exceed the UP limit of cut steps, change to default MAX value."<<std::endl;}
        UpperSteps_D[NVARS_D] = steps_up + 1;
        for (int j = 0; j < steps_up+1; ++j)
        {
            if (steps_up == 0)
            {
                UpperBounds_D[NVARS_D][j] = start_up;
            }
            else
            {
                UpperBounds_D[NVARS_D][j] = start_up + j*(end_up-start_up)/(1.0*steps_up);
            }
        }
        if (NCuts==0)
        {
            NCuts = steps_up+1;
        }
        else
        {
            NCuts *= steps_up+1;
        }
        UpperCurrent_D[NVARS_D] = 0;
    }
    NVARS_D += 1;
    NVARS += 1;
}
void CutList::addCut(int *variable, int start_low, int end_low, int steps_low, int start_up, int end_up, int steps_up)
{
    VARS_I[NVARS_I] = variable;
    if (start_low<0)
    {
        LowerSteps_I[NVARS_I] = 0;
    }
    else
    {
        if (steps_low + 1 > STEPS) {steps_low = STEPS-1; std::cout<<"Exceed the UP limit of cut steps, change to default MAX value."<<std::endl;}
        LowerSteps_I[NVARS_I] = steps_low + 1;
        for (int j = 0; j < steps_low+1; ++j)
        {
            if (steps_low == 0)
            {
                LowerBounds_I[NVARS_I][j] = start_low;
            }
            else
            {
                LowerBounds_I[NVARS_I][j] = start_low + j*(end_low-start_low)/(1.0*steps_low);
            }
        }
        if (NCuts==0)
        {
            NCuts = steps_low+1;
        }
        else
        {
            NCuts *= steps_low+1;
        }
        LowerCurrent_I[NVARS_I] = 0;
    }

    if (start_up<0)
    {
        UpperSteps_I[NVARS_I] = 0;
    }
    else
    {
        if (steps_up + 1 > STEPS) {steps_up = STEPS-1; std::cout<<"Exceed the UP limit of cut steps, change to default MAX value."<<std::endl;}
        UpperSteps_I[NVARS_I] = steps_up + 1;
        for (int j = 0; j < steps_up+1; ++j)
        {
            if (steps_up == 0)
            {
                UpperBounds_I[NVARS_I][j] = start_up;
            }
            else
            {
                UpperBounds_I[NVARS_I][j] = start_up + j*(end_up-start_up)/(1.0*steps_up);
            }
        }
        if (NCuts==0)
        {
            NCuts = steps_up+1;
        }
        else
        {
            NCuts *= steps_up+1;
        }
        UpperCurrent_I[NVARS_I] = 0;
    }
    NVARS_I += 1;
    NVARS += 1;
}

bool CutList::Apply(int i)
{
    bool good = true;
    for (int ivarI = 0; ivarI < NVARS_I; ++ivarI)
    {
        if (LowerSteps_I[ivarI] > 0)
        {
            good *= ((*VARS_I[ivarI])>=LowerBounds_I[ivarI][LowerCurrent_I[ivarI]]);
            LowerCurrent_I[ivarI] += 1;
            if (LowerCurrent_I[ivarI] >= LowerSteps_I[ivarI]) LowerCurrent_I[ivarI] = 0;
        }
        if (UpperSteps_I[ivarI] > 0)
        {
            good *= ((*VARS_I[ivarI])<=UpperBounds_I[ivarI][UpperCurrent_I[ivarI]]);
            UpperCurrent_I[ivarI] += 1;
            if (UpperCurrent_I[ivarI] >= UpperSteps_I[ivarI]) UpperCurrent_I[ivarI] = 0;
        }
    }
    for (int ivarD = 0; ivarD < NVARS_D; ++ivarD)
    {
        if (LowerSteps_D[ivarD] > 0)
        {
            good *= ((*VARS_D[ivarD])>=LowerBounds_D[ivarD][LowerCurrent_D[ivarD]]);
            LowerCurrent_D[ivarD] += 1;
            if (LowerCurrent_D[ivarD] >= LowerSteps_D[ivarD]) LowerCurrent_D[ivarD] = 0;
        }
        if (UpperSteps_D[ivarD] > 0)
        {
            good *= ((*VARS_D[ivarD])<=UpperBounds_D[ivarD][UpperCurrent_D[ivarD]]);
            UpperCurrent_D[ivarD] += 1;
            if (UpperCurrent_D[ivarD] >= UpperSteps_D[ivarD]) UpperCurrent_D[ivarD] = 0;
        }
    }
    return good;
}

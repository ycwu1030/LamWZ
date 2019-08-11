#include "CutList.h"


CutList::CutList()
{
    NVARS_D = 0;
    NVARS_I = 0;
    NCuts = 0;
}
void CutList::addCut(double *variable, std::string name, double start_low, double end_low, int steps_low, double start_up, double end_up, int steps_up)
{
    VARS_D[NVARS_D] = variable;
    VARSNAME_D[NVARS_D] = name;
    if (steps_low<0)
    {
        LowerPoints_D[NVARS_D] = -1;
    }
    else
    {
        if (steps_low + 1 > MAXPOINTS) {steps_low = MAXPOINTS-1; std::cout<<"Exceed the UP limit of cut steps, change to default MAX value."<<std::endl;}
        LowerPoints_D[NVARS_D] = steps_low + 1;
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
        LowerCurrentPoint_D[NVARS_D] = 0;
    }

    if (start_up<0)
    {
        UpperPoints_D[NVARS_D] = -1;
    }
    else
    {
        if (steps_up + 1 > MAXPOINTS) {steps_up = MAXPOINTS-1; std::cout<<"Exceed the UP limit of cut steps, change to default MAX value."<<std::endl;}
        UpperPoints_D[NVARS_D] = steps_up + 1;
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
        UpperCurrentPoint_D[NVARS_D] = 0;
    }
    NVARS_D += 1;
    NVARS += 1;
}
void CutList::addCut(int *variable, std::string name, int start_low, int end_low, int steps_low, int start_up, int end_up, int steps_up)
{
    VARS_I[NVARS_I] = variable;
    VARSNAME_I[NVARS_I] = name;
    if (start_low<0)
    {
        LowerPoints_I[NVARS_I] = 0;
    }
    else
    {
        if (steps_low + 1 > MAXPOINTS) {steps_low = MAXPOINTS-1; std::cout<<"Exceed the UP limit of cut steps, change to default MAX value."<<std::endl;}
        LowerPoints_I[NVARS_I] = steps_low + 1;
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
        LowerCurrentPoint_I[NVARS_I] = 0;
    }

    if (start_up<0)
    {
        UpperPoints_I[NVARS_I] = 0;
    }
    else
    {
        if (steps_up + 1 > MAXPOINTS) {steps_up = MAXPOINTS-1; std::cout<<"Exceed the UP limit of cut steps, change to default MAX value."<<std::endl;}
        UpperPoints_I[NVARS_I] = steps_up + 1;
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
        UpperCurrentPoint_I[NVARS_I] = 0;
    }
    NVARS_I += 1;
    NVARS += 1;
}

bool CutList::Apply(int i)
{
    // bool good = true;
    // for (int ivarI = 0; ivarI < NVARS_I; ++ivarI)
    // {
    //     if (LowerPoints_I[ivarI] > 0)
    //     {
    //         good *= ((*VARS_I[ivarI])>=LowerBounds_I[ivarI][LowerCurrentPoint_I[ivarI]]);
    //         LowerCurrentPoint_I[ivarI] += 1;
    //         if (LowerCurrentPoint_I[ivarI] >= LowerPoints_I[ivarI]) LowerCurrentPoint_I[ivarI] = 0;
    //     }
    //     if (UpperPoints_I[ivarI] > 0)
    //     {
    //         good *= ((*VARS_I[ivarI])<=UpperBounds_I[ivarI][UpperCurrentPoint_I[ivarI]]);
    //         UpperCurrentPoint_I[ivarI] += 1;
    //         if (UpperCurrentPoint_I[ivarI] >= UpperPoints_I[ivarI]) UpperCurrentPoint_I[ivarI] = 0;
    //     }
    // }
    // for (int ivarD = 0; ivarD < NVARS_D; ++ivarD)
    // {
    //     if (LowerPoints_D[ivarD] > 0)
    //     {
    //         good *= ((*VARS_D[ivarD])>=LowerBounds_D[ivarD][LowerCurrentPoint_D[ivarD]]);
    //         LowerCurrentPoint_D[ivarD] += 1;
    //         if (LowerCurrent_D[ivarD] >= LowerSteps_D[ivarD]) LowerCurrent_D[ivarD] = 0;
    //     }
    //     if (UpperSteps_D[ivarD] > 0)
    //     {
    //         good *= ((*VARS_D[ivarD])<=UpperBounds_D[ivarD][UpperCurrent_D[ivarD]]);
    //         UpperCurrent_D[ivarD] += 1;
    //         if (UpperCurrent_D[ivarD] >= UpperSteps_D[ivarD]) UpperCurrent_D[ivarD] = 0;
    //     }
    // }
    int NCOUNTS = 1;
    int cp=1;
    bool good = true;
    for (int ivarI = 0; ivarI < NVARS_I; ++ivarI)
    {
        if (LowerPoints_I[ivarI] > 0)
        {
            cp = int(i/NCOUNTS)%LowerPoints_I[ivarI];
            good *= ((*VARS_I[ivarI])>=LowerBounds_I[ivarI][cp]);
            NCOUNTS*=LowerPoints_I[ivarI];
        }
        if (UpperPoints_I[ivarI] > 0)
        {
            cp = int(i/NCOUNTS)%UpperPoints_I[ivarI];
            good *= ((*VARS_I[ivarI])<=UpperBounds_I[ivarI][cp]);
            NCOUNTS*=UpperPoints_I[ivarI];
        }
    }
    for (int ivarD = 0; ivarD < NVARS_D; ++ivarD)
    {
        if (LowerPoints_D[ivarD] > 0)
        {
            cp = int(i/NCOUNTS)%LowerPoints_D[ivarD];
            good *= ((*VARS_D[ivarD])>=LowerBounds_D[ivarD][cp]);
            NCOUNTS *= LowerPoints_D[ivarD];
        }
        if (UpperPoints_D[ivarD] > 0)
        {
            cp = int(i/NCOUNTS)%UpperPoints_D[ivarD];
            good *= ((*VARS_D[ivarD])<=UpperBounds_D[ivarD][cp]);
            NCOUNTS *= UpperPoints_D[ivarD];
        }
    }
    return good;
}

std::string CutList::CutInfo(int i)
{
    std::string info;
    std::stringstream sst;
    int NCOUNTS = 1;
    int cp=1;
    for (int ivarI = 0; ivarI < NVARS_I; ++ivarI)
    {
        if (LowerPoints_I[ivarI] > 0)
        {
            cp = int(i/NCOUNTS)%LowerPoints_I[ivarI];
            sst << "(" << LowerBounds_I[ivarI][cp] << "<=" << VARSNAME_I[ivarI] << ")";
            NCOUNTS*=LowerPoints_I[ivarI];
        }
        if (UpperPoints_I[ivarI] > 0)
        {
            cp = int(i/NCOUNTS)%UpperPoints_I[ivarI];
            sst << "(" << VARSNAME_I[ivarI] << "<=" << UpperBounds_I[ivarI][cp] << ")";
            NCOUNTS*=UpperPoints_I[ivarI];
        }
    }
    for (int ivarD = 0; ivarD < NVARS_D; ++ivarD)
    {
        if (LowerPoints_D[ivarD] > 0)
        {
            cp = int(i/NCOUNTS)%LowerPoints_D[ivarD];
            sst << "(" << LowerBounds_D[ivarD][cp] << "<=" << VARSNAME_D[ivarD] << ")";
            NCOUNTS *= LowerPoints_D[ivarD];
        }
        if (UpperPoints_D[ivarD] > 0)
        {
            cp = int(i/NCOUNTS)%UpperPoints_D[ivarD];
            sst << "(" << VARSNAME_D[ivarD] << "<=" << UpperBounds_D[ivarD][cp] << ")";
            NCOUNTS *= UpperPoints_D[ivarD];
        }
    }
    sst >> info;
    return info;
}

#ifndef AFTERBURNER_H
#define AFTERBURNER_H
#include"mixer.h"

extern double P7C, T7C, far7, Wg7, Wfaf;

class AfterBurner
{
private:
    double nAfterBurn, dAir;
    double T1C;
    double tprc_Afterburn;
    static int ROW;
    struct lpAfterBurn
    {
        double* tprc_Max = new double[ROW];
        double* tprc_Mil1 = new double[ROW];
        double* tprc_Mil2 = new double[ROW];
        double* T1C = new double[ROW];
        int nPoints_tprc;
        double co_tprc_Max, co_tprc_Mil1, co_tprc_Mil2;
    };

public:
     AfterBurner(int MAX_POINTS);
};

#endif // AFTERBURNER_H

#ifndef NOZZLE_H
#define NOZZLE_H
#include"afterburner.h"
#include"inlet.h"

extern double A8_calcu,Wa2;

class Nozzle
{
private:
    double dAir, A8_DES;
    double  A9, Wg7;
    double pamp,P9S;
    double Lamd8, Lamd9;
    double C9,cd9;
    double F,sfc;
    struct lpNozzle
    {
        int nPoints_A9, nPoints_PN;
        double** CD9;
        double *P7SP9, *A9SA8;
        double Co_Cd9;

    };

public:
     Nozzle(int MAX_POINTS);
};

#endif // NOZZLE_H

#include "aerodynamic_function.h"
#include<cmath>


double LamdQ(double W, double K, double E)
{
    double A = pow((K + 1) / 2, 1 / (K - 1));
    double ans = 1.0;
    double err;
    double B = (K - 1) / (K + 1);
    double C = 1 / (K - 1);
    err = W - (A * ans * pow((1 - B * ans * ans), C));
    do
    {
        ans += 0.1;
        err = W - (A * ans * pow((1 - B * ans * ans), C));
    }
    while(fabs(err) > 1e-5);
    return ans;
}

double Qlamd(double K, double lamd)
{
    double Q;
    Q = pow((K + 1) / 2, (1 / (K - 1))) * lamd * pow((1 - (K - 1) / (K + 1) * lamd * lamd), (1 / (K - 1)));
    return Q;
}

double Pilamd(double K, double lamd)
{
    double Pi;
    Pi = pow((1 - (K - 1) / (K + 1) * lamd * lamd), (K / (K - 1)));
    return Pi;
}

double Flamd(double K, double lamd)
{
    double F, Q, Z;
    Q = pow(((K + 1) / 2), (1 / (K - 1))) * lamd * pow((1 - (K - 1) / (K + 1) * lamd * lamd), (1 / (K - 1)));
    Z = lamd + (1 / lamd);
    F = pow((2 / (K + 1)), (1 / (K - 1))) * Q * Z;
    return F;
}

double LamdPi(double Pi, double K)
{
    double lamd;
    lamd = sqrt((pow(Pi, ((K - 1) / K)) - 1) * (K + 1) / (K - 1));
    return lamd;
}

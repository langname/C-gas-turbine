#include "gas.h"
#include<cmath>

Gas::Gas()
{

}

void Atmosphere(double Alt, double Pamp, double Tamp, double delta_temp, double delta_press)
{
        if (Alt <= 11.0)
        {
            Pamp = 1.01325 * pow(1.0 - Alt / 44.308, 5.2553);
            Tamp = 288.15 - 6.5 * Alt;
        }
        else
        {
            Pamp = 0.022615 * exp((11.0 - Alt) / 6.338);
            Tamp = 216.5;
        }
        Tamp += delta_temp;
        Pamp += delta_press;

}

double BlankWater(char ch, double t)
{
    double as0 = 0.23113968e3;
       double a[9] = { -0.25179888e6,+0.33930851e5,
       -0.43630281e4,+0.10594936e5,
       -0.54952291e4,+0.14919330e4,
       -0.22772255e3,+0.17874978e2,-0.53236046 };
       double z1 = 0, z2 = 0;
       double tt, dtemp, H2O = 0.01801534;
       int i;
       tt = t * 0.001;
       switch (ch)
       {
       case'H':
       case'h':
           for (i = 0; i < 9; i++)
           {
               dtemp = pow(tt, i);
               z1 += a[i] * dtemp;
               z2 += a[i] * pow(298.15 * 0.001, i);
           }
           z1 = z1 - z2;
           break;
       case'C':
       case'c':
           for (i = 1; i < 9; i++)
           {
               dtemp = 0.001 * pow(tt, i - 1);
               z1 += i * a[i] * dtemp;
           }
           break;
       case'S':
       case's':
           dtemp = log(tt);
           z1 = 0.001 * a[1] * dtemp;
           for (i = 2; i < 9; i++)
           {
               dtemp = 0.001 * pow(tt, i - 1);
               z1 += i * a[i] * dtemp / (i - 1);

           }
           z1 += as0;
           dtemp = z1 / H2O;
           return dtemp;
       }
}

double BlankAir(char ch, double t, double f)
{
    double as0 = 0.80558643e4, bs0 = -0.17800633e4;
        double a[9] = { -0.30183674e6,+0.10489652e7,
                     -0.23284057e6,+0.45288431e6,
                     -31308477e6,+0.11341362e6,
                     -0.21298087e5,+0.16363600e4,0.0 };
        double b[9] = { -0.11152575e6,-0.31020306e6,
                        +0.29961197e7,-0.27934788e7,
                        +0.18746407e7,-0.73499597e6,
                        +0.15062602e6,-0.12510984e5,0.0 };
        double z1 = 0, z2 = 0;
        double tt, dtemp;
        int i;
        tt = t * 0.001;
        switch (ch)
        {
        case'H':
        case'h':
            for (i = 0; i < 9; i++)
            {
                dtemp = pow(tt, i);
                z1 += a[i] * dtemp;
                z2 += b[i] * dtemp;
            }
            break;
        case 'C':
        case'c':
            for (i = 0; i < 9; i++)
            {
                dtemp = 0.001 * pow(tt, i - 1);
                z1 += i * a[i] * dtemp;
                z2 += i * b[i] * dtemp;
            }
            break;
        case'S':
        case's':
            dtemp = log(tt);
            z1 = 0.001 * a[1] * dtemp;
            z2 = 0.001 * b[1] * dtemp;
            for (i = 2; i < 9; i++)
            {
                dtemp = 0.001 * pow(tt, i - 1);
                z1 += i * a[i] * dtemp / (i - 1);
                z2 += i * b[i] * dtemp / (i - 1);
            }
            z1 += as0;
            z2 += bs0;

        }
        dtemp = z1 + f / (1 + f) * z2;
        return dtemp;
}

double Air(char ch, double t, double f, double d)
{
    double dtemp;
       dtemp = ((1 + f) * BlankAir(ch, t, f) + d * BlankWater(ch, t)) / (1 + f + d);
       return dtemp;
}

double ddd(double t, double d)
{
    const double fst = 0.0676372;
        double L, dtemp;
        L = 1 / fst;
        dtemp = (1 + L) * Air('H', t, fst, d) - L * Air('H', t, 0, d);
        return dtemp;
}

void TFromHS(char ch, double &t, double hs, double gry, double dAir)
{
    int n = 0;
       double t1, t2, t3, hs1, hs2, hs3;
       t1 = 200;
       t2 = 3000;
       hs1 = Air(ch, t1, gry, dAir);
       if (fabs((hs1 - hs) / hs) < 1.0e-8) { t = t1; return; }
       hs2 = Air(ch, t2, gry, dAir);
       if (fabs((hs2 - hs) / hs) < 1.0e-8) { t = t2; return; }
       while (n < 30)
       {
           t3 = (t2 + t1) / 2;
           hs3 = Air(ch, t3, gry, dAir);
           if (fabs((hs3 - hs) / hs) < 1.0e-6) { t = t3; return; }
           if (hs3 > hs) { t2 = t3; hs2 = hs3; }
           else { t1 = t3; hs1 = hs3; }
           n++;
       }
       t = 9999;//错误标志
}

void CpKRFromT(double &Cp, double &K, double &R, double tAir, double gry=0, double dAir=0)
{
    double bm, mg, H2O = 18.01534;
        Cp = Air('C', tAir, gry, dAir);
        bm = 28.9644 * (1 - 0.308765 * gry);
        mg= (1 + gry + dAir) / ((1 + gry) / bm + dAir / H2O);
        R = 8314.298 / mg;
        K = Cp / (Cp - R);
}

#include "bypass.h"
#include<cmath>
#include "aerodynamic_function.h"
#include "calcu_fuction.h"
#include "gas.h"
#include"compressor.h"


Bypass::Bypass()
{
    double Cp, rAir, kAir, ckm, l25, ppx ;
    lpByPass lpByPass1;
    //计算外涵道空气流量
    Wa22=Wa2C-Wa2B;
    if (lpByPass1.Mode == 0)
        tprc_ByPass = lpEngine1->tprc25;
    else
    {
        CpKRFromT(Cp,kAir,rAir,T22C,0,dAir);
        ckm = sqrt(kAir/rAir*pow(2.0/(kAir+1),(kAir+1)/(kAir - 1)));
        l25 = LamdQ(Wa22 * sqrt(T22C) / P22C / ckm / lpEngine1->A22 / 1e+5, kAir, 0);
        ppx = kAir / (kAir - 1) * l25 * l25 * pow(1.0 - (kAir - 1) / (kAir + 1.0) * l25 * l25, 1.0 / (kAir - 1));
        if (bDesign)
            lpByPass1.Co_tprc = (1 - lpEngine1->tprc25) / ppx;
        P25C = P2BC * tprc_ByPass;
        T25C = T22C * lpByPass1.T25CdT22C;
    }
}

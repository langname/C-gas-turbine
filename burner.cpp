#include "burner.h"
#include<cmath>
#include "aerodynamic_function.h"
#include "calcu_fuction.h"
#include "gas.h"
#include"global_variables.h"

Burner::Burner()
{
    double Cp, k, r, ckm, qlamd, lamd3, qry;
    CpKRFromT(Cp,k,r,T3C,0,dAir);
    ckm = sqrt(k/r*pow(2/(k+1),(k+1)/(k-1)));
    qlamd = Wa3 * sqrt(T3C) / P3C / ckm / lpEngine1->A3 / 1e5;
    lamd3 = LamdQ(qlamd, k, 0);
    if (bDesign)
    {
        //在设计点，计算常系数Co_tprc
        Co_tprc = (1 - lpEngine1->tprc_Burn) / lamd3 / lamd3;

    }
    //计算主燃烧室总压恢复系数以及出口总压
    tprc_Burn = 1 - Co_tprc * lamd3 * lamd3;
    P4C = P3C * tprc_Burn;
    //计算油气比，燃油质量流量
    qry = Air('H', T4C, 0, 0) - Air('H', T3C, 0, 0) + dAir * (BlankWater('H', T4C) - BlankWater('H', T3C));
    double farb = lpEngine1->eff_Burn * lpEngine1->FHV_Burn * 4184.0 - ddd(T4C, dAir);//油气比
    farb = qry / farb;
    Wfb = farb * Wa3;
}

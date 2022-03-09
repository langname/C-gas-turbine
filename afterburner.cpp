#include "afterburner.h"
#include<cmath>
#include "aerodynamic_function.h"
#include "calcu_fuction.h"
#include "gas.h"
#include"global_variables.h"

AfterBurner::AfterBurner(int MAX_POINTS)
{
        int i;
        double* tprc_Array1=new double[MAX_POINTS];
        double* tprc_Array2 = new double[MAX_POINTS];
        double* tprc_Array3 = new double[MAX_POINTS];
        double* T1C_Array = new double[MAX_POINTS];
        double tprc_Max, tprc_Mil1, tprc_Mil2, qry;
        lpAfterBurn lpAfterBurn1;

        //非加力工作状态
        if (nAfterBurn <= 0)
        {
            //插值求加力燃烧室工作点参数
            for (i = 0; i < lpAfterBurn1.nPoints_tprc; i++)
            {
                tprc_Array1[i] = lpAfterBurn1.tprc_Max[i];
                tprc_Array2[i] = lpAfterBurn1.tprc_Mil1[i];
                tprc_Array3[i] = lpAfterBurn1.tprc_Mil2[i];
                T1C_Array[i]   = lpAfterBurn1.T1C[i];
            }
            Qip(lpAfterBurn1.nPoints_tprc, T1C_Array, tprc_Array1, T1C, tprc_Max);
            Qip(lpAfterBurn1.nPoints_tprc, T1C_Array, tprc_Array2, T1C, tprc_Mil1);
            Qip(lpAfterBurn1.nPoints_tprc, T1C_Array, tprc_Array3, T1C, tprc_Mil2);

            //设计状态求耦合系数
            if (bDesign)
            {
                lpAfterBurn1.co_tprc_Max = lpEngine1->tprc_AfterBurn1 / tprc_Max;
                lpAfterBurn1.co_tprc_Mil1 = lpEngine1->tprc_AfterBurn2 / tprc_Mil1;
                lpAfterBurn1.co_tprc_Mil2 = lpEngine1->tprc_AfterBurn3 / tprc_Mil2;
            }
            double tprc_Afterburn = lpAfterBurn1.co_tprc_Max * tprc_Max;
            P7C = P6C * tprc_Afterburn;
            T7C = T6C;
            far7 = far6;
            Wg7 = Wg6;
            Wfaf = 0;
        }
        else
        {
            //加力状态根基加力温度求加力燃烧室燃油流量和油气比
            qry = Air('H', T7C, 0, 0) - Air('H', T6C, 0, 0) + dAir * (BlankWater('H', T7C) - BlankWater('H', T6C)) + far6 * (ddd(T7C, 0) - ddd(T6C, 0));
            far7 = lpEngine1->eff_AfterBurn * lpEngine1->FHV_AfterBurn * 4184.0 - ddd(T7C, dAir);
            far7 = qry / far7;
            Wfaf = far7 * (Wg6 - Wfb);//加力燃烧室燃油流量
            far7 = far7 + far6;
            for (i = 0; i < lpAfterBurn1.nPoints_tprc; i++)
            {
                tprc_Array1[i] = lpAfterBurn1.tprc_Mil2[i];
                T1C_Array[i] = lpAfterBurn1.T1C[i];

            }
            Qip(lpAfterBurn1.nPoints_tprc, T1C_Array, tprc_Array1, T1C, tprc_Mil2);
            tprc_Afterburn = tprc_Mil2;
            P7C = P6C * tprc_Afterburn;
            Wg7 = Wg6 + Wfaf;
        }
}

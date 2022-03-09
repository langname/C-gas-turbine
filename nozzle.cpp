#include "nozzle.h"
#include<cmath>
#include "aerodynamic_function.h"
#include "calcu_fuction.h"
#include "gas.h"
#include"global_variables.h"

 Nozzle::Nozzle(int MAX_POINTS)
{
    int i, j;
    double Cp, K, R, Ckm8, QLamd8, A9Max, ftemp, QLamd9;
    double Pa3;
    double* tempArray1 = new double[MAX_POINTS];
    double* tempArray2 = new double[MAX_POINTS];
    lpNozzle lpNozzle1;

    //缩放喷管
    if (lpEngine1->Mode_Nozzle == 1)
    {
        //求喉部M=1时的最大反压
        CpKRFromT(Cp, K, R, T7C, far7, dAir);
        Ckm8 = sqrt(K / R * pow(2.0 / (K + 1.0), (K + 1.0) / (K - 1.0)));
        A8_calcu = Wg7 * sqrt(T7C) / P7C / Ckm8 / 1.0E+5;
        if (bDesign)A8_DES = A8_calcu;
        ftemp = 1.0 / lpEngine1->MinA9dA8;
        ftemp = LamdQ(ftemp, K, 0);
        Pa3 = P7C * Pilamd(K, ftemp);

        //喉部为亚声速气流，喷管为亚临界状态
        if (pamp > Pa3)
        {
            double P9S = pamp;
            Lamd8 = LamdQ(QLamd8, K, 0);
            Lamd9 = LamdQ(QLamd8 / lpEngine1->MinA9dA8, K, 0);
            A8_calcu = A8_calcu / QLamd8;
            A9 = A8_calcu * lpEngine1->MinA9dA8;
            Lamd9 = LamdQ(QLamd9, K, 0);
            QLamd8 = QLamd9 * lpEngine1->MinA9dA8;
            Lamd8 = LamdQ(QLamd8, K, 0);
            A8_calcu = A8_calcu / QLamd8;
            A9 = A8_calcu * lpEngine1->MinA9dA8;
        }

        //喉部速度为音速，喷管为临界状态或超临界状态
        else
        {
            P9S = pamp;
            QLamd8 = 1.0;
            Lamd8= 1.0;
            Lamd9 = LamdPi(pamp / P7C, K);
            A9 = A8_calcu / Qlamd(K, Lamd9);
            A9Max = A8_calcu * lpEngine1->MinA9dA8;

            //到达A9限制状态，不完全膨胀状态
            if (A9 > A9Max)
            {
                A9 = A9Max;
                Lamd9 = LamdQ(A8_calcu / A9, K, 1);
                P9S = P7C * Pilamd(K, Lamd9);
            }
        }

        //计算喷管出口气流速度
        C9 = Lamd9 * sqrt(2 * K * R * T7C / (K + 1));

        //插值求喷管特性图上工作点参数
        for (i = 0; i <lpNozzle1.nPoints_A9 ; i++)
        {
            for (j = 0; j < lpNozzle1.nPoints_PN; j++)
            {
                tempArray1[j] = lpNozzle1.CD9[i][j];
            }
            Qip(lpNozzle1.nPoints_PN, lpNozzle1.P7SP9, tempArray1, P7C / P9S, tempArray2[i]);
        }
        Qip(lpNozzle1.nPoints_A9, lpNozzle1.A9SA8, tempArray2, A9 / A8_calcu, cd9);

        //设计状态 求速度系数的耦合系数
        if (bDesign) lpNozzle1.Co_Cd9 = lpEngine1->cd9 / cd9;

        cd9 = cd9 * lpNozzle1.Co_Cd9;
    }

    //收缩喷管
    if (lpEngine1->Mode_Nozzle == 0)
    {
        //求喷管临界截面面积
        CpKRFromT(Cp, K, R, T7C, far7, dAir);
        Ckm8 = sqrt(K / R * pow(2.0 / (K + 1.0), (K + 1.0) / (K - 1.0)));
        A8_calcu = Wg7 * sqrt(T7C) / P7C / Ckm8 / 1.0E+5;
        if (bDesign)  A8_DES = A8_calcu;

        //判断尾喷管状态
        Lamd8 = Lamd9 = LamdPi(pamp / P7C, K);

        //临界 、超临界状态
        if (Lamd9 >= 1.0)
        {
            P9S = P7C / (pow((K + 1) / 2, K / (K - 1)));
            Lamd9 = 1.0;
            QLamd8 = 1.0;
            C9 = sqrt(2 * K / (1 + K) * R * T7C);
        }

        //亚临界状态
        else
        {
            P9S = pamp;
            QLamd8 = Qlamd(K, Lamd9);
            C9 = sqrt(2 * K / (K - 1) * R * T7C * (1 - 1 / pow(P7C / pamp, (K - 1) / K)));

        }
        Ckm8 = sqrt(K / R * pow(2.0 / (K + 1.0), (K + 1.0) / (K - 1.0)));
        A9 = A8_calcu = Wg7 * sqrt(T7C) / P7C / Ckm8 / 1.0E+5 /QLamd8;

        //插值求尾喷管速度系数
        for (i=0;i<lpNozzle1.nPoints_A9;i++)
        {
            for (j = 0; j < lpNozzle1.nPoints_PN; j++)
                tempArray1[j] = lpNozzle1.CD9[i][j];
            Qip(lpNozzle1.nPoints_PN, lpNozzle1.P7SP9, tempArray1, P7C / P9S, tempArray2[i]);
        }
        Qip(lpNozzle1.nPoints_A9, lpNozzle1.A9SA8, tempArray2, A9 / A8_calcu, cd9);
        if (bDesign) lpNozzle1.Co_Cd9 = lpEngine1->cd9 / cd9;
        cd9 = cd9 * lpNozzle1.Co_Cd9;
    }

    //求发动机推力  耗油率
    F = C9 * cd9 * Wg7 - Wa2 * Vtas + (P9S - pamp) * A9 * 1.0E+5;
    F = F / 9.80665;
    sfc = (Wfb + Wfaf) * 3600.0 / F;
}

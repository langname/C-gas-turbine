#include "mixer.h"
#include<cmath>
#include "aerodynamic_function.h"
#include "calcu_fuction.h"
#include "gas.h"
#include"global_variables.h"

 Mixer::Mixer()
{
    double Cp, K1, K2, K3, R;
    double QA, QB, QC, H6,ckm;
    double Lamd25, Lamd5, Lamd6;
    //设计状态根据混合器进口总面积，利用静压平衡求内外涵 面积分配比例
    if (bDesign)
    {
        int nCount = 0;
        double Cp5, Cp25, K5, K25, R5, R25, Ckm5, Ckm25, A20, A10, A00;
        double QLamd5, QLamd25;
        CpKRFromT(Cp5, K5, R5, T5C, far5, dAir);
        CpKRFromT(Cp25, K25, R25, T25C, 0.0, dAir);
        Ckm5 = sqrt(K5 / R5 * pow(2.0 / (K5 + 1.0), (K5 + 1.0) / (K5 - 1.0)));
        Ckm25=sqrt(K25 / R25 * pow(2.0 / (K25 + 1.0), (K25 + 1.0) / (K25 - 1.0)));
        A00 = Wg5 * sqrt(T5C) / P5C / Ckm5 / 1.0E+5;
        A10 = A00;
        A20 = lpEngine1->AMix;
        //利用二分法迭代
        do
        {
            QLamd5 = Wg5 * sqrt(T5C) / P5C / Ckm5 / A00 / 1.0E+5;
            Lamd5 = LamdQ(QLamd5, K5, 0);
            QLamd25 = Wa22 * sqrt(T25C) / P25C / Ckm25 / (lpEngine1->AMix - A00) / 1.0E+5;
            Lamd25 = LamdQ(QLamd25, K25, 0);
            P5S = P5C * Pilamd(K5, Lamd5);
            P25S = P25C * Pilamd(K25, Lamd25);
            if ((nCount > 50) || (fabs((P5S - P25S) / P25S) < 1.0E-5))
            {
                Amix1 = A00;
                Amix2 = lpEngine1->AMix - A00;
                break;
            }
            if (P5S < P25S)A10 = A00;
            if (P5S > P25S)A20 = A00;
            A00 = (A10 + A20) / 2.0;
            nCount++;
        } while (1);
    }
    //求混合器出口总温T6C
    Wg6 = Wg5 + Wa22;
    far6 = Wfb / (Wg6 - Wfb);
    H6 = (Air('H', T5C, far5, dAir) * Wg5 + Air('H', T25C, 0.0, dAir) * Wa22)/Wg6;
    TFromHS('H', T6C, H6, far6, dAir);
    //求混合器出口总压P6C
    CpKRFromT(Cp, K1, R, T25C, 0.0, dAir);
    ckm = sqrt(K1 / R * pow(2.0 / (K1 + 1), (K1 + 1) / (K1 - 1)));
    Lamd25 = LamdQ(Wa22 * sqrt(T25C) / P25C / ckm / Amix2 / 1E5, K1, 0);
    CpKRFromT(Cp, K2, R, T5C, far5, dAir);
    ckm = sqrt(K2 / R * pow(2.0 / (K2 + 1), (K2 + 1) / (K2 - 1)));
    Lamd5 = LamdQ(Wg5 * sqrt(T5C) / P5C / ckm / Amix1 / 1E5, K2, 0);
    CpKRFromT(Cp, K3, R, T6C, far6, dAir);
    QA = P5C * Flamd(K2, Lamd5) + P25C * Flamd(K1, Lamd25) * Amix2 / Amix1;
    QB = P5C * Qlamd(K2, Lamd5) * sqrt(T6C / T5C) + P25C * Amix2 / Amix1 * Qlamd(K1, Lamd25);
    QC = pow((K3 + 1.0) / 2.0, 1.0 / (K3 - 1.0));
    Lamd6 = QA * QC / QB - sqrt(pow(QA * QC / QB, 2) - 4.0);
    P6C = lpEngine1->tprc_Mix * QA / Flamd(K3, Lamd6) / (1.0 + Amix2 / Amix1);
    //计算内、外涵道混合器进口静压
    P5S = P5C * Pilamd(K2, Lamd5);
    P25S = P25C * Pilamd(K1, Lamd25);
}

#include "inlet.h"
#include<cmath>
#include "aerodynamic_function.h"
#include "calcu_fuction.h"
#include "gas.h"
#include<engine_law.h>
#include<iostream>
#include<QTextStream>
#include"global_variables.h"

 Inlet::Inlet()
{
    QTextStream qin(stdin);
    QTextStream qout(stdout);
    double tt,cp,R,Kk;

    //对dAir,tamp tt Xmfly pamp进行初始赋值
    tamp=tt=298.15;   //空气温度
    pamp=101325;   //大气压力
    Xmfly=0.75;  //飞行马赫数

    //进气道指针
    lpInhale lpInhale1;

    //初始化进气道类型
    qout<<"请输入进气道损失类型";
    int i;
    qin>>i;
    lpInhale1.MODE=set_lpinlet_point(i);

    switch (lpInhale1.MODE)
        {
        case 0://忽略进气道损失
            tprc_inhale = 1;
            break;
        case 1://施工进气道
            tprc_inhale = 0.97;
            break;
        case 2://国标给定的进气道
            tprc_inhale = Xmfly <= 1 ? 1 : 1.0 - 0.075 * pow(Xmfly - 1.0, 1.35);
            break;
        case 3://美军军标给定的进气道
            tprc_inhale = Xmfly <= 1 ? 0.97 : 0.97 * (1 - 0.1 * pow(Xmfly - 1, 1.95));
            break;
        case 4:
            Qip(lpInhale1.nPoints, lpInhale1.AM, &lpInhale1.tprc_inhale, Xmfly, tprc_inhale);
        }

        do {
            CpKRFromT(cp, Kk, R, tt, 0, dAir);
            T2C = T1C = tamp * (1 + (Kk - 1) * Xmfly * Xmfly / 2);
            if (fabs((tt - T1C) / tt) < 1e-6)
                break;
            tt = T1C;
        } while (1);
            CpKRFromT(cp, Kk, R, T1C, 0, dAir);
            Vtas = Xmfly * sqrt(Kk * R * tamp);
            P1C = pamp * pow(1 + (Kk - 1) * Xmfly * Xmfly / 2.0, Kk / (Kk - 1));
            if (bDesign)tprc_inhale = 1.0;
            P2C = P1C*tprc_inhale;           

}

int Inlet::set_lpinlet_point(int MODE)
{
    return MODE;
}

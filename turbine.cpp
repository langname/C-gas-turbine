#include "turbine.h"
#include<cmath>
#include "aerodynamic_function.h"
#include "calcu_fuction.h"
#include "gas.h"
#include"global_variables.h"

 Turbine::Turbine(int MAX_POINTS,char str)
{
        int i, j;
        double H41, Dhthrc, WaCool, HCool, kCool;
        lpTurb lpTurbH;
        lpTurb lpTurbL;
        double* LtdT4C   = new double[MAX_POINTS];
        double* AetaCor  = new double[MAX_POINTS];
        double* DhthrAry = new double[MAX_POINTS];
        double* AetaAry  = new double[MAX_POINTS];
        double  wgdes, ltdes, eptdes;
        double dtemp1;

        switch (str) {
        case 'H':
            //高压涡轮首轮静叶冷却
            HCool = 0;
            WaCool = 0;
            kCool = 0;
            for (i = 0; i < lpEngine1->nCoolAir; i++)
            {
                lpCoolItem = lpEngine1->CoolAirItem + i;
                HCool += (lpCoolItem->dhSBlade * lpCoolItem->WaExport * Air('H', lpCoolItem->TC, 0, dAir));
                WaCool += (lpCoolItem->WaExport * lpCoolItem->dhSBlade);
                kCool += (lpCoolItem->WaExport * lpCoolItem->dhSBlade * lpCoolItem->khPower);
            }
            //根据能量守恒方程计算冷却后的温度T41C
            far4 = Wfb / Wa3;
            Wg4 = Wa3 + Wfb + WaCool;
            H41 = (Air('H', T4C, far4, dAir) * (Wa3 + Wfb) + HCool) / Wg4;
            far4 = Wfb / (Wg4 - Wfb);
            TFromHS('H', T41C, H41, far4, dAir);

            break;
        case 'L':
            T41C=T4BC;
            far5=far4B;
            break;
        }
        //设计工况下
        if (bDesign)
        {
            switch (str) {
            case 'H':
            {
                T41C_DES = T41C;
                P41C_DES = P4C;
                //求高压涡轮特性图上设计点换算流量、换算功、效率
                dtemp1 = lpTurbH.Wacor_Array[lpTurbH.nPoint_Wacor - 1] -lpTurbH.Wacor_Array[0];
                wgdes = dtemp1 * lpTurbH.Des_ZT + lpTurbH.Wacor_Array[0];//换算流量

                for (i = 0; i < lpTurbH.nPoint_Wacor; i++)
                {
                    for (j = 0; j < lpTurbH.nPoint_nCor; j++)
                    {
                        LtdT4C[j] = lpTurbH.LtCor_Array[j][i];//换算功
                        AetaCor[j] = lpTurbH.Ept_Array[j][i];//换算效率
                    }
                    Qip(lpTurbH.nPoint_nCor, lpTurbH.CorLines, LtdT4C, lpTurbH.Des_CorLine, DhthrAry[i]);
                    Qip(lpTurbH.nPoint_nCor, lpTurbH.CorLines, AetaCor, lpTurbH.Des_CorLine, AetaAry[i]);
                }

                Qip(lpTurbH.nPoint_Wacor, lpTurbH.Wacor_Array, DhthrAry, wgdes, ltdes);
                Qip(lpTurbH.nPoint_Wacor, lpTurbH.Wacor_Array, AetaAry, wgdes, eptdes);

                double n2tcor = n2r_H * sqrt(T41C_DES / T41C);
                Dhthrc = NCH / (T41C * (Wg4 + kCool)) / lpEngine1->EpmH;
                double Wg4cor_Cal = Wg4 * sqrt(T41C) / P4C;
                //求换算转速、效率、换算功、换算流量的耦合系数
                lpTurbH.Co_CorLine = lpTurbH.Des_CorLine / n2tcor;
                lpTurbH.Co_Ept = eptdes / lpEngine1->EptH;
                lpTurbH.Co_LtCor = ltdes / Dhthrc;
                lpTurbH.Co_Wacor = wgdes / Wg4cor_Cal;
                break;
            }
            case 'L':
            {
                T41C_DES = T41C;
                P41C_DES = P4C;
                //求特性图上设计点换算流量、换算功、效率
                dtemp1 = lpTurbL.Wacor_Array[lpTurbL.nPoint_Wacor - 1] -lpTurbL.Wacor_Array[0];
                wgdes = dtemp1 * lpTurbL.Des_ZT + lpTurbL.Wacor_Array[0];//换算流量

                for (i = 0; i < lpTurbL.nPoint_Wacor; i++)
                {
                    for (j = 0; j < lpTurbL.nPoint_nCor; j++)
                    {
                        LtdT4C[j] = lpTurbL.LtCor_Array[j][i];//换算功
                        AetaCor[j] = lpTurbL.Ept_Array[j][i];//换算效率
                    }
                    Qip(lpTurbL.nPoint_nCor, lpTurbL.CorLines, LtdT4C, lpTurbL.Des_CorLine, DhthrAry[i]);
                    Qip(lpTurbL.nPoint_nCor, lpTurbL.CorLines, AetaCor, lpTurbL.Des_CorLine, AetaAry[i]);
                }

                Qip(lpTurbL.nPoint_Wacor, lpTurbL.Wacor_Array, DhthrAry, wgdes, ltdes);
                Qip(lpTurbL.nPoint_Wacor, lpTurbL.Wacor_Array, AetaAry, wgdes, eptdes);

                double n2tcor = n2r_L * sqrt(T41C_DES / T41C);
                Dhthrc = NCH / (T41C * (Wg4B + kCool)) / lpEngine1->EpmH;
                double Wg4cor_Cal = Wg4 * sqrt(T41C) / P4C;
                //求换算转速、效率、换算功、换算流量的耦合系数
                lpTurbH.Co_CorLine = lpTurbL.Des_CorLine / n2tcor;
                lpTurbH.Co_Ept = eptdes / lpEngine1->EptH;
                lpTurbH.Co_LtCor = ltdes / Dhthrc;
                lpTurbH.Co_Wacor = wgdes / Wg4cor_Cal;
                break;
            }
            }

        }

        switch (str) {
        case 'H':
        {
            //非设计工况
            //求涡轮换算转速
            double n2tcor = n2r_H * sqrt(T41C_DES / T41C);
            n2tcor = n2tcor * lpTurbH.Co_CorLine;
            //求涡轮的换算功
            Dhthrc = NCH / (T41C * (Wg4 + kCool)) / lpEngine1->EpmH;
            //求涡轮的换算流量
            Wg4cor_Cal = Wg4 * sqrt(T41C) / P4C;

            //在涡轮特性图上线性插值求出工作点参数
            for (i = 0; i < lpTurbH.nPoint_Wacor; i++)
            {
                for (j = 0; j < lpTurbH.nPoint_nCor; j++)
                {
                    LtdT4C[j] = lpTurbH.LtCor_Array[j][i];
                    AetaCor[j] = lpTurbH.Ept_Array[j][i];
                }
                Qip(lpTurbH.nPoint_nCor, lpTurbH.CorLines, LtdT4C, n2tcor, DhthrAry[i]);
                Qip(lpTurbH.nPoint_nCor, lpTurbH.CorLines, AetaCor, n2tcor, AetaAry[i]);
            }

            Qip(lpTurbH.nPoint_Wacor, DhthrAry, lpTurbH.Wacor_Array, dtemp1, Wg4cor_Fig);
            Qip(lpTurbH.nPoint_Wacor, DhthrAry, AetaAry, dtemp1, AetaFig);
            //求涡轮实际工作点参数
            Wg4cor_Fig = Wg4cor_Fig / lpTurbH.Co_Wacor;//实际流量
            EptH = AetaFig / lpTurbH.Co_Ept;//实际效率
            //求涡轮出口总温、总压、落压比
            double H4S4B, H4BS, T4BS, Cp, kAir, rAir1, rAir2, rAir, Cp1, Cp2;
            H4S4B = Dhthrc * T41C;
            H4BS = Air('H', T41C, far4, dAir) - H4S4B;
            TFromHS('H', T4BS, H4BS, far4, dAir);
            CpKRFromT(Cp1,kAir,rAir1,T41C,far4,dAir);
            CpKRFromT(Cp2, kAir, rAir2, T4BS, far4, dAir);
            rAir = (rAir1 + rAir2) / 2;
            Cp = (Cp1 + Cp2) / 2.0;
            Pit = pow(1.0 - (1.0 - T4BS / T41C) / EptH, -Cp / rAir);
            P4BC = P4C / Pit;
            break;
        }
        case 'L':
        {
            //非设计工况
            //求涡轮换算转速
            double n2tcor = n2r_L * sqrt(T41C_DES / T41C);
            n2tcor = n2tcor * lpTurbL.Co_CorLine;
            //求涡轮的换算功
            Dhthrc = NCH / (T41C * (Wg4 + kCool)) / lpEngine1->EpmH;
            //求涡轮的换算流量
            Wg4Bcor_Cal = Wg4B * sqrt(T41C) / P4BC;

            //在涡轮特性图上线性插值求出工作点参数
            for (i = 0; i < lpTurbL.nPoint_Wacor; i++)
            {
                for (j = 0; j < lpTurbL.nPoint_nCor; j++)
                {
                    LtdT4C[j] = lpTurbL.LtCor_Array[j][i];
                    AetaCor[j] = lpTurbL.Ept_Array[j][i];
                }
                Qip(lpTurbL.nPoint_nCor, lpTurbL.CorLines, LtdT4C, n2tcor, DhthrAry[i]);
                Qip(lpTurbL.nPoint_nCor, lpTurbL.CorLines, AetaCor, n2tcor, AetaAry[i]);
            }

            Qip(lpTurbL.nPoint_Wacor, DhthrAry, lpTurbL.Wacor_Array, dtemp1, Wg4cor_Fig);
            Qip(lpTurbL.nPoint_Wacor, DhthrAry, AetaAry, dtemp1, AetaFig);
            //求涡轮实际工作点参数
            Wg4cor_Fig = Wg4cor_Fig / lpTurbL.Co_Wacor;//实际流量
            EptH = AetaFig / lpTurbL.Co_Ept;//实际效率
            //求涡轮出口总温、总压、落压比
            double H4S4B, H4BS, Cp, kAir, rAir1, rAir2, rAir, Cp1, Cp2;
            H4S4B = Dhthrc * T41C;
            H4BS = Air('H', T41C, far4, dAir) - H4S4B;
            TFromHS('H', T4BS, H4BS, far4, dAir);
            CpKRFromT(Cp1,kAir,rAir1,T41C,far4,dAir);
            CpKRFromT(Cp2, kAir, rAir2, T4BS, far4, dAir);
            rAir = (rAir1 + rAir2) / 2;
            Cp = (Cp1 + Cp2) / 2.0;
            Pit = pow(1.0 - (1.0 - T4BS / T41C) / EptH, -Cp / rAir);
            P5C = P4BC / Pit;
            break;
        }
        }



        //涡轮动叶冷却
        HCool  = 0;
        WaCool = 0;
        for (i = 0; i < lpEngine1->nCoolAir; i++)
        {
            lpCoolItem = lpEngine1->CoolAirItem + 1;
            HCool += (lpCoolItem->dhMBlade*lpCoolItem->WaExport*Air('H',lpCoolItem->TC,0,dAir));
            WaCool += (lpCoolItem->WaExport * lpCoolItem->dhMBlade);

        }
        //求冷却后的温度T5C
        Wg4B = Wg4 + WaCool;
        far4B = Wfb / (Wg4B - Wfb);
        H41 = (Air('H', T4BS, far4, dAir) * Wg4 + HCool) / Wg4B;
        TFromHS('H', T5C, H41, far4B, dAir);
}

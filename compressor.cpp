#include "compressor.h"
#include<cmath>
#include "aerodynamic_function.h"
#include "calcu_fuction.h"
#include "gas.h"
#include<engine_law.h>
#include"inlet.h"
#include<global_variables.h>


Compressor::Compressor(int Max_Points, char str)
{
    double* Pi_Array    = new double[Max_Points];
        double* Wacor_Array = new double[Max_Points];
        double* EpC_Array   = new double[Max_Points];
        double* ZCLEq       = new double[Max_Points];
        double* PicEq       = new double[Max_Points];
        double* WacorEq     = new double[Max_Points];
        double* EpcEq       = new double[Max_Points];
        double PiFig, WacorFig, EpcFig, nCorFig;
        int i, j;

        switch (str) {
        case 'L':
            n1cor= n1r * sqrt(288.15 / T2C);
            break;
        case 'H':
            n1cor= n1r * sqrt(288.15 / T2BC);
            break;
        }

        setmatrix(15,10);
        lpComp lpCompl;//创建一级指针
        lpCompl.PiC_Array   = setArray();//创建嵌套指针
        lpCompl.Wacor_Array = setArray();
        lpCompl.EpC_Array   = setArray();

        switch (str) {
        case 'L':
            //读取外部低压压气机文件

            //将低压压气机数据传入指针

            break;
        case 'H':
            //读取外部高压压气机文件

            //将高压压气机数据传入指针

            break;
        }

        if (bDesign)
        {
            //计算换算转速的耦合系数
            lpCompl.Co_CorLine = lpCompl.Des_CorLine / n1cor;
        }
        nCorFig = n1cor * lpCompl.Co_CorLine;
        //插值求压气机的等换算转速线

        for (i = 0; i < lpCompl.nPoints_Per_Line; i++)
        {
            for (j = 0; j < lpCompl.nCor_Lines; j++)
            {
                Pi_Array[j] = lpCompl.PiC_Array[j][i];
                Wacor_Array[j] = lpCompl.Wacor_Array[j][i];
                EpC_Array[j] = lpCompl.EpC_Array[j][i];
            }
            Qip(lpCompl.nCor_Lines, lpCompl.Corlines, Pi_Array, nCorFig, PicEq[i]);
            Qip(lpCompl.nCor_Lines, lpCompl.Corlines, Wacor_Array, nCorFig, WacorEq[i]);
            Qip(lpCompl.nCor_Lines, lpCompl.Corlines, EpC_Array, nCorFig, EpcEq[i]);
        }
        double y0 = PicEq[0];
        double y2 = PicEq[lpCompl.nPoints_Per_Line - 1];
        double y1;
        for (i = 0; i < lpCompl.nPoints_Per_Line; i++)
        {
            y1 = PicEq[i];
            ZCLEq[i] = (y1 - y0) / (y2 - y0);
        }
        PiFig = ZCL * (y2 - y0) + y0;//ZCL 气体的压力比函数值，与气体种类有关
        //计算压气机特性图上工作点参数WacorFig,EpcFig
        Qip(lpCompl.nPoints_Per_Line, ZCLEq, WacorEq, ZCL, WacorFig);
        Qip(lpCompl.nPoints_Per_Line, ZCLEq, EpcEq, ZCL, EpcFig);


        if (bDesign)   //计算压气机耦合系数
        {
            switch (str) {
            case 'L':
                lpCompl.Co_Pic   = (lpEngine1->PicL - 1) / (PiFig - 1);
                lpCompl.Co_Wacor = lpEngine1->WacL * sqrt(T2C / 288.15) / P2C / WacorFig * 1.01325;
                lpCompl.Co_EpC   = lpEngine1->EpcL / EpcFig;
                break;
            case 'H':
                lpCompl.Co_Pic   = (lpEngine1->PicL - 1) / (PiFig - 1);
                lpCompl.Co_Wacor = lpEngine1->WacL * sqrt(T2BC / 288.15) / P2BC / WacorFig * 1.01325;
                lpCompl.Co_EpC   = lpEngine1->EpcL / EpcFig;
                break;
            }

        }
        //求压气机实际工作点参数，耦合计算
        PiCL = lpCompl.Co_Pic * (PiFig - 1) + 1;
        Wa2cor = lpCompl.Co_Wacor * WacorFig;
        EpCL = lpCompl.Co_EpC * EpcFig;

        switch (str) {
        case 'L':
        {
            double Wa2 = Wa2cor * P2C / sqrt(T2C) * sqrt(288.15) / 1.01325;
            Wa2C=Wa2;
        }
            break;
        case 'H':
        {
            double Wa2B = Wa2cor * P2BC / sqrt(T2BC) * sqrt(288.15) / 1.01325;
        }
            break;
        }

        double tx, sx, Cp, K, rAir, LCI, LC, HOut;
        double PiC, EpC;
        //低压压气机的出口参数 总温T2BC  总压P2BC   实际功 LCL
        PiC = PiCL;
        EpC = EpCL;
        switch (str) {
        case 'L':
            CpKRFromT(Cp, K, rAir, T2C, 0, dAir);
            sx = Air('S', T2C, 0, dAir) + rAir * log(PiC);
            TFromHS('S', tx, sx, 0, dAir);  //tx赋值
            LCI = Air('H', tx, 0, dAir) - Air('H', T2C, 0, dAir);
            LC = LCI / EpC;
            HOut = Air('H', T2C, 0, dAir) + LC;
            TFromHS('H', T2BC, HOut, 0, dAir);
            P2BC = P2C * PiC;
            LCL = LC * Wa2C;
            break;
        case 'H':
            CpKRFromT(Cp, K, rAir, T2BC, 0, dAir);
            sx = Air('S', T2BC, 0, dAir) + rAir * log(PiC);
            TFromHS('S', tx, sx, 0, dAir);  //tx赋值
            LCI = Air('H', tx, 0, dAir) - Air('H', T2BC, 0, dAir);
            LC = LCI / EpC;
            HOut = Air('H', T2BC, 0, dAir) + LC;
            TFromHS('H', T3C, HOut, 0, dAir);
            P3C = P2BC * PiC;
            LCL = LC * Wa2B;
            break;
        }


        if (bDesign)
        {
            T2BC_DES = T2BC;
            P2BC_DES = P2BC;
        }


        //只有高压压气机出口才需要引气，在这里对高压压气机和低压压气机进行区分
        switch (str) {
        case 'L':
            break;
        case 'H':
            //计算引气参数   引气总温、总压、流量
            for (i = 0; i < lpEngine1->nCoolAir; i++)
            {
                lpCoolItem = lpEngine1->CoolAirItem +i;
                if (lpCoolItem->Mode == 0)//引气部位：压气机后引气
                {
                    lpCoolItem->TC = T3C;
                    lpCoolItem->PC = P3C;
                    lpCoolItem->WaExport = lpCoolItem->dExport * Wa2B;
                }
            }
            //计算引气之后的压气机出口流量
            for (i = 0; i < lpEngine1->nCoolAir; i++)
            {
                lpCoolItem = lpEngine1->CoolAirItem + i;
                if (lpCoolItem->Mode == 0)Wa2B -= lpCoolItem->WaExport;
            }
            Wa3=Wa2B;
            break;
        }
}

void Compressor::readFile()
{

}

void Compressor::setPoint()
{

}

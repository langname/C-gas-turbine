#ifndef TURBINE_H
#define TURBINE_H
#include<burner.h>
#include "engine_law.h"
#include"compressor.h"
#include"burner.h"

extern double n2r_H,n2r_L;//转速作为输入变量须先设定
extern double Wg4;//高压涡轮进口
extern double P4BC,T4BC,Wg4B,far4B;//高压涡轮出口截面
extern double P5C,T5C,Wg5,far5;//低压涡轮出口界面
extern double Wg4cor_Fig,Wg4Bcor_Fig,Wg4cor_Cal,Wg4Bcor_Cal;

class Turbine
{
private:
    double dAir;
    double far4 ;
    double T41C;
    double T41C_DES, P41C_DES; 
    double NCH;//高压压气机功率
    double  AetaFig, EptH;
    double Pit;//落压比
    double T4BS;
    static int ROW, COL;
    struct lpTurb
    {
        double* Wacor_Array=new double[ROW];
        double** LtCor_Array;
        double** Ept_Array;
        int nPoint_Wacor, nPoint_nCor ;
        double Des_ZT, *CorLines, Des_CorLine;
        double Co_CorLine, Co_Ept, Co_LtCor, Co_Wacor;
    };
public:
    void SET_ARRAY(int row, int col)
    {
        ROW = row;
        COL = col;
    }
    double** setArray()
    {
        double** p = new double* [ROW];
        for (int i = 0; i < ROW; i++)
            p[i] = new double[COL];
        return p;
    }
public:
     Turbine(int MAX_POINTS,char str);
};

#endif // TURBINE_H

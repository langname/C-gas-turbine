#ifndef COMPRESSOR_H
#define COMPRESSOR_H

extern double T2BC,P2BC,Wa2B;//高压压气机进口温度压力流量
extern double T22C,P22C,Wa2C;//低压压气机出口温度压力流量
extern double T3C,P3C,Wa3;//高压压气机出口温度压力流量
extern double n1r,ZCL;//ZCL压力比函数，与气体种类有关
class Compressor
{
private:
    double n1cor;
    double PiCL, Wa2cor, EpCL;
    double dAir;//气体湿度
    double LCL;//压气机耗功
    double T2BC_DES, P2BC_DES;//设计点的出温度和压力

    static int ROW, COL;
        struct lpComp
        {
            double** PiC_Array;
            double** Wacor_Array;
            double** EpC_Array;
            double Des_CorLine, n1cor, Co_CorLine;
            double* Corlines=new double[ROW];
            int nCor_Lines, nPoints_Per_Line;
            double Co_Pic, Co_Wacor, Co_EpC;//

        };
public:
    Compressor(int Max_Points,char str);
    void readFile();
    void setPoint();
    void setmatrix(int row,int col)//
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
};

#endif // COMPRESSOR_H

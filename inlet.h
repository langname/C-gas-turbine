#ifndef INLET_H
#define INLET_H


extern double T2C, P2C;//由于P2C和T2C在其余地方也会反复使用，所以将其定义为全局变量
extern double Vtas;
class Inlet
{
private:
    double tprc_inhale;
    double Xmfly;//飞行马赫数
    double pamp, tamp;//大气压力温度 需要初始化
    double T1C,P1C;
    double dAir;//燃气湿度


    struct lpInhale
    {
        int MODE;
        static int nPoints;
        double* AM = new double[nPoints];
        double tprc_inhale;
    };


public:
    Inlet();
    int set_lpinlet_point(int MODE);
    void inlet_pass_info(double sender1,double sender2,double &recive1,double &recive2);

};

#endif // INLET_H

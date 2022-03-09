#ifndef BYPASS_H
#define BYPASS_H
#include"global_variables.h"
#include"compressor.h"

extern double Wa22;//外涵道进口
extern double P25C,T25C;//外涵道出口
class Bypass
{
private:

    double tprc_ByPass;//总压恢复系数
    double dAir;//空气湿度
    struct lpByPass
    {
        int Mode=0;
        double Co_tprc, T25CdT22C;
    };
public:
   Bypass();
};

#endif // BYPASS_H

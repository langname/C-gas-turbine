#ifndef MIXER_H
#define MIXER_H
#include"bypass.h"
#include"turbine.h"
#include"burner.h"

extern double P6C, T6C,Wg6,far6; //混合器出口总温T6C   出口流量Wg6
extern double P5S, P25S;//内涵道静压P5S  外涵道静压P25S

class Mixer
{
private:
    double dAir;

    double Amix1, Amix2;//内涵道面积Amix2 外涵道面积Amix1
    double tprc_Afterburn;
public:
     Mixer();
};

#endif // MIXER_H

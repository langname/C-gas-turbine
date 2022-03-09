#ifndef BURNER_H
#define BURNER_H
#include"compressor.h"

extern double P4C,T4C;//燃烧室出口总压和出口总温
extern double Wfb;//燃烧室需要的燃油质量

class Burner
{
private:
    double dAir;
    double Co_tprc, tprc_Burn;

public:
    Burner();
};

#endif // BURNER_H

#ifndef GAS_H
#define GAS_H

extern double Pamp,Tamp;

void Atmosphere(double Alt, double Pamp, double Tamp, double delta_temp, double delta_press);
double BlankWater(char ch,double t);
double BlankAir(char ch, double t, double f);
double Air(char ch, double t, double f, double d);
double ddd(double t, double d);
void TFromHS(char ch, double& t, double hs, double gry, double dAir);
void CpKRFromT(double& Cp, double& K, double& R, double tAir, double gry, double dAir);
class Gas
{
public:
    Gas();

};

#endif // GAS_H

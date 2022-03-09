#ifndef AERODYNAMIC_FUNCTION_H
#define AERODYNAMIC_FUNCTION_H

double LamdQ(double W, double K, double E);
double Qlamd(double K, double lamd);
double Pilamd(double K, double lamd);
double Flamd(double K, double lamd);
double LamdPi(double Pi, double K);
class Aerodynamic_function
{
public:
    Aerodynamic_function();

};

#endif // AERODYNAMIC_FUNCTION_H

#include "info_base.h"

Info_base::Info_base()
{

}

void Info_base::Inlet_info_in(double Inlet_P2C, double Inlet_T2C)
{
    P2C=Inlet_P2C;
    T2C=Inlet_T2C;
}

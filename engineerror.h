#ifndef ENGINEERROR_H
#define ENGINEERROR_H

extern double Error[5];

class EngineError
{
public:
    double MaxP3;
    double A8_eng;
    double Pit_Ctrl;
    double Wg4Bcor_Cal,Wg4cor_Cal;
public:
     EngineError(int Mode);
};

#endif // ENGINEERROR_H

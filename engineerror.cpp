#include "engineerror.h"
#include"nozzle.h"

 EngineError::EngineError(int Mode)
{
    int type=lpEngine1->EngineType;

    //高压流量平衡残差
    Error[0]=(Wg4cor_Cal-Wg4cor_Fig)/Wg4cor_Fig;
    //低压流量平衡残差
    Error[1]=(Wg4Bcor_Cal-Wg4Bcor_Fig)/Wg4Bcor_Fig;

    switch (type)
    {
    //双转子混排涡扇发动机
    case 1:
        // 内、外涵静压平衡残差
        Error[2]=(P5S-P25S)/P5S/100;
        break;
        //双转子涡喷发动机
    case 2:
        Error[2]=(Wa2-Wa2B)/Wa2B;
        break;
    }
    //尾喷管临界界面面积平衡残量
    Error[3]=(A8_calcu-A8_eng)/A8_eng;

    switch (Mode)
    {
    case 104:
        //高压压气机出口压力限制为常数
        //Engine.MaxP3为高压压气机出口压力限制值
        Error[4]=(P3C-MaxP3)/MaxP3;
        break;
    case 4:
    case 5:
    case 6:
        //涡轮落压比为常数的调节规律
        Error[3]=(P4C/P5C-Pit_Ctrl)/Pit_Ctrl;
        break;
    }
}

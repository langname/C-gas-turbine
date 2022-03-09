#include "base_calcu.h"
#include"global_variables.h"

Base_Calcu::Base_Calcu()
{
    int EngineType,EngineAfterBurn;

    EngineType=lpEngine1->EngineType;
    EngineAfterBurn=lpEngine1->EngineType;

    //低压压气机出口参数计算
    Compressor CompL(15,'L');
    //高压压气机出口参数计算
    Compressor CompH(15,'H');

    //是否为双转子混排涡扇发动机？  是，计算外涵道出口参数
    if(EngineType==1)
        //外涵道出口参数计算
        Bypass bypass1();
    else
    {
        BPR=-1;
        T25C=-1;
        P25C=-1;
    }

    //主燃烧室出口计算
    Burner Burner1();
    //高压涡轮出口计算
    Turbine TurbH();

    //低压涡轮出口参数计算
    Turbine TurnL();

    //是否为双转子混排涡扇发动机？  是，计算混合器出口参数
    if(EngineType==2)
    {
        //混合器计算
        Mixer Mixer1();
    }
    else
    {
        T6C=T5C;
        P6C=P5C;
        Wg6=Wg5;
        far6=far5;
    }

    //有无加力燃烧？  有的话，计算加力燃烧室出口参数
    if(EngineAfterBurn)
    {
        //加力燃烧室计算
        AfterBurner AfterBurner1();;
    }
    else
    {
        T7C=T6C;
        P7C=P6C;
        Wg7=Wg6;
        far7=far6;
        Wfaf=0;
    }

    //尾喷管出口参数计算，发动机性能参数计算
    Nozzle Nozzle1();
}

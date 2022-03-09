#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H
typedef struct lpCoolAirItem
{

        int Mode=0;
        double TC, PC, WaExport, dExport;//Wa为质量流量   dExpor为引流流量占总流量的百分数
        double dhSBlade, dhMBlade,khPower;

}LPCOOLAIRITEM;

typedef struct lpEngine
{

        double PicL, WacL, EpcL;
        double tprc25;
        double A22,A3;
        double tprc_Burn, eff_Burn, FHV_Burn;
        int nCoolAir;//总的引流点位
        double EpmH, EptH;
        double AMix,tprc_Mix,tprc_AfterBurn1, tprc_AfterBurn2, tprc_AfterBurn3;
        double eff_AfterBurn, FHV_AfterBurn;
        //尾喷管
        int Mode_Nozzle;
        double MinA9dA8;
        double cd9;
        int EngineType;

        LPCOOLAIRITEM * CoolAirItem;

}LPENGINE;


extern LPENGINE *lpEngine1;
extern LPCOOLAIRITEM *lpCoolItem;
extern int bDesign;
extern void Set_lpEngine();
extern void Set_lpCoolItem();

class Global_Variables//发动机指针数据以及冷却指针数据
{
public:
    Global_Variables();
};

#endif // GLOBAL_VARIABLES_H

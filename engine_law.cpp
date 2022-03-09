#include "engine_law.h"
#include<cmath>
#include<gaussfunction.h>
#include<base_calcu.h>
#include"engineerror.h"
#include<turbine.h>
#include<burner.h>

 Engine_Law::Engine_Law(int Mode, double &Gn1r, double &Gn2r, double &GZcl, double &GZch, double &GT4)
{
    double y[5][6];
    double a[36],b[6],x[6];
    double zm;
    int nCount,i,j,NN=0;
    const double CC=1.002;
    bDesign=0;

    //根据不同的调节规律确定试给参数的个数
    switch (Mode) {
    case 1:
      case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        nCount=5;
        break;
    case 104:
        nCount=6;
        break;
    }
    //按照N+1点列量法的数学模型计算各 试给参数 的各次 试给值
    do
    {
        for(i=0;i<nCount;i++)//第一次i循环
        {
            switch (Mode) {
            case 1: //n1r=const
            case 4:
                y[0][i]=GZcl;
                y[1][i]=GZch;
                y[2][i]=GT4;
                y[3][i]=Gn2r;
                n1r=Gn1r;
             break;
            case 2: //n2r=const
            case 5:
                y[0][i]=GZcl;
                y[1][i]=GZch;
                y[2][i]=Gn1r;
                y[3][i]=GT4;
                n2r=Gn2r;
                break;
            case 3://T4C=comst
            case 6:
                y[0][i]=GZcl;
                y[1][i]=GZch;
                y[2][i]=Gn1r;
                y[3][i]=Gn2r;
                T4C=GT4;
                break;
            case 104:
                //P3C= const
                y[0][i]=GZcl;
                y[1][i]=GZch;
                y[2][i]=Gn1r;
                y[3][i]=Gn2r;
                y[4][i]=GT4;
                break;
            }
         if(i)y[i-1][i]*=CC;
        }


        //取各次试给参数值，进行发动机热力计算，并检验残量是否满足要求
        for(i=0;i<nCount;i++)//第二次i循环
        {
            switch (Mode) {
            case 1://n1r=const
            case 4:
                ZCL=y[0][i];
                ZCH=y[1][i];
                n2r=y[3][i];
                T4C=y[2][i];
                break;
            case 2://n2r=const
            case 5:
                ZCL=y[0][i];
                ZCH=y[1][i];
                n1r=y[2][i];
                T4C=y[3][i];
                break;
            case 3://T4c=const
            case 6:
                ZCL=y[0][i];
                ZCH=y[1][i];
                n1r=y[2][i];
                n2r=y[3][i];
                break;
            case 104:
                ZCL=y[0][i];
                ZCH=y[1][i];
                n1r=y[2][i];
                n2r=y[3][i];
                GT4=y[4][i];
                break;
            }
            //进行发动机热力计算
            Base_Calcu();
            //计算各个平衡方程残差
            EngineError EngineError1(Mode);
            zm=0;
            for(j=0;j<nCount-1;j++)//j循环
                zm+=(Error[j]*Error[j]);
            zm=sqrt(zm);
            //检查残差是否满足收敛要求
            if(zm<1e-5)
            {
                //找到发动机各部件共同工作点
                switch (Mode) {
                case 1:
                case 4:
                    GZcl=y[0][i];
                    GZch=y[1][i];
                    Gn2r=y[3][i];
                    GT4=y[2][i];
                    break;
                case 2:
                case 5:
                    GZcl=y[0][i];
                    GZch=y[1][i];
                    Gn1r=y[2][i];
                    GT4=y[3][i];
                    break;
                case 3:
                case 6:
                    GZcl=y[0][i];
                    GZch=y[1][i];
                    Gn1r=y[2][i];
                    Gn2r=y[3][i];
                    break;
                case 104:
                    GZcl=y[0][i];
                    GZch=y[1][i];
                    Gn1r=y[2][i];
                    Gn2r=y[3][i];
                    GT4=y[4][i];
                    break;
                }
//                return NN;
            }


            //将残差存储到数组，方便求下次试给值时使用
            for(j=1;j<nCount;j++)
                a[j*nCount+i]=Error[j-1];
        }
        for(j=0;j<nCount;j++)
        {
            a[j]=1.0;
            b[j]=0.0;
        }
        b[0]=1.0;
        //高斯法求解线性方程组
        GaussFunction(nCount,a,b,x);
        //根据不同的发动机调节规律，求下次迭代的试给值
        switch (Mode) {
        case 1:
        case 4:
            GZcl=0;
            GZch=0;
            Gn2r=0;
            GT4=0;
            for(j=0;j<nCount;j++)
            {
                GZcl+=(y[0][j]*x[j]);
                GZch+=(y[1][j]*x[j]);
                Gn2r+=(y[3][j]*x[j]);
                GT4+=(y[2][j]*x[j]);
            }
            break;
        case 2:
        case 5:
            GZcl=0;
            GZch=0;
            Gn1r=0;
            GT4=0;
            for(j=0;j<nCount;j++)
            {
                GZcl+=(y[0][j]*x[j]);
                GZch+=(y[1][j]*x[j]);
                Gn1r+=(y[2][j]*x[j]);
                GT4+=(y[3][j]*x[j]);
            }
            break;
        case 3:
        case 6:
            GZcl=0;
            GZch=0;
            Gn2r=0;
            Gn1r=0;
            for(j=0;j<nCount;j++)
            {
                GZcl+=(y[0][j]*x[j]);
                GZch+=(y[1][j]*x[j]);
                Gn1r+=(y[2][j]*x[j]);
                Gn2r+=(y[3][j]*x[j]);
            }
            break;
        case 104:
            GZcl=0;
            GZch=0;
            Gn2r=0;
            GT4=0;
            Gn1r=0;
            for(j=0;j<nCount;j++)
            {
                GZcl+=(y[0][j]*x[j]);
                GZch+=(y[1][j]*x[j]);
                Gn1r+=(y[2][j]*x[j]);
                Gn2r+=(y[3][j]*x[j]);
                GT4+=(y[4][j]*x[j]);
            }
            break;
        }
        NN++;
    }while(NN<30);
//    return NN;
}











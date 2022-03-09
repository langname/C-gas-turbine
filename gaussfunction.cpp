#include "gaussfunction.h"
#include<cmath>

void GaussFunction(int &count, double a[], double b[], double x[])
{
    int m,n,i,j,k;
    m=n=count;
    //构建矩阵
    double **A=new double*[m];
    for(i=0;i<m;i++)
        A[i]=new double [n+1];
    double *temp=new double[n+1];
    switch (count) {
    case 5:
    {
        for( i=0;i<count;i++)
        {
            for( j=0;j<count+1;j++)
            {
                if(j=5)
                {
                    A[i][j]=b[i];
                }
                A[i][j]=a[i*count+j];
            }
        }
    };
        break;
    case 6:
    {
        for(int i=0;i<count;i++)
        {
            for(int j=0;i<count+1;j++)
            {
                if(j=6)
                {
                    A[i][j]=b[i];
                }
                A[i][j]=a[i*count+j];
            }
        }
    };
        break;
    }


    double d=1;

    for( k=0;k<count-1;k++)
    {   //列主元
        double max=0;
        int hang=0,num=0;
        for(int i=k;i<count;i++)
        {
            if(fabs(A[i][k])>max)
            {
                max=fabs(A[i][k]);
                hang=i;
            }
        }
        for(i=0;i<count+1;i++)//换行
        {
            temp[i]=A[k][i];
            A[k][i]=A[hang][i];
            A[hang][i]=temp[i];
        }

        //消元
        for(i=k+1;i<count;i++)
        {
            d=A[i][k]/A[k][k];
            for(j=0;j<n+1;j++)
            {
                A[i][j]=A[i][j]-d*A[k][j];
            }
        }
    }

    //求解解向量
    for(i=m-1;i>=0;i--)
    {
        d=0;
        for(k=0;k<n;k++)
        {
            d=d+temp[k]*A[i][k];
        }
        x[i]=(A[i][n]-d)/A[i][i];
    }

    //释放内存
    for(i=0;i<m;i++)delete[] A[i];
        delete[] A;

    delete[] temp;
}


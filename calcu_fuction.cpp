#include "calcu_fuction.h"

Calcu_fuction::Calcu_fuction()
{

}

void Qip(int n, double x[], double y[], double x0, double &y0)
{
    int i, j;
        double x1, y1;
        //对数值按自变量升序进行排序
        for (i = 0; i < n - 1; i++)
        {
            for (j = i + 1; j < n; j++)
            {
                if (x[i] > x[j])
                {
                    x1 = x[j];
                    y1 = y[j];
                    x[j] = x[i];
                    y[j] = y[i];
                    x[i] = x1;
                    y[i] = y1;
                }
            }
        }
        //插值求插值点y的值
        for (i = 0; i < n; i++) { if (x0 < x[i])break; }
        if (i == 0) { i = 1; j = 0; }
        else if (i == n) { i = n - 1; j = n - 2; }
        else { j = i - 1; }
        y0 = y[j] + (y[i] - y[j]) * (x0 - x[j]) / (x[i] - x[j]);
}

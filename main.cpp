/* Main */

#include <FEXTools.h>
#include "Core.h"

int main()
{
    double* x = new double[30];
    double* y = new double[30];

    for (int i = 0; i < 30; i++)
    {
        x[i] = ((double)i) / 2;
        y[i] = 3 * x[i] * x[i];
    }

    DataPoints dataSet(30, x, y);

    dataSet.Draw("/home/bruno/Dev/FEXTools/Test.eps");

    return 0;
}
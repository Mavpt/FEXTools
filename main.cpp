/* Main */

#include <fstream>
#include <FEXTools.h>
#include "Core.h"

#include <iostream>

using namespace std;

int main()
{
    ofstream Stream("/home/bruno/Dev/FEXTools/InputTest.txt");

    double* x   = new double[30];
    double* y   = new double[30];
    double* e_x = new double[30];
    double* e_y = new double[30];

    for (int i = 0; i < 30; i++)
    {
        x[i]   = ((double)i) / 2;
        y[i]   = 5 * x[i] * x[i];
        e_x[i] = .6;
        e_y[i] = 20;

        Stream << ((double)i) / 2 << " " << 50 * i << " " << .3 << " " << 10 << endl;
    }

    Stream.close();

    DataSet DataSetTest1({ "DataSetTest1", "xAxis1", "yAxis1" }, "/home/bruno/Dev/FEXTools/InputTest.txt");
    DataSetTest1.Draw("/home/bruno/Dev/FEXTools/Test.eps");
    DataSet DataSetTest2({ "DataSetTest2", "xAxis2", "yAxis1" }, 30, x, y, e_x, e_y);
    DataSetTest2.Draw("/home/bruno/Dev/FEXTools/Test.eps");

    DataStack DataStackTest({ "DataStackTest", "xAxisStack", "xAxisStack" });
    DataStackTest.Add(DataSetTest1);
    DataStackTest.Add(DataSetTest2);
    DataStackTest.Draw("/home/bruno/Dev/FEXTools/Test.eps");

    delete[] x;
    delete[] y;
    delete[] e_x;
    delete[] e_y;

    return 0;
}
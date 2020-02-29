/* Main */

#include <fstream>
#include <FEXTools.h>
#include "Core.h"

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
        y[i]   = 3 * x[i] * x[i];
        e_x[i] = .5;
        e_y[i] = .5;

        Stream << x[i] << "   " << y[i] << "  " << e_x[i] << "  " << e_y[i] << endl;
    }

    Stream.close();

    // DataSet dataSet("MyDataSetTest", 30, "MyX", x, e_x, "MyY", y, e_y);
    DataSet dataSet("MyDataSetTest", "MyX", "MyY", "/home/bruno/Dev/FEXTools/InputTest.txt");

    dataSet.Draw("/home/bruno/Dev/FEXTools/Test.eps");

    return 0;
}
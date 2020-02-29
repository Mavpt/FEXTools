/* Main */

#include <fstream>
#include <FEXTools.h>
#include "Core.h"

#include <iostream>

using namespace std;

int main()
{
    ofstream Stream("/home/bruno/Dev/FEXTools/InputTest.txt");
    ofstream Stream2("/home/bruno/Dev/FEXTools/InputTest2.txt");

    double* x   = new double[30];
    double* y   = new double[30];
    double* e_x = new double[30];
    double* e_y = new double[30];

    double* x2   = new double[30];
    double* y2   = new double[30];
    double* e_x2 = new double[30];
    double* e_y2 = new double[30];

    for (int i = 0; i < 30; i++)
    {
        x[i]   = ((double)i) / 2;
        y[i]   = 3 * x[i] * x[i];
        e_x[i] = 0;
        e_y[i] = 0;

        x2[i]   = ((double)i) / 2;
        y2[i]   = 5 * x[i];
        e_x2[i] = 0;
        e_y2[i] = 0;

        Stream << x[i] << "   " << y[i] << "  " << e_x[i] << "  " << e_y[i] << endl;
        Stream2 << x2[i] << "   " << y2[i] << "  " << e_x2[i] << "  " << e_y2[i] << endl;
    }

    Stream.close();

    DataSet dataSet({ "MyDataSetTest", "MyX", "MyY" }, "/home/bruno/Dev/FEXTools/InputTest.txt");
    dataSet.Draw("/home/bruno/Dev/FEXTools/Test.eps");
    DataSet dataSet2({ "MyDataSetTest2", "MyX", "MyY" }, "/home/bruno/Dev/FEXTools/InputTest2.txt");
    dataSet2.Draw("/home/bruno/Dev/FEXTools/Test.eps");

    DataStack MyDataStack({ "MyDataStackTest", "MyXTEST", "MyTESTY" });
    MyDataStack.Add(&dataSet);
    MyDataStack.Add(&dataSet2);
    MyDataStack.Draw("/home/bruno/Dev/FEXTools/Test.eps");

    return 0;
}
/* Main */

#include <fstream>
#include <FEXTools.h>
#include "Core.h"

#include <iostream>

using namespace std;

int main()
{
    DataProperties u_DataProperties("FEXToolsTest", "xAxis", 380.0, 1000.0, "yAxis", 0.4, 1.5);

    // DataSet
    DataSet DataSetTest(u_DataProperties, DrawProperties(), "../InputTest.txt");
    DataSetTest.Draw("../DataSetTest.eps");

    // DataStack
    DataStack DataStackTest(u_DataProperties);
    DataStackTest.Add(DataSetTest);
    DataStackTest.Draw("../DataStackTest.eps");

    // FunctionFitter
    FunctionFitter FunctionFitterTest(u_DataProperties, DrawProperties(), "../InputTest.txt", "../Function.txt", "../Function.txt");
    FunctionFitterTest.Draw("../FunctionFitterTest.eps");

    return 0;
}
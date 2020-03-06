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
    DataSet DataSetTest(u_DataProperties, DrawProperties(), "../IO/Data.txt");
    DataSetTest.Draw("../IO/DataSetTest.eps");

    // FunctionFitter
    FunctionFitter FunctionFitterTest(u_DataProperties, DrawProperties(), "../IO/Data.txt", "../IO/Function.txt", "../IO/Function.txt");
    FunctionFitterTest.Draw("../IO/FunctionFitterTest.eps");

    // DataStack
    DataStack DataStackTest(u_DataProperties);
    DataStackTest.Add(&DataSetTest, { kBlue });
    DataStackTest.Draw("../IO/DataStackTest.eps");

    DataStack FunctionDataStackTest(u_DataProperties);
    FunctionDataStackTest.Add(&FunctionFitterTest, { kBlue, kFullDotLarge, 0.5, kMagenta, kSolid, 1 });
    FunctionDataStackTest.Draw("../IO/FunctionDataStackTest.eps", 1);

    return 0;
}
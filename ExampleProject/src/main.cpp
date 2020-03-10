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
    DataSet DataSetTest(u_DataProperties, { kRed, kFullDotLarge, 1 }, "../IO/Data.txt");
    DataSetTest.Draw("../IO/DataSetTest.eps");

    DataSetTest.SetDrawProperties({ kBlue, kFullDotLarge, 0.5 });

    // FunctionFitter
    FunctionFitter FunctionFitterTest(u_DataProperties, { kRed, kFullDotLarge, 1, kRed, kDotted, 1 }, "../IO/Data.txt", "../IO/Function.txt");
    FunctionFitterTest.Draw("../IO/FunctionFitterTest.eps");

    FunctionFitterTest.SetDrawProperties({ kBlack, kFullDotLarge, 0.8, kAquamarine, kSolid, 1 });

    // Interpolator
    Interpolator InterpolatorTest(u_DataProperties, { kRed, kFullDotLarge, 1, kPink, kDotted, 1 }, "../IO/Data.txt");
    InterpolatorTest.Draw("../IO/InterpolatorTest.eps");

    InterpolatorTest.SetDrawProperties({ kBlack, kFullDotLarge, 0.8, kAquamarine, kSolid, 1 });

    // DataStack
    DataStack DataStackTest(u_DataProperties);
    DataStackTest.Add(&DataSetTest);
    DataStackTest.Draw("../IO/DataStackTest.eps");

    DataStack FunctionDataStackTest(u_DataProperties);
    FunctionDataStackTest.Add(&FunctionFitterTest);
    FunctionDataStackTest.Draw("../IO/FunctionDataStackTest.eps");

    DataStack InterpolatorDataStackTest(u_DataProperties);
    InterpolatorDataStackTest.Add(&InterpolatorTest);
    InterpolatorDataStackTest.Draw("../IO/InterpolatorDataStackTest.eps");

    return 0;
}
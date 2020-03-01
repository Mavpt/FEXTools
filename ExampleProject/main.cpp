/* Main */

#include <fstream>
#include <FEXTools.h>
#include "Core.h"

#include <iostream>

using namespace std;

int main()
{
    DataProperties u_DataProperties("FEXToolsTest", "xAxis", 380.0, 1000.0, "yAxis", 0.4, 1.5);

    FunctionFitter FunctionFitterTest(u_DataProperties, DrawProperties(), "../InputTest.txt", "../Function.txt", "../Function.txt");

    FunctionFitterTest.Draw("../Test.eps");

    return 0;
}
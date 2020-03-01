/* Main */

#include <fstream>
#include <FEXTools.h>
#include "Core.h"

#include <iostream>

using namespace std;

int main()
{
    DataProperties u_DataProperties("FEXToolsTest", "xAxis", 380.0, 1000.0, "yAxis", 0.4, 1.5);

    FunctionFitter FunctionFitterTest(u_DataProperties, DrawProperties(), "/home/bruno/Dev/FEXTools/InputTest.txt",
                                      "/home/bruno/Dev/FEXTools/Function.txt", "/home/bruno/Dev/FEXTools/Function.txt");

    FunctionFitterTest.Draw("/home/bruno/Dev/FEXTools/Test.eps");

    return 0;
}
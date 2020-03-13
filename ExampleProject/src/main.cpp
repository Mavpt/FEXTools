/* Main */

#include <fstream>
#include <FEXTools.h>
#include "Core.h"

#include <iostream>

using namespace std;

#define __DEBUG__ 1
#if __DEBUG__

struct AllocationMetrics
{
    uint32_t TotalAllocated = 0, TotalFreed = 0;

    void CurrentUsage()
    {
        std::cout << "CURRENT ALLOCATED MEMORY = " << TotalAllocated - TotalFreed << " (" << TotalAllocated << " - " << TotalFreed << " )" << std::endl;
    }
};

static AllocationMetrics s_AllocationMetrics;

void* operator new(size_t size)
{
    s_AllocationMetrics.TotalAllocated++;
    // s_AllocationMetrics.CurrentUsage();

    return malloc(size);
}

void operator delete(void* memory)
{
    s_AllocationMetrics.TotalFreed++;
    s_AllocationMetrics.CurrentUsage();

    free(memory);
}

#endif

int main()
{
    DataProperties u_DataProperties("FEXToolsTest", "xAxis", 380.0, 1000.0, "yAxis", 0, 1.5);

    // DataSet
    DataSet DataSetTest(u_DataProperties, { kRed, kFullDotLarge, 2, kRed, kDotted, 1 }, "../IO/Data.txt");
    DataSetTest.Draw("../IO/DataSetTest.png");

    // FunctionFitter
    FunctionFitter FunctionFitterTest(u_DataProperties, { kRed, kFullDotLarge, 2, kRed, kDotted, 1 }, "../IO/Data.txt", "../IO/Function.txt");
    FunctionFitterTest.Draw("../IO/FunctionFitterTest.png");

    // Interpolator
    Interpolator InterpolatorTest(u_DataProperties, { kRed, kFullDotLarge, 2, kPink, kDotted, 1 }, "../IO/Data.txt", "../IO/Interpolator.txt");
    InterpolatorTest.Draw("../IO/InterpolatorTest.png");

    // DataStack
    DataStack DataStackTest(u_DataProperties);
    DataStackTest.Add(&DataSetTest);
    DataStackTest.Draw("../IO/DataStackTest.png");

    DataStack FunctionDataStackTest(u_DataProperties);
    FunctionDataStackTest.Add(&FunctionFitterTest);
    FunctionDataStackTest.Draw("../IO/FunctionDataStackTest.png");

    DataStack InterpolatorDataStackTest(u_DataProperties);
    InterpolatorDataStackTest.Add(&InterpolatorTest);
    InterpolatorDataStackTest.Draw("../IO/InterpolatorDataStackTest.png");

    return 0;
}